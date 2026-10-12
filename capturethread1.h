#ifndef CAPTURETHREAD1_H
#define CAPTURETHREAD1_H


#include <QThread>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/core/core_c.h>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/imgcodecs/imgcodecs_c.h>
#include <opencv2/video/video.hpp>
#include <opencv2/videoio/videoio_c.h>
#include <QtCore>

#include <QMessageBox>
#include <QTimer>

class CaptureThread1 : public QThread
{
    Q_OBJECT

public:
    explicit CaptureThread1(QObject *parent);
    explicit CaptureThread1(QString,cv::Size);
    explicit CaptureThread1(std::string);
    explicit CaptureThread1(cv::Mat);
    QTimer* tmrTimer;
    cv::Mat src;    
    bool stop;
    cv::Size rtspReso;
    void release_ip();
    void pause_ip();
    cv::VideoCapture capWebam;


protected:
    void run();
signals:
    void frameCaptured(cv::Mat);


public slots:
    void captureFrame();
    void captureFrame1();

private:
    cv::Mat capturedFrame;
};

#endif // CAPTURETHREAD1_H
