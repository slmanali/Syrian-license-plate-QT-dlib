#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QtCore>
#include <QPainter>
#include <fstream>
#include <QMessageBox>
#include <QInputDialog>
#include <QFile>
#include <QTextStream>

#include <QtSerialPort/QSerialPort>
#include<QtSerialPort/QSerialPortInfo>
#include <QMessageBox>
#include <QTimer>

#include "VideoSortDetector.h"
#include "Utils.h"

#include <opencv/highgui.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core_c.h>
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/imgcodecs/imgcodecs_c.h>
#include <opencv2/videoio/videoio_c.h>

#include <dlib/svm_threaded.h>
#include <dlib/gui_widgets.h>
#include <dlib/image_processing.h>
#include <dlib/data_io.h>
#include "dlib/opencv.h"
#include <dlib/image_io.h>
#include <dlib/dir_nav.h>
#include <dlib/image_processing/object_detector.h>
#include <dlib/image_processing/full_object_detection.h>
#include <dlib/dnn.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>
#include <dlib/dnn.h>

#include <capturethread.h>
#include <QFormLayout>
#include <QtSql>
#include "form.h"
#include "ip_camera.h"

#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>

#include "httprequestworker.h"
#include <QJsonObject>
#include <QShortcut>
#include <QTimer>
#include "qjsontablemodel.h"

using namespace dlib;
using namespace std;
using namespace cv;
using namespace easytracker;

typedef scan_fhog_pyramid<pyramid_down<6>> image_scanner_type;

template <long num_filters, typename SUBNET> using con5d = con<num_filters, 5, 5, 2, 2, SUBNET>;
template <long num_filters, typename SUBNET> using con3 = con<num_filters, 3, 3, 1, 1, SUBNET>;
template <typename SUBNET> using downsampler = relu<bn_con<con5d<32, relu<bn_con<con5d<32, relu<bn_con<con5d<32, SUBNET>>>>>>>>>;
template <typename SUBNET> using rcon3 = relu<bn_con<con3<32, SUBNET>>>;
using net_type = loss_mmod<con<1, 6, 6, 1, 1, rcon3<rcon3<rcon3<downsampler<input_rgb_image_pyramid<pyramid_down<6>>>>>>>>;
template <long num_filters, typename SUBNET> using con5d = con<num_filters,5,5,2,2,SUBNET>;
template <long num_filters, typename SUBNET> using con5  = con<num_filters,5,5,1,1,SUBNET>;
template <typename SUBNET> using downsampler1  = relu<affine<con5d<32, relu<affine<con5d<32, relu<affine<con5d<16,SUBNET>>>>>>>>>;
template <typename SUBNET> using rcon5  = relu<affine<con5<55,SUBNET>>>;
using net_type1 = loss_mmod<con<1,9,9,1,1,rcon5<rcon5<rcon5<downsampler1<input_rgb_image_pyramid<pyramid_down<6>>>>>>>>;
//***************************************
struct plate{
    string name;
    string svm_path;
    rgb_pixel color;
    plate(string name, string svm_path, rgb_pixel color) :
        name(name), svm_path(svm_path), color(color) {}
};
//***************************************
struct digit{
    string name;
    string svm_path;
    rgb_pixel color;
    digit(string name, string svm_path, rgb_pixel color) :
        name(name), svm_path(svm_path), color(color) {}
};
//***************************************
struct region{
    string name;
    string svm_path;
    rgb_pixel color;
    region(string name, string svm_path, rgb_pixel color) :
        name(name), svm_path(svm_path), color(color) {}
};
//***************************************
struct numberRectangle{
    int index;
    double std_val;
    numberRectangle(int a, double b) :
        index(a), std_val(b) {}
};


namespace Ui {
class MainWindow;

}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    std::vector<plate> plates;
    std::vector<object_detector<image_scanner_type> > detectors_plate;
    std::vector<digit> digits;
    std::vector<object_detector<image_scanner_type> > detectors_number;
    std::vector<digit> digitsdnn;
    std::vector<net_type> detectors_numberdnn;
//    std::vector<shape_predictor> shape_predictor_dnn;
    std::vector<region> regions;
    std::vector<object_detector<image_scanner_type> > detectors_region;
    std::vector<cv::Scalar> number_color;
    Mat tmp;
    Mat tmp_train;
    QString fileName;
    QString home_folder;
    QString city_train;
    bool recognize;
    bool recognize1;
    std::vector<cv::Mat> res;
    std::vector<cv::Mat> res1;
    std::vector<cv::Mat> res2;
    int frame_count;
    int frame_count1;
    int frame_count2;
    int frame_proc;
    cv::Mat plate_img;
    bool video_proc;
    bool video_proc1;
    bool video_proc2;
    Form *new_plate;
    string plate_region;
    string plate_digits;
    QSqlDatabase m_db;
    QSqlDatabase s_db;
    QSqlQuery qry;
    QSqlQuery s_qry;
    int id_del;
    QString image_value;
    QString image_value_all;
    bool check_all;
    bool ip;
    bool ip1;
    bool ip2;
    int ip_num;
    cv::VideoWriter* video_rec;
    bool record;
    bool Trigger;
    int frame_rec;
    VideoSortDetector* videoSortDetecotDemo;
    double freq;
    cv::Mat detected_car;
    bool first_detect;
    bool interval_proc;

    net_type1 net;
    shape_predictor sp;

    QString city_id="";
    QString numb_id="";
    QString web_service_ip="";

    bool plate_Ent=false;
    int time_elpased=0;
