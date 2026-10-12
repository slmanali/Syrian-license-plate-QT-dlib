#include "capturethread.h"

#include <QByteArray>
#include <QMutexLocker>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <vlc/vlc.h>
#include <atomic>
#include <cstdint>
#include <limits>
#include <memory>
#include <stdexcept>
#include <vector>

namespace {

struct StreamFrames
{
    explicit StreamFrames(cv::Size size)
    {
        // LibVLC requires aligned planes. Pad rows and storage to 32 bytes too.
        pitch = (static_cast<unsigned>(size.width) * 3u + 31u) & ~31u;
        const std::size_t rows = (static_cast<std::size_t>(size.height) + 31u) & ~std::size_t(31u);
        if (rows > (std::numeric_limits<std::size_t>::max() - 31u) / pitch)
            throw std::length_error("Stream frame is too large");
        storage.resize(static_cast<std::size_t>(pitch) * rows + 31u);
        const std::uintptr_t aligned =
                (reinterpret_cast<std::uintptr_t>(storage.data()) + 31u) & ~std::uintptr_t(31u);
        pixels = reinterpret_cast<unsigned char *>(aligned);
    }

    QMutex mutex;
    std::vector<unsigned char> storage;
    unsigned char *pixels = nullptr;
    unsigned pitch = 0;
    bool ready = false;
    std::atomic<bool> failed{false};
};

void *lockVideo(void *opaque, void **planes)
{
    StreamFrames *frames = static_cast<StreamFrames *>(opaque);
    frames->mutex.lock();
    *planes = frames->pixels;
    return nullptr;
}

void unlockVideo(void *opaque, void *, void *const *)
{
    StreamFrames *frames = static_cast<StreamFrames *>(opaque);
    frames->ready = true;
    frames->mutex.unlock();
}

void streamError(const libvlc_event_t *, void *opaque)
{
    static_cast<StreamFrames *>(opaque)->failed.store(true);
}

struct PlayerDeleter
{
    void operator()(libvlc_media_player_t *player) const
    {
        // Stop decoding before the callback context and pixel storage go away.
        libvlc_media_player_stop(player);
        libvlc_media_player_release(player);
    }
};

} // namespace

CaptureThread::CaptureThread(QObject *parent)
    : CaptureThread(0, parent)
{
}

CaptureThread::CaptureThread(int cameraIndex, QObject *parent)
    : QThread(parent), m_source(Source::Camera), m_cameraIndex(cameraIndex)
{
    qRegisterMetaType<cv::Mat>("cv::Mat");
}

CaptureThread::CaptureThread(const std::string &videoPath, QObject *parent)
    : QThread(parent), m_source(Source::VideoFile), m_videoPath(videoPath)
{
    qRegisterMetaType<cv::Mat>("cv::Mat");
}

CaptureThread::CaptureThread(const QString &streamUrl, cv::Size resolution, QObject *parent)
    : QThread(parent), m_source(Source::Stream), m_streamUrl(streamUrl), m_resolution(resolution)
{
    qRegisterMetaType<cv::Mat>("cv::Mat");
}

CaptureThread::~CaptureThread()
{
    requestStop();
    wait();
}

void CaptureThread::requestStop()
{
    QMutexLocker locker(&m_mutex);
    requestInterruption();
    m_stateChanged.wakeAll();
}

void CaptureThread::setPaused(bool paused)
{
    QMutexLocker locker(&m_mutex);
    m_paused = paused;
    m_stateChanged.wakeAll();
}

void CaptureThread::togglePause()
{
    QMutexLocker locker(&m_mutex);
    m_paused = !m_paused;
    m_stateChanged.wakeAll();
}

bool CaptureThread::isPaused() const
{
    QMutexLocker locker(&m_mutex);
    return m_paused;
}

cv::Size CaptureThread::frameSize() const
{
    QMutexLocker locker(&m_mutex);
    return m_frameSize;
}

QString CaptureThread::errorString() const
{
    QMutexLocker locker(&m_mutex);
    return m_error;
}

void CaptureThread::run()
{
    {
        QMutexLocker locker(&m_mutex);
        m_error.clear();
        m_frameSize = cv::Size();
    }
    if (isInterruptionRequested())
        return;

    try {
        if (m_source == Source::Stream)
            runStream();
        else
            runVideo();
    } catch (const std::exception &) {
        // Backend exception text can contain a stream URL and its credentials.
        reportError(tr("Capture failed while opening or decoding the source."));
    }
}

bool CaptureThread::waitForVideo()
{
    QMutexLocker locker(&m_mutex);
    while (m_paused && !isInterruptionRequested())
        m_stateChanged.wait(&m_mutex);
    return !isInterruptionRequested();
}

