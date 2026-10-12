#include "capturethread.h"
#include <QDebug>
bool pause=false;
Q_DECLARE_METATYPE(cv::Mat)
//***************************************
#define RTSP
#ifdef RTSP
#include <vlc/vlc.h>
libvlc_instance_t *inst;
// RTSP address
//char* rtspAddress = "rtsp://admin:12345@196.10.10.155:554";
// Video resolution WxH
//cv::Size rtspRes(528, 384);

struct VideoDataStruct
{
    int param;
};
int done = 0;
libvlc_media_player_t *mp;
unsigned int videoBufferSize = 0;
uint8_t *videoBuffer = 0;
//***************************************
void cbVideoPrerender(void *p_video_data, uint8_t **pp_pixel_buffer, int size)
{
    if (size > videoBufferSize || !videoBuffer)
    {
        printf("Reallocate raw video buffer %d bytes\n", size);
        free(videoBuffer);
        videoBuffer = (uint8_t *)malloc(size);
        videoBufferSize = size;
    }

    // videoBuffer = (uint8_t *)malloc(size);
    *pp_pixel_buffer = videoBuffer;
}
//***************************************
void cbVideoPostrender(void *p_video_data, uint8_t *p_pixel_buffer, int width, int height, int pixel_pitch, int size, int64_t pts)
{
    // Unlocking
    //CloseHandle(hMutex);

}
//***************************************
static void handleEvent(const libvlc_event_t* pEvt, void* pUserData)
{
    libvlc_time_t time;
    switch (pEvt->type)
    {
    case libvlc_MediaPlayerTimeChanged:
        time = libvlc_media_player_get_time(mp);
        printf("MediaPlayerTimeChanged %lld ms\n", (long long)time);
        break;
    case libvlc_MediaPlayerEndReached:
        printf("MediaPlayerEndReached\n");
        done = 1;
        break;
    default:
        printf("%s\n", libvlc_event_type_name(pEvt->type));
    }
}
#endif
//***************************************
CaptureThread::CaptureThread(QObject* parent):QThread(parent)
{
    capWebam.open(0);
    if(!capWebam.isOpened()) return;
    QThread* t = new QThread();
    moveToThread(t);
    t->start();
    tmrTimer = new QTimer;

    QObject::connect(tmrTimer, SIGNAL(timeout()),this, SLOT(captureFrame()));
    connect(this, SIGNAL(finished()), t, SLOT(quit()));
    tmrTimer->start(40);
}
//***************************************
CaptureThread::CaptureThread(std::string video_path)
{
    capWebam.open(video_path);
    if(!capWebam.isOpened())
    {
        qDebug() << "can't open";
        return;
    }
    QThread* t = new QThread();
    moveToThread(t);
    t->start();
    tmrTimer = new QTimer;

    qRegisterMetaType< cv::Mat >("cv::Mat");
    QObject::connect(tmrTimer, SIGNAL(timeout()),this, SLOT(captureFrame()));
//    connect(this, SIGNAL(finished()), t, SLOT(quit()));
    tmrTimer->start(40);
}
//***************************************
CaptureThread::CaptureThread(QString video_path,cv::Size  rtspRes)
{
#ifdef RTSP
    // VLC pointers
    libvlc_media_t *m;
    void *pUserData = 0;

    VideoDataStruct dataStruct;

    // VLC options
    char smem_options[1000];

    // RV24
    sprintf_s(smem_options
        , "#transcode{vcodec=RV24}:smem{"
        "video-prerender-callback=%lld,"
        "video-postrender-callback=%lld,"
        "video-data=%lld,"
        "no-time-sync},"
        , (long long int)(intptr_t)(void*)&cbVideoPrerender
        , (long long int)(intptr_t)(void*)&cbVideoPostrender
        , (long long int)(intptr_t)(void*)&dataStruct
        );

    const char * const vlc_args[] = {
        "-I", "dummy",            // Don't use any interface
        "--ignore-config",        // Don't use VLC's config
//        "--extraintf=logger",     // Log anything
        "--verbose=1",            // Be verbose
        "--sout", smem_options    // Stream to memory
    };

    // Launch VLC
    inst = libvlc_new(sizeof(vlc_args) / sizeof(vlc_args[0]), vlc_args);
    rtspReso= rtspRes;
    // Create a new item
    m = libvlc_media_new_location(inst, video_path.toStdString().c_str());

    // Create a media player playing environement
    mp = libvlc_media_player_new_from_media(m);

    libvlc_event_manager_t* eventManager = libvlc_media_player_event_manager(mp);
    libvlc_event_attach(eventManager, libvlc_MediaPlayerTimeChanged, handleEvent, pUserData);
    libvlc_event_attach(eventManager, libvlc_MediaPlayerEndReached, handleEvent, pUserData);
    libvlc_event_attach(eventManager, libvlc_MediaPlayerPositionChanged, handleEvent, pUserData);

    //libvlc_video_set_format(mp, "RV24", 240, 320, 240 * 3 );

    // play the media_player
    libvlc_media_player_play(mp);
    #endif
    QThread* t = new QThread();
    moveToThread(t);
    t->start();
    tmrTimer = new QTimer;

    qRegisterMetaType< cv::Mat >("cv::Mat");
    QObject::connect(tmrTimer, SIGNAL(timeout()),this, SLOT(captureFrame1()));
    tmrTimer->start(20);
}
//***************************************
CaptureThread::CaptureThread(cv::Mat ttmp)
{
    src=ttmp;
    QThread* t = new QThread();
    moveToThread(t);
    t->start();
    tmrTimer = new QTimer;

    qRegisterMetaType< cv::Mat >("cv::Mat");
    QObject::connect(tmrTimer, SIGNAL(timeout()),this, SLOT(Frame_detect()));
    connect(this, SIGNAL(finished()), t, SLOT(quit()));
//    QObject::connect(this, SIGNAL(finished()), this, SLOT(terminate()));
//    QObject::connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));
    tmrTimer->start(40);
}
//***************************************
void CaptureThread::run()
{
//    QObject::connect(tmrTimer, SIGNAL(timeout()), this, SLOT(captureFrame()));

    exec();
}
//***************************************
int mmm=10;
int sss=0;
//***************************************
void CaptureThread::captureFrame()
{
    if(capWebam.isOpened()&& (capWebam.get(CV_CAP_PROP_POS_FRAMES)<capWebam.get(CV_CAP_PROP_FRAME_COUNT))&& sss<10)
    {
        if(mmm==capWebam.get(CV_CAP_PROP_POS_FRAMES))
            sss++;
        else
        {
//            qDebug() << capWebam.get(CV_CAP_PROP_POS_FRAMES);
            mmm=capWebam.get(CV_CAP_PROP_POS_FRAMES);
            capWebam.read(capturedFrame);
            cv::Mat ff;
            // = capturedFrame + cv::Scalar(-50, -50, -50); //decrease the brightness by 75 units
            capturedFrame.copyTo(ff);
            emit frameCaptured(ff);
        }
    }
    else
    {
        this->quit();
        emit NULL;
    }

}
//***************************************
void CaptureThread::captureFrame1()
{
    if (videoBuffer)
    {
        // CV_8UC3 = 8 bits, 3 chanels
        cv::Mat frame = cv::Mat(rtspReso, CV_8UC3, videoBuffer);
        if(frame.empty())
        {
            qDebug() << "frame is empty";
            emit NULL;
        }
        cv::Mat ff;
        frame.copyTo(ff);
        emit frameCaptured(ff);
    }

    else
    {
        this->quit();
        emit NULL;
    }

}
//***************************************
void CaptureThread::release_ip()
{
    qDebug() << "release Ip camera";
    libvlc_media_player_stop(mp);
    libvlc_media_player_release(mp);
    libvlc_release(inst);
}
//***************************************
void CaptureThread::pause_ip()
{
    if(!pause)
    {
        qDebug() << "Pause Ip camera";
        libvlc_media_player_pause(mp);
        pause=true;
    }
    else
    {
        qDebug() << "Play Ip camera";
        libvlc_media_player_play(mp);
        pause=false;
    }
}
