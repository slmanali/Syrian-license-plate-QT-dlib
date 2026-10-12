#include "capturethread1.h"
#include <QDebug>
Q_DECLARE_METATYPE(cv::Mat)
//***************************************
#define RTSP
#ifdef RTSP
#include <vlc/vlc.h>
libvlc_instance_t *inst1;
// RTSP address
//char* rtspAddress1 = "rtsp://admin:12345@196.10.10.155:554";
// Video resolution WxH
//cv::Size rtspRes(528, 384);

struct VideoDataStruct1
{
    int param;
};
int done1 = 0;
bool pause1=false;
libvlc_media_player_t *mp1;
unsigned int videoBufferSize1 = 0;
uint8_t *videoBuffer1 = 0;
//***************************************
void cbVideoPrerender1(void *p_video_data, uint8_t **pp_pixel_buffer, int size)
{
    if (size > videoBufferSize1 || !videoBuffer1)
    {
        printf("Reallocate raw video buffer %d bytes\n", size);
        free(videoBuffer1);
        videoBuffer1 = (uint8_t *)malloc(size);
        videoBufferSize1 = size;
    }

    // videoBuffer = (uint8_t *)malloc(size);
    *pp_pixel_buffer = videoBuffer1;
}
//***************************************
void cbVideoPostrender1(void *p_video_data, uint8_t *p_pixel_buffer, int width, int height, int pixel_pitch, int size, int64_t pts)
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
        time = libvlc_media_player_get_time(mp1);
        printf("MediaPlayerTimeChanged %lld ms\n", (long long)time);
        break;
    case libvlc_MediaPlayerEndReached:
        printf("MediaPlayerEndReached\n");
        done1 = 1;
        break;
    default:
        printf("%s\n", libvlc_event_type_name(pEvt->type));
    }
}
#endif
//***************************************
CaptureThread1::CaptureThread1(QObject* parent):QThread(parent)
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
CaptureThread1::CaptureThread1(std::string video_path)
{
    capWebam.open(video_path);
    if(!capWebam.isOpened())
    {
        qDebug() << "can't open";
        return;
    }
    QThread* t = new QThread();
//    qDebug() << capWebam.get(CV_CAP_PROP_FRAME_COUNT);
    moveToThread(t);
    t->start();
    tmrTimer = new QTimer;

    qRegisterMetaType< cv::Mat >("cv::Mat");
    QObject::connect(tmrTimer, SIGNAL(timeout()),this, SLOT(captureFrame()));
//    connect(this, SIGNAL(finished()), t, SLOT(quit()));
    tmrTimer->start(40);
}
//***************************************
CaptureThread1::CaptureThread1(QString video_path,cv::Size  rtspRes)
{
#ifdef RTSP
    // VLC pointers
    libvlc_media_t *m;
    void *pUserData = 0;

    VideoDataStruct1 dataStruct;

    // VLC options
    char smem_options[1000];

    // RV24
    sprintf_s(smem_options
        , "#transcode{vcodec=RV24}:smem{"
        "video-prerender-callback=%lld,"
        "video-postrender-callback=%lld,"
        "video-data=%lld,"
        "no-time-sync},"
        , (long long int)(intptr_t)(void*)&cbVideoPrerender1
        , (long long int)(intptr_t)(void*)&cbVideoPostrender1
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
    inst1 = libvlc_new(sizeof(vlc_args) / sizeof(vlc_args[0]), vlc_args);
    rtspReso= rtspRes;
    // Create a new item
    m = libvlc_media_new_location(inst1, video_path.toStdString().c_str());

    // Create a media player playing environement
    mp1 = libvlc_media_player_new_from_media(m);

    libvlc_event_manager_t* eventManager = libvlc_media_player_event_manager(mp1);
    libvlc_event_attach(eventManager, libvlc_MediaPlayerTimeChanged, handleEvent, pUserData);
    libvlc_event_attach(eventManager, libvlc_MediaPlayerEndReached, handleEvent, pUserData);
    libvlc_event_attach(eventManager, libvlc_MediaPlayerPositionChanged, handleEvent, pUserData);

    //libvlc_video_set_format(mp, "RV24", 240, 320, 240 * 3 );

    // play the media_player
    libvlc_media_player_play(mp1);
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
CaptureThread1::CaptureThread1(cv::Mat ttmp)
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
void CaptureThread1::run()
{
//    QObject::connect(tmrTimer, SIGNAL(timeout()), this, SLOT(captureFrame()));

    exec();
}
//***************************************
int mmm1=10;
int sss1=0;
//***************************************
void CaptureThread1::captureFrame()
{
    if(capWebam.isOpened()&& (capWebam.get(CV_CAP_PROP_POS_FRAMES)<capWebam.get(CV_CAP_PROP_FRAME_COUNT)) && sss1<10)
    {
        if(mmm1==capWebam.get(CV_CAP_PROP_POS_FRAMES))
            sss1++;
        else
        {
            mmm1=capWebam.get(CV_CAP_PROP_POS_FRAMES);
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
void CaptureThread1::captureFrame1()
{
    if (videoBuffer1)
    {
        // CV_8UC3 = 8 bits, 3 chanels
        cv::Mat frame = cv::Mat(rtspReso, CV_8UC3, videoBuffer1);
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
void CaptureThread1::release_ip()
{
    qDebug() << "release Ip camera";
    libvlc_media_player_stop(mp1);
    libvlc_media_player_release(mp1);
    libvlc_release(inst1);
}
//***************************************
void CaptureThread1::pause_ip()
{
    if(!pause1)
    {
        qDebug() << "Pause Ip camera";
        libvlc_media_player_pause(mp1);
        pause1=true;
    }
    else
    {
        qDebug() << "Play Ip camera";
        libvlc_media_player_play(mp1);
        pause1=false;
    }
}