public slots:
    void init_plate(std::vector<plate> *plates, std::vector<object_detector<image_scanner_type> > *detectors);
    void init_number(std::vector<digit> *digits, std::vector<object_detector<image_scanner_type> > *detectors);
    void init_numberdnn(std::vector<digit> *digits, std::vector<net_type> *detectors/*,std::vector<shape_predictor> *shape_predictor_dnn*/);
    void init_region(std::vector<region> *regions, std::vector<object_detector<image_scanner_type> > *detectors);
    cv::Rect dlib_rect_detectionToOpenCV(dlib::rect_detection r);
    cv::Rect dlibRectangleToOpenCV(dlib::rectangle r);
    string recognize_region(cv::Mat cvimg, std::vector<object_detector<image_scanner_type> > detectors, std::vector<region> regions,double& conf,cv::Rect& bb,cv::Scalar& colorr,int& kk);
    string recognize_plate(cv::Mat cvimg, std::vector<object_detector<image_scanner_type> > detectors, std::vector<digit> digits, std::vector<cv::Rect>& rectss,double& all_conf);
    string recognize_platednn(cv::Mat cvimg, std::vector<net_type> detectors/*,std::vector<shape_predictor> shape_predictor_dnn*/, std::vector<digit> digits, std::vector<cv::Rect>& rectss,double& all_conf);
    dlib::rectangle openCVRectToDlib(cv::Rect r);
    std::vector<cv::Mat>  image_recognize(cv::Mat pic);
    void  image_recognize_v1(cv::Mat);
    void  image_recognize_v2(cv::Mat);
    void  image_recognize_v3(cv::Mat src11);
    void  camera_show(cv::Mat);
    void camera_show1(cv::Mat);
    void camera_show2(cv::Mat);
    void fini();
    void name_port();
    void int_finish();
    void readrec();
    void done();
    void fini1();
    void done1();
    void fini2();
    void done2();
    //double angle(cv::Point pt1, cv::Point pt2, cv::Point pt0);
    //void detect_edges(cv::Mat src,cv::Mat& image0,cv::Mat& image1,cv::Mat& image2,cv::Mat& image3);
    void extract_contours(cv::Mat& image0,cv::Mat& image1,cv::Mat& image2,cv::Mat& image3,std::vector<std::vector<cv::Point> >& fin);
    std::vector<cv::Mat> affine_transform(cv::Mat src,std::vector<std::vector<cv::Point> >& fin);
    bool find_cars_dnn(cv::Mat src);
    void calculateSD(std::vector<float> data,float& mean,float& standardDeviation);
    void handle_result(HttpRequestWorker *worker);

private slots:

    void on_open_v1_clicked();

    void on_stop_v1_clicked();

    void on_pause_v1_clicked();

    void on_open_ip_camera1_clicked();

    void on_record_camera_1_clicked();

    void on_open_v2_clicked();

    void on_pause_v2_clicked();

    void on_stop_v2_clicked();

    void on_open_ip_camera2_clicked();

    void on_open_v3_clicked();

    void on_pause_v3_clicked();

    void on_stop_v3_clicked();

    void on_open_ip_camera3_clicked();

    void on_Trigger_bt_clicked();

    void on_tableView_clicked(const QModelIndex &index);

    void on_Serial_bT_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_checkBox_8_clicked(bool checked);

    void on_checkBox_9_clicked(bool checked);

    void on_checkBox_7_clicked(bool checked);

    void on_time_interval_check_clicked(bool checked);

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    cv::Mat matOriginal;
    QImage qimgOriginal;
    void stopCapture(CaptureThread *&capture);
    CaptureThread* cpThread = nullptr;
    CaptureThread* cpThread1 = nullptr;
    CaptureThread* cpThread2 = nullptr;
    QSerialPort* ser;
    QTimer* nam_tmr;
    QTimer* ser_tmr;
    QTimer* int_tmr = nullptr;
    bool obstacle;
    bool prev_obstacle;
    int num_obs;
    int counterr;
    int sub_counter;
    int img_num;


};

#endif // MAINWINDOW_H