bool CaptureThread::waitForNextFrame(unsigned long milliseconds)
{
    QMutexLocker locker(&m_mutex);
    if (!isInterruptionRequested())
        m_stateChanged.wait(&m_mutex, milliseconds);
    return !isInterruptionRequested();
}

void CaptureThread::publishFrame(const cv::Mat &frame)
{
    {
        QMutexLocker locker(&m_mutex);
        if (isInterruptionRequested() || m_paused)
            return;
        m_frameSize = frame.size();
    }
    // Queued receivers must not share a buffer that the decoder will overwrite.
    emit frameCaptured(frame.clone());
}

void CaptureThread::reportError(const QString &message)
{
    {
        QMutexLocker locker(&m_mutex);
        m_error = message;
    }
    emit captureError(message);
}

void CaptureThread::runVideo()
{
    cv::VideoCapture capture;
    const bool opened = m_source == Source::Camera
            ? capture.open(m_cameraIndex) : capture.open(m_videoPath);
    if (!opened) {
        reportError(tr("Cannot open the camera or video file."));
        return;
    }

    cv::Mat frame;
    while (waitForVideo()) {
        // Live cameras may report no frame count. A failed read ends the session.
        if (!capture.read(frame) || frame.empty()) {
            if (m_source == Source::Camera && !isInterruptionRequested())
                reportError(tr("The camera stopped delivering frames."));
            break;
        }
        publishFrame(frame);
        if (!waitForNextFrame(40))
            break;
    }
}

void CaptureThread::runStream()
{
    if (m_streamUrl.isEmpty() || m_resolution.width <= 0 || m_resolution.height <= 0 ||
            static_cast<unsigned>(m_resolution.width) >
            (std::numeric_limits<unsigned>::max() - 31u) / 3u) {
        reportError(tr("The stream URL and a positive frame size are required."));
        return;
    }

    // All resources belong to this invocation of run(), including callback data.
    StreamFrames frames(m_resolution);
    const char *args[] = {"--ignore-config", "--no-audio", "--no-video-title-show", "--quiet"};
    std::unique_ptr<libvlc_instance_t, decltype(&libvlc_release)> instance(
                libvlc_new(sizeof(args) / sizeof(args[0]), args), &libvlc_release);
    if (!instance) {
        reportError(tr("Cannot initialize VLC."));
        return;
    }
    const QByteArray url = m_streamUrl.toUtf8();
    std::unique_ptr<libvlc_media_t, decltype(&libvlc_media_release)> media(
                libvlc_media_new_location(instance.get(), url.constData()), &libvlc_media_release);
    if (!media) {
        reportError(tr("Cannot create the VLC media source."));
        return;
    }
    std::unique_ptr<libvlc_media_player_t, PlayerDeleter> player(
                libvlc_media_player_new_from_media(media.get()));
    if (!player) {
        reportError(tr("Cannot create the VLC player."));
        return;
    }

    libvlc_video_set_callbacks(player.get(), lockVideo, unlockVideo, nullptr, &frames);
    libvlc_video_set_format(player.get(), "RV24", m_resolution.width, m_resolution.height, frames.pitch);
    if (libvlc_event_attach(libvlc_media_player_event_manager(player.get()),
                           libvlc_MediaPlayerEncounteredError, streamError, &frames) != 0) {
        reportError(tr("Cannot monitor VLC playback errors."));
        return;
    }
    if (isInterruptionRequested())
        return;
    if (libvlc_media_player_play(player.get()) != 0) {
        reportError(tr("Cannot start the stream."));
        return;
    }

    bool playerPaused = false;
    bool receivedFrame = false;
    while (!isInterruptionRequested()) {
        const libvlc_state_t state = libvlc_media_player_get_state(player.get());
        if (frames.failed.load() || state == libvlc_Error) {
            reportError(tr("VLC could not open or continue the stream."));
            break;
        }

        const bool paused = isPaused();
        // Apply pause after VLC finishes opening, so an early request is retained.
        if (paused != playerPaused && (state == libvlc_Playing || state == libvlc_Paused)) {
            libvlc_media_player_set_pause(player.get(), paused ? 1 : 0);
            playerPaused = paused;
        }
        cv::Mat frame;
        if (!paused) {
            QMutexLocker locker(&frames.mutex);
            if (frames.ready) {
                const cv::Mat rgb(m_resolution, CV_8UC3, frames.pixels, frames.pitch);
                cv::cvtColor(rgb, frame, cv::COLOR_RGB2BGR);
                frames.ready = false;
            }
        }
        if (!frame.empty()) {
            receivedFrame = true;
            publishFrame(frame);
        }
        if (state == libvlc_Ended) {
            if (!receivedFrame)
                reportError(tr("The stream ended without delivering video frames."));
            break;
        }
        if (!waitForNextFrame(20))
            break;
    }
}
