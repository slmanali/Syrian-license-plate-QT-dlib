#ifndef CAPTURETHREAD_H
#define CAPTURETHREAD_H

#include <QMetaType>
#include <QMutex>
#include <QString>
#include <QThread>
#include <QWaitCondition>
#include <opencv2/core.hpp>
#include <string>

Q_DECLARE_METATYPE(cv::Mat)

// One instance owns one capture session. Connect signals before calling start().
class CaptureThread : public QThread
{
    Q_OBJECT

public:
    explicit CaptureThread(QObject *parent = nullptr);
    explicit CaptureThread(int cameraIndex, QObject *parent = nullptr);
    explicit CaptureThread(const std::string &videoPath, QObject *parent = nullptr);
    CaptureThread(const QString &streamUrl, cv::Size resolution, QObject *parent = nullptr);
    ~CaptureThread() override;

    // These methods update synchronized state; the worker owns the backends.
    void requestStop();
    void setPaused(bool paused);
    void togglePause();
    bool isPaused() const;
    cv::Size frameSize() const;
    QString errorString() const;

signals:
    void frameCaptured(cv::Mat frame);
    void captureError(QString message);

protected:
    void run() override;

private:
    enum class Source { Camera, VideoFile, Stream };
    void runVideo();
    void runStream();
    bool waitForVideo();
    bool waitForNextFrame(unsigned long milliseconds);
    void publishFrame(const cv::Mat &frame);
    void reportError(const QString &message);

    Source m_source;
    int m_cameraIndex = 0;
    std::string m_videoPath;
    QString m_streamUrl;
    cv::Size m_resolution;

    mutable QMutex m_mutex;
    QWaitCondition m_stateChanged;
    bool m_paused = false;
    cv::Size m_frameSize;
    QString m_error;
};

#endif // CAPTURETHREAD_H
