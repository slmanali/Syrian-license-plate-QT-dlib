#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <limits>
#include <numeric>
#include <thread>
#include <QDesktopWidget>
#include <QDebug>
//***************************************
#define kernel_size 3
#define Ratio 3
#define lowThresh 50
#define minContourArea 50
#define minCos -0.4
#define maxCos 0.4
#define minAspectRatio 0.5//1 //exp 1.6
#define maxAspectRatio 10 //5.4 //exp 7.6
#define minimageRatio 0.004 //new
#define maximageRatio 0.08	 //new
std::vector<cv::Mat> image_listt;
typedef std::numeric_limits< double > dbl;
QString Cities[14]={"Damascus","Deraa","Sub Damascus","Homs","Hama","Latakia","Tartous","Aleppo"
                    ,"Al-Raqqah","Deir ez-Zor","Al-Hasakah","Al-Suwayda","Idlib","Quneitra"};
QString Cities_ar[14]={"دمشق","درعا","ريف دمشق","حمص","حماة","اللاذقية",
                       "طرطوس","حلب","الرقة","دير الزور","الحسكة","السويداء","ادلب","القنيطرة"};
QString Cities_ID[14]={"11","15","12","31","33","41","43","21","22","51","52","16","23","14"};
//***************************************
bool myfunction0(cv::Mat im0, cv::Mat im1) { return ((im0.cols*im0.rows)>(im1.cols*im1.rows)); }
bool myfunction(rect_detection rect1, rect_detection rect2) { return (rect1.rect.left()<rect2.rect.left()); }
bool myfunction1(rect_detection rect1, rect_detection rect2) { return (rect1.detection_confidence>rect2.detection_confidence); }
bool myfunction2(rect_detection rect1, rect_detection rect2) { return (rect1.rect.area()>rect2.rect.area()); }
bool myfunction3(dlib::rectangle rect1, dlib::rectangle rect2) { return (rect1.area()>rect2.area()); }
bool myfunction4(double c0, double c1) { return (c0>c1); }
bool myfunction5(numberRectangle c0, numberRectangle c1) { return (std::abs(c0.std_val)<std::abs(c1.std_val)); }
int l_cit=0;
int l_cit1=0;
int l_cit2=0;
//***************************************
MainWindow::MainWindow(QWidget *parent) :    QMainWindow(parent),    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    home_folder=QCoreApplication::applicationDirPath();
    QString line="";
    QString line1="";
    QString line2="";
    QFile inputFile(home_folder+"/type.txt");
    if (inputFile.open(QIODevice::ReadOnly))
    {
       QTextStream in(&inputFile);
//       while (!in.atEnd())
       {
           line=in.readLine();
           line1=in.readLine();
           line2=in.readLine();
       }
       inputFile.close();
    }
    if(line!="yes")
    {
        ui->additional_Details->setEnabled(false);
        ui->additional_Details->setVisible(false);
    }
    else
        web_service_ip=line1;
    time_elpased=line2.toInt();
    init_plate(&plates, &detectors_plate);
    init_number(&digits, &detectors_number);
    init_numberdnn(&digitsdnn, &detectors_numberdnn/*, &shape_predictor_dnn*/);
    init_region(&regions,&detectors_region);
    dlib::deserialize(home_folder.toStdString()+"\\mmod_front_and_rear_end_vehicle_detector.dat") >> net >> sp;

    QString fileName1=home_folder+"/Cars.sqlite";
    m_db=QSqlDatabase::addDatabase( "QSQLITE" );
    m_db.setDatabaseName(fileName1.toUtf8() );
    if( !m_db.open() )
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::information(this, "خطأ", "قاعدة المعطيات",
                                         QMessageBox::Ok);
    }
    qDebug( "Cars Connected!" );
    qry = QSqlQuery( m_db );

    if(!QDir(home_folder+"\\database").exists())
        QDir().mkdir(home_folder+"\\database");

    nam_tmr=new QTimer(this);
    ser_tmr=new QTimer(this);

    ser=new QSerialPort(this);

    Q_FOREACH(QSerialPortInfo port, QSerialPortInfo::availablePorts())
        ui->Por_name->addItem(port.portName());
    connect(nam_tmr,SIGNAL(timeout()),this,SLOT(name_port()));
    nam_tmr->start(15000);
//    ui->widget->setEnabled(false);
//    ui->widget_2->setEnabled(false);
    recognize=false;
    recognize1=false;
    video_proc=false;
    video_proc1=false;
    video_proc2=false;

    ip=false;
    ip1=false;
    ip2=false;

    std::cout.precision(dbl::max_digits10);

    ui->stop_v1->setEnabled(false);
    ui->pause_v1->setEnabled(false);
//    ui->record_camera_1->setEnabled(false);
    record=false;

    ui->stop_v2->setEnabled(false);
    ui->pause_v2->setEnabled(false);
    ui->record_camera_2->setEnabled(false);

    ui->stop_v3->setEnabled(false);
    ui->pause_v3->setEnabled(false);
    ui->record_camera_3->setEnabled(false);
    first_detect=false;
    id_del=0;
    check_all=false;
    Trigger=false;
}
//***************************************
MainWindow::~MainWindow()
{
    if(nam_tmr->isActive())
        nam_tmr->stop();

    if(ser->isOpen())
    {
        ser->close();
        disconnect(ser_tmr, SIGNAL(timeout()), this, SLOT(readrec()));
    }
    if(!tmp.empty())
        tmp.release();

    if(ip)
        cpThread->release_ip();

    if(ip1)
        cpThread1->release_ip();

    if(ip2)
        cpThread2->release_ip();

    if (video_proc)
        cpThread->stop=true;
    if (video_proc1)
        cpThread1->stop=true;
    if (video_proc2)
        cpThread2->stop=true;

    m_db.close();

    if(!m_db.isOpen())
        qDebug( "Closed!" );
    delete ui;
}
//***************************************
cv::Rect MainWindow::dlib_rect_detectionToOpenCV(dlib::rect_detection r)
{
    return cv::Rect(cv::Point2i(r.rect.left(), r.rect.top()), cv::Point2i(r.rect.right() + 1, r.rect.bottom() + 1));
}
//***************************************
cv::Rect MainWindow::dlibRectangleToOpenCV(dlib::rectangle r)
{
    return cv::Rect(cv::Point2i(r.left(), r.top()), cv::Point2i(r.right() + 1, r.bottom() + 1));
}
//***************************************
void MainWindow::init_plate(std::vector<plate> *plates, std::vector<object_detector<image_scanner_type> > *detectors)
{
    // Load SVM detectors
    plates->push_back(plate("rectangle", home_folder.toStdString()+"\\1.svm",
        rgb_pixel(255, 0, 0)));
//    plates->push_back(plate("rectangle_2", home_folder.toStdString()+"\\rectangle_0.svm",
//        rgb_pixel(255, 0, 122)));
//    plates->push_back(plate("rectangle_3", home_folder.toStdString()+"\\rectangle_2.svm",
//        rgb_pixel(255, 255, 0)));
    plates->push_back(plate("square", home_folder.toStdString()+"\\6.svm",
        rgb_pixel(255, 122, 0)));
    for (unsigned long i = 0; i < plates->size(); i++) {
        object_detector<image_scanner_type> detector;
        deserialize(plates->at(i).svm_path) >> detector;
//        object_detector::feature_vector_type ii= detector.get_w().;
        detectors->push_back(detector);
    }
}
//***************************************
void MainWindow::init_number(std::vector<digit> *digits, std::vector<object_detector<image_scanner_type> > *detectors)
{
    // Load SVM detectors
//    digits->push_back(digit("0", home_folder.toStdString()+"\\object_detector_0.svm",
//        rgb_pixel(255, 0, 0)));
//    digits->push_back(digit("1", home_folder.toStdString()+"\\object_detector_1.svm",
//        rgb_pixel(255, 122, 0)));
//    digits->push_back(digit("2", home_folder.toStdString()+"\\object_detector_2.svm",
//        rgb_pixel(255, 255, 0)));
//    digits->push_back(digit("3", home_folder.toStdString()+"\\object_detector_3.svm",
//        rgb_pixel(255, 0, 122)));
//    digits->push_back(digit("4", home_folder.toStdString()+"\\object_detector_4.svm",
//        rgb_pixel(255, 122, 255)));
//    digits->push_back(digit("5",home_folder.toStdString()+"\\object_detector_5.svm",
//        rgb_pixel(0, 255, 255)));
//    digits->push_back(digit("6",home_folder.toStdString()+"\\object_detector_6.svm",
//        rgb_pixel(255, 122, 122)));
//    digits->push_back(digit("7", home_folder.toStdString()+"\\object_detector_7.svm",
//        rgb_pixel(122, 122, 0)));
//    digits->push_back(digit("8",home_folder.toStdString()+"\\object_detector_8.svm",
//        rgb_pixel(255, 255, 255)));
//    digits->push_back(digit("9", home_folder.toStdString()+"\\object_detector_9.svm",
//        rgb_pixel(255, 122, 122)));
    digits->push_back(digit("0", home_folder.toStdString()+"\\svm_detectors\\object_detector_0.svm",
        rgb_pixel(255, 0, 0)));
    digits->push_back(digit("1", home_folder.toStdString()+"\\svm_detectors\\object_detector_1.svm",
        rgb_pixel(255, 122, 0)));
    digits->push_back(digit("2", home_folder.toStdString()+"\\svm_detectors\\object_detector_2.svm",
        rgb_pixel(255, 255, 0)));
    digits->push_back(digit("3", home_folder.toStdString()+"\\svm_detectors\\object_detector_3.svm",
        rgb_pixel(255, 0, 122)));
    digits->push_back(digit("4", home_folder.toStdString()+"\\svm_detectors\\object_detector_4.svm",
        rgb_pixel(255, 122, 255)));
    digits->push_back(digit("5",home_folder.toStdString()+"\\svm_detectors\\object_detector_5.svm",
        rgb_pixel(0, 255, 255)));
    digits->push_back(digit("6",home_folder.toStdString()+"\\svm_detectors\\object_detector_6.svm",
        rgb_pixel(255, 122, 122)));
    digits->push_back(digit("7", home_folder.toStdString()+"\\svm_detectors\\object_detector_7.svm",
        rgb_pixel(122, 122, 0)));
    digits->push_back(digit("8",home_folder.toStdString()+"\\svm_detectors\\object_detector_8.svm",
        rgb_pixel(255, 255, 255)));
    digits->push_back(digit("9", home_folder.toStdString()+"\\svm_detectors\\object_detector_9.svm",
        rgb_pixel(255, 122, 122)));
    for (unsigned long i = 0; i < digits->size(); i++) {
        object_detector<image_scanner_type> detector;
        deserialize(digits->at(i).svm_path) >> detector;
        detectors->push_back(detector);
    }
}
//***************************************
void MainWindow::init_numberdnn(std::vector<digit> *digits, std::vector<net_type> *detectors
                                /*,std::vector<shape_predictor> *shape_predictor_dnn*/)
{
    // Load SVM detectors
    digits->push_back(digit("0", home_folder.toStdString()+"\\object_detector_0.dat",
        rgb_pixel(255, 0, 0)));
    digits->push_back(digit("1", home_folder.toStdString()+"\\object_detector_1.dat",
        rgb_pixel(255, 122, 0)));
    digits->push_back(digit("2", home_folder.toStdString()+"\\object_detector_2.dat",
        rgb_pixel(255, 255, 0)));
    digits->push_back(digit("3", home_folder.toStdString()+"\\object_detector_3.dat",
        rgb_pixel(255, 0, 122)));
    digits->push_back(digit("4", home_folder.toStdString()+"\\object_detector_4.dat",
        rgb_pixel(255, 122, 255)));
    digits->push_back(digit("5",home_folder.toStdString()+"\\object_detector_5.dat",
        rgb_pixel(0, 255, 255)));
    digits->push_back(digit("6",home_folder.toStdString()+"\\object_detector_6.dat",
        rgb_pixel(255, 122, 122)));
    digits->push_back(digit("7", home_folder.toStdString()+"\\object_detector_7.dat",
        rgb_pixel(122, 122, 0)));
    digits->push_back(digit("8",home_folder.toStdString()+"\\object_detector_8.dat",
        rgb_pixel(255, 255, 255)));
    digits->push_back(digit("9", home_folder.toStdString()+"\\object_detector_9.dat",
        rgb_pixel(255, 122, 122)));
    for (unsigned long i = 0; i < digits->size(); i++) {
        net_type detector;
//        shape_predictor sp;
        deserialize(digits->at(i).svm_path) >> detector /*>>sp*/;
        detectors->push_back(detector);
//        shape_predictor_dnn->push_back(sp);
    }
}
//***************************************
void MainWindow::init_region(std::vector<region> *regions, std::vector<object_detector<image_scanner_type> > *detectors)
{
    // Load SVM detectors
    regions->push_back(region("Damascus",home_folder.toStdString()+"\\object_detector_damascus.svm",
        rgb_pixel(255, 0, 0)));
    regions->push_back(region("Sub Damascus",home_folder.toStdString()+"\\object_detector_sub.svm",
        rgb_pixel(0, 255, 0)));
    regions->push_back(region("Deraa",home_folder.toStdString()+"\\object_detector_deraa.svm",
        rgb_pixel(255, 255, 0)));

    for (unsigned long i = 0; i < regions->size(); i++) {
        object_detector<image_scanner_type> detector;
        deserialize(regions->at(i).svm_path) >> detector;
        detectors->push_back(detector);
    }
}
//***************************************
string MainWindow::recognize_region(cv::Mat cvimg, std::vector<object_detector<image_scanner_type> > detectors,
                                    std::vector<region> regions,double& conf,cv::Rect& bound,cv::Scalar& city,int &l)
{
    std::vector<rect_detection> rects;
    cv::Rect cut_rect;
    if((double)cvimg.cols/cvimg.rows>2.0)
    {
        int we=cvimg.cols*0.28;
        cv::Rect temp(0,0,we,cvimg.rows);
        cut_rect=temp;
    }
    else
    {
        int px=cvimg.cols*0.5;
        int py=cvimg.rows*0.6;
        cv::Rect temp(px,py,cvimg.cols-px,cvimg.rows-py);
        cut_rect=temp;
    }
    cv::Mat cutimg=cvimg(cut_rect);
//    cv_image<unsigned char> img(cvimg);
    cv_image<unsigned char> img(cutimg);
    auto start = std::chrono::high_resolution_clock::now();
    evaluate_detectors(detectors, img, rects,-0.2);//-0.2
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
//    std::cout<<"region "<<elapsed.count()<<std::endl;
    ui->lineEdit_6->setText(QString::number(elapsed.count()));
    string region;
    if(rects.empty())
    {
        region = "none";
        conf=0.0;
        return region;
    }
    std::sort(rects.begin(), rects.end(), myfunction1);
    if (rects.size() > 0)
    {
        if(rects[0].detection_confidence>0.1)
        {
            region = regions[rects[0].weight_index].name;
            for(l=0;l<14;l++)
            {
                if(QString::fromStdString(region)==Cities[l])
                {
                    region=Cities_ar[l].toStdString();
                    break;
                }
            }
            conf=rects[0].detection_confidence;
            bound=dlib_rect_detectionToOpenCV(rects[0]);
            city=cv::Scalar(digits[rects[0].weight_index].color.red,digits[rects[0].weight_index].color.green,
                    digits[rects[0].weight_index].color.blue);
        }
        else
        {
            region = "none";
            conf=rects[0].detection_confidence;
            city=cv::Scalar(255,255,255);
        }
    }
//    else
//    {
//        std::sort(rects.begin(), rects.end(), myfunction2);
//        if (rects.size() > 0)
//        {
//            if(rects[0].detection_confidence>0)
//            {
//                region = regions[rects[0].weight_index].name;
//                for(l=0;l<14;l++)
//                {
//                    if(QString::fromStdString(region)==Cities[l])
//                    {
//                        region=Cities_ar[l].toStdString();
//                        break;
//                    }
//                }
//                conf=rects[0].detection_confidence;
//                bound=dlib_rect_detectionToOpenCV(rects[0]);
//                city=cv::Scalar(digits[rects[0].weight_index].color.red,digits[rects[0].weight_index].color.green,
//                        digits[rects[0].weight_index].color.blue);
//            }
//            else
//            {
//                region = "none";
//                conf=rects[0].detection_confidence;
//                city=cv::Scalar(255,255,255);
//            }
//        }
//        else
//        {
//            region = "none";
//            city=cv::Scalar(255,255,255);
//            conf=rects[0].detection_confidence;
//        }
//    }
    return region;

}
//***************************************
void MainWindow::calculateSD(std::vector<float> data,float& mean,float& standardDeviation)
{
    float sum = 0.0;

    int i;

    for(i = 0; i < data.size(); ++i)
    {
        sum += data[i];
    }

    mean = sum/data.size();

    for(i = 0; i < data.size(); ++i)
        standardDeviation += pow(std::abs(data[i] - mean), 2);
    standardDeviation=sqrt(standardDeviation / data.size());
//    std::cout<<"mean="<<mean<<" standardDeviation="<<standardDeviation<<std::endl;
}
//***************************************
string MainWindow::recognize_plate(cv::Mat cvimg, std::vector<object_detector<image_scanner_type> > detectors,
                                   std::vector<digit> digits, std::vector<cv::Rect>& rectss,double& conf)
{    
    std::vector<rect_detection> rects;
    number_color.clear();
    rectss.clear();
    cv::Rect cut_rect;
    std::vector<cv::Rect> cvrects;
    std::vector<float> dataY;
    std::vector<double> dataX;
    string plate;
    std::vector<double> conf_list;
    if((double)cvimg.cols/cvimg.rows>2.0)
    {
        int px=cvimg.cols*0.4;
        cv::Rect temp(px,0,cvimg.cols-px,cvimg.rows);
        cut_rect=temp;
    }
    else
    {
        int px=cvimg.cols*0.35;
        int he=cvimg.rows*0.6;
        cv::Rect temp(px,0,cvimg.cols-px,he);
        cut_rect=temp;
    }
    cv::Mat cutimg=cvimg(cut_rect);
    cv_image<unsigned char> img(cutimg);
    auto start = std::chrono::high_resolution_clock::now();
    evaluate_detectors(detectors, img, rects,0);//-0.12
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
//    std::cout<<"digits "<<elapsed.count()<<std::endl;
    ui->lineEdit_5->setText(QString::number(elapsed.count()));
    if(rects.empty())
        return "Unrecognized";
    std::sort(rects.begin(), rects.end(), myfunction);
    float mean=0.0, standardDeviation=0.0;
    int delete_number=rects.size()-6;
    if(delete_number>=1)
    {
        for(unsigned long j = 0; j < rects.size(); ++j)
        {
            cvrects.push_back(dlib_rect_detectionToOpenCV(rects[j]));
            dataY.push_back(cvrects[j].y+((float)cvrects[j].height/2.0));
            dataX.push_back(cvrects[j].x+((double)cvrects[j].width/2.0));
            std::cout<<digits[rects[j].weight_index].name<<" "<<cvrects[j].x<<" "<<cvrects[j].y<<" "<<
                                                           cvrects[j].width<<" "<<cvrects[j].height<<" "<<dataY[j]<<std::endl;
        }
        std::vector<numberRectangle> dell;
        for (unsigned long j = 0; j < rects.size()-1; ++j)
        {
            if(j!=rects.size()-1)
            {
                if((dataX[j]+cvrects[j].width/2)>dataX[j+1])
                {
                    if(rects[j].detection_confidence>rects[j+1].detection_confidence)
                    {
                        {
                            numberRectangle temp((int)j+1,(double)dataX[j]+cvrects[j].width/2-dataX[j+1]);
                            dell.push_back(temp);
                        }
                    }
                    else
                    {
                        {
                            numberRectangle temp((int)j,(double)dataX[j]+cvrects[j].width/2-dataX[j+1]);
                            dell.push_back(temp);
                        }
                    }
                }
            }
        }
        std::vector<rect_detection> rects_t;
        std::cout<<dell.size()<<std::endl;
        std::sort(dell.begin(), dell.end(), myfunction5);
        for (unsigned long i=0;i<dell.size();i++)
             std::cout<<dell[i].index<<" "<<dell[i].std_val<<std::endl;
        bool second_step=false;
        if(dell.size()>delete_number)
        {
            for (unsigned long j = 0; j < delete_number; ++j)
                rects[dell[dell.size()-j-1].index].detection_confidence=-100;
        }
        else
        {
            for (unsigned long i=0;i<dell.size();i++)
                 rects[dell[i].index].detection_confidence=-100;
            second_step=true;
            delete_number-=dell.size();
        }
        for(unsigned long j = 0; j <rects.size(); j++)
        {
            if(rects[j].detection_confidence!=-100)
                rects_t.push_back(rects[j]);
        }
        rects.clear();
        rects=rects_t;
        rects_t.clear();
        cvrects.clear();
        dataX.clear();
        dataY.clear();
        for(unsigned long j = 0; j < rects.size(); ++j)
        {
            cvrects.push_back(dlib_rect_detectionToOpenCV(rects[j]));
            dataX.push_back(cvrects[j].x+(cvrects[j].width/2));
            dataY.push_back(cvrects[j].y+(cvrects[j].height/2));
            std::cout<<digits[rects[j].weight_index].name<<" "<<cvrects[j].x<<" "<<cvrects[j].y<<" "<<
                                                           cvrects[j].width<<" "<<cvrects[j].height<<" "<<dataY[j]<<std::endl;
        }
        calculateSD(dataY,mean,standardDeviation);
        std::cout<<delete_number<<"--"<<mean-standardDeviation<<" "<<mean+standardDeviation<<std::endl;
        std::vector<numberRectangle> ns_dell;
        for (unsigned long i=0;i<dataY.size();i++)
        {
            if(dataY[i]<mean-standardDeviation)
            {
                numberRectangle temp(i,dataY[i]-mean+standardDeviation);
                ns_dell.push_back(temp);

            }
            if(dataY[i]>mean+standardDeviation)
            {
                numberRectangle temp(i,dataY[i]-mean-standardDeviation);
                ns_dell.push_back(temp);
            }
        }
        std::cout<<ns_dell.size()<<std::endl;
        std::sort(ns_dell.begin(), ns_dell.end(), myfunction5);
//        for (unsigned long i=0;i<ns_dell.size();i++)
//             std::cout<<ns_dell[i].index<<" "<<ns_dell[i].std_val<<std::endl;
        if(second_step)
        {
            if(ns_dell.size()>delete_number)
            {
                for (unsigned long i=0;i<delete_number;i++)
                {
                     rects.at(ns_dell[ns_dell.size()-i-1].index).detection_confidence=-100.0;
                }
            }
            else
            {
                for (unsigned long i=0;i<ns_dell.size();i++)
                {
                    rects.at(ns_dell[i].index).detection_confidence=-100.0;
                }
//                second_step=true;
//                delete_number-=ns_dell.size();
            }
        }
        for(unsigned long j = 0; j <rects.size(); j++)
        {
            if(rects[j].detection_confidence!=-100)
                rects_t.push_back(rects[j]);
        }

        rects.clear();
        rects=rects_t;
        rects_t.clear();
        cvrects.clear();
        dataX.clear();
        dataY.clear();
        for(unsigned long j = 0; j < rects.size(); ++j)
        {
            cvrects.push_back(dlib_rect_detectionToOpenCV(rects[j]));
            dataX.push_back(cvrects[j].x+(cvrects[j].width/2));
            dataY.push_back(cvrects[j].y+(cvrects[j].height/2));
            std::cout<<digits[rects[j].weight_index].name<<" "<<cvrects[j].x<<" "<<cvrects[j].y<<" "<<
                                                           cvrects[j].width<<" "<<cvrects[j].height<<" "<<dataY[j]<<std::endl;
        }
        for (unsigned long j = 0; j < rects.size(); ++j)
        {
            if((double)cvimg.cols/cvimg.rows>2.0)
            {
                if((rects[j].rect.left()+rects[j].rect.right())/2>2*cvimg.cols/5)
                {
                    if ( (digits[rects[j].weight_index].name != "") )
                      {
                           plate.append(digits[rects[j].weight_index].name);

                           rectss.push_back(dlib_rect_detectionToOpenCV(rects[j]));
                           number_color.push_back(cv::Scalar(digits[rects[j].weight_index].color.red,
                                                  digits[rects[j].weight_index].color.green,
                                                  digits[rects[j].weight_index].color.blue));

                      }
                }
            }
            else
            {
                if((rects[j].rect.left()+rects[j].rect.right())/2>cvimg.cols/3 &&
                        (rects[j].rect.top()+rects[j].rect.bottom())/2<2*cvimg.rows/3)
                {
                    if ( (digits[rects[j].weight_index].name != "") )
                    {
                       plate.append(digits[rects[j].weight_index].name);
                       rectss.push_back(dlib_rect_detectionToOpenCV(rects[j]));
                       number_color.push_back(cv::Scalar(digits[rects[j].weight_index].color.red,
                                              digits[rects[j].weight_index].color.green,
                                              digits[rects[j].weight_index].color.blue));
                    }
                }
            }

        }

        while(plate.size()>6)
        {
            double min=conf_list[0];
            int index=0;
            for (unsigned long j = 1; j < rectss.size(); ++j)
            {
                if(conf_list[j]<min)
                {
                    index=j;
                    min=conf_list[j];
                }
            }
            rectss.erase(rectss.begin()+index);
            plate.erase(plate.begin()+index);
            conf_list.erase(conf_list.begin()+index);
            number_color.erase(number_color.begin()+index);
        }
        if(conf_list.size()!=0)
        {
            auto mean = std::accumulate(std::begin(conf_list), std::end(conf_list), 0.0) / std::size(conf_list);
            conf=mean;
        }
    }
    else
    {
        for (unsigned long j = 0; j < rects.size(); ++j)
        {
            plate.append(digits[rects[j].weight_index].name);
            conf_list.push_back(rects[j].detection_confidence);
            rectss.push_back(dlib_rect_detectionToOpenCV(rects[j]));
            number_color.push_back(cv::Scalar(digits[rects[j].weight_index].color.red,
                                   digits[rects[j].weight_index].color.green,
                                   digits[rects[j].weight_index].color.blue));
        }
        if(conf_list.size()!=0)
        {
            auto mean = std::accumulate(std::begin(conf_list), std::end(conf_list), 0.0) / std::size(conf_list);
            conf=mean;
        }
    }
   return plate;
}
//***************************************
string MainWindow::recognize_platednn(cv::Mat cvimg, std::vector<net_type> detectors
                                      /*,std::vector<shape_predictor> shape_predictor_dnn*/,
                                      std::vector<digit> digits, std::vector<cv::Rect>& rectss,double& conf)
{
    matrix<rgb_pixel> img;
    cvtColor(cvimg,cvimg,CV_GRAY2RGB);
    cv_image<bgr_pixel> image(cvimg);
    assign_image(img, image);
    std::vector<rect_detection> rects;
    number_color.clear();
    rectss.clear();
    auto start = std::chrono::high_resolution_clock::now();
    for(int i=0;i<detectors.size()-1;i++)
    {
        for (auto&& d : detectors[i](img))
        {
            dlib::rect_detection rect;
            rect.detection_confidence=d.detection_confidence;
            rect.rect=d.rect;
            rect.weight_index=(unsigned long) i;//std::stoul (d.label,nullptr,0);
            rects.push_back(rect);
        }
    }
//    evaluate_detectors(detectors, img, rects,-0.12);//-0.12
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    std::cout<<"digits "<<elapsed.count()<<std::endl;
    ui->lineEdit_5->setText(QString::number(elapsed.count()));
    if(rects.empty())
        return "Unrecognized";
    //std::sort(rects.begin(), rects.end(), myfunction);
    std::vector<int> dell;
    for (unsigned long j = 0; j < rects.size()-1; ++j)
    {
        if(j!=rects.size()-1)
        {
            if(std::abs(rects[j].rect.left()-rects[j+1].rect.left())<5)
            {
                if(rects[j].detection_confidence>rects[j+1].detection_confidence)
                {
                    if(dell.empty())
                        dell.push_back(j+1);
                    else
                    {
                        if(std::find(dell.begin(), dell.end(), j+1) != dell.end())
                            dell.push_back(j+1);
                    }
                }
                else
                {
                    if(dell.empty())
                        dell.push_back(j);
                    else
                    {
                        if(std::find(dell.begin(), dell.end(), j) != dell.end())
                            dell.push_back(j);
                    }
                }
            }
        }
    }
    for (unsigned long j = 0; j < dell.size(); ++j)
        rects.erase(rects.begin()+dell[j]);
    string plate;
    std::vector<double> conf_list;
    for (unsigned long j = 0; j < rects.size(); ++j)
    {
        if((double)cvimg.cols/cvimg.rows>2.0)
        {
            if((rects[j].rect.left()+rects[j].rect.right())/2>2*cvimg.cols/5)
            {
                if ( (digits[rects[j].weight_index].name != "") )
                  {
                       plate.append(digits[rects[j].weight_index].name);
                       conf_list.push_back(rects[j].detection_confidence);
                       rectss.push_back(dlib_rect_detectionToOpenCV(rects[j]));
                       number_color.push_back(cv::Scalar(digits[rects[j].weight_index].color.red,
                                              digits[rects[j].weight_index].color.green,digits[rects[j].weight_index].color.blue));

                  }
            }
        }
        else
        {
            if((rects[j].rect.left()+rects[j].rect.right())/2>cvimg.cols/3
                    && (rects[j].rect.top()+rects[j].rect.bottom())/2<2*cvimg.rows/3)
            {
                if ( (digits[rects[j].weight_index].name != "") )
                {
//                  if(rects[j].detection_confidence>0.1)
                  {
                       plate.append(digits[rects[j].weight_index].name);
                       conf_list.push_back(rects[j].detection_confidence);
                       rectss.push_back(dlib_rect_detectionToOpenCV(rects[j]));
                       number_color.push_back(cv::Scalar(digits[rects[j].weight_index].color.red,
                                              digits[rects[j].weight_index].color.green,digits[rects[j].weight_index].color.blue));
                  }
                }
            }
        }

    }

    while(plate.size()>6)
    {
        double min=conf_list[0];
        int index=0;
        for (unsigned long j = 1; j < rectss.size(); ++j)
        {
            if(conf_list[j]<min)
            {
                index=j;
                min=conf_list[j];
            }
        }
        rectss.erase(rectss.begin()+index);
        plate.erase(plate.begin()+index);
        conf_list.erase(conf_list.begin()+index);
        number_color.erase(number_color.begin()+index);
    }
    if(plate.size()<=6)
    {
        for (unsigned long j = 0; j < rectss.size(); ++j)
        {
            if(conf_list[j]<0.1)
            {
                rectss.erase(rectss.begin()+(int)j);
                plate.erase(plate.begin()+(int)j);
                conf_list.erase(conf_list.begin()+(int)j);
                number_color.erase(number_color.begin()+(int)j);
            }
        }
    }
    if(conf_list.size()!=0)
    {
        auto mean = std::accumulate(std::begin(conf_list), std::end(conf_list), 0.0) / std::size(conf_list);
//        std::sort(conf_list.begin(), conf_list.end(), myfunction4);
//        if(conf_list.size()%2==0)
//            conf=conf_list[conf_list.size()/2];
//        else
//            conf=conf_list[(conf_list.size()+1)/2];
    }
   return plate;
}
//***************************************
dlib::rectangle MainWindow::openCVRectToDlib(cv::Rect r)
{
    return dlib::rectangle((long)r.tl().x, (long)r.tl().y, (long)r.br().x - 1, (long)r.br().y - 1);
}
//***************************************
double angle(cv::Point pt1, cv::Point pt2, cv::Point pt0)
{
    double dx1 = pt1.x - pt0.x;
    double dy1 = pt1.y - pt0.y;
    double dx2 = pt2.x - pt0.x;
    double dy2 = pt2.y - pt0.y;
    return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}
//***************************************
void detect_edges(cv::Mat src,cv::Mat& image0,cv::Mat& image1,cv::Mat& image2,cv::Mat& image3)
{
    Mat detected_edges[4];
    Mat chan[3];
    Mat src_gray;
    cvtColor( src, src_gray, CV_BGR2GRAY );
    split(src,chan);
    //reduce noise in grayscale with a 2x2 kernel
    adaptiveThreshold(src_gray,src_gray,255,ADAPTIVE_THRESH_GAUSSIAN_C,THRESH_BINARY,11,2);
        blur( src_gray, detected_edges[0], Size(2,2) );

    //reduce noise in all the channels with a 2x2 kernel
    for(int i=0;i<3;i++)
    {
        blur( chan[i], detected_edges[i+1], Size(2,2) );
    }
    // canny detector
    for(int i=0;i<4;i++)
    {
        int highThresh=lowThresh*Ratio;
        Canny( detected_edges[i], detected_edges[i], lowThresh, highThresh, kernel_size );
    }

    image0=detected_edges[0];
    image1=detected_edges[1];
    image2=detected_edges[2];
    image3=detected_edges[3];
}
//***************************************
void MainWindow::extract_contours(cv::Mat& image0,cv::Mat& image1,cv::Mat& image2
                                  ,cv::Mat& image3,std::vector<std::vector<cv::Point> >& fin)
{
    std::vector<std::vector<cv::Point> > contours;
    std::vector<std::vector<cv::Point> > seg;
    cv::Rect r;
    Mat detected_edges[4];
    detected_edges[0]=image0;
    detected_edges[1]=image1;
    detected_edges[2]=image2;
    detected_edges[3]=image3;

    for(int tt=0;tt<4;tt++)
    {
        findContours(detected_edges[tt].clone(), contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);

        std::vector<cv::Point> approx;
        for (unsigned long i = 0; i < contours.size(); i++)
        {
                // Approximate contour with accuracy proportional
                // to the contour perimeter
            /*if(i==1 || i==2)
                continue;*/
            cv::approxPolyDP(
                cv::Mat(contours[i]),
                approx,
                cv::arcLength(cv::Mat(contours[i]), true) * 0.02,
                //7,
                true
                );

                // Skip small or non-convex objects
                if (std::fabs(cv::contourArea(contours[i])) < 100 || !cv::isContourConvex(approx))
                continue;
                if(approx.size() >= 4)
                    {
                            seg.push_back(contours[i]);
                            //fin.push_back(contours[i]);

                            // Number of vertices of polygonal curve
                            int vtc = approx.size();

                            // Get the degree (in cosines) of all corners
                            std::vector<double> cos;
                            for (int j = 2; j < vtc+1; j++)
                                    cos.push_back(angle(approx[j%vtc], approx[j-2], approx[j-1]));

                            // Sort ascending the corner degree values
                            std::sort(cos.begin(), cos.end());

                            // Get the lowest and the highest degree
                            double mincos = cos.front();
                            double maxcos = cos.back();


                                // Use the degrees obtained above and the number of vertices
                                // to determine the shape of the contour
                                if (mincos >= minCos && maxcos <=maxCos)
                                {

                                        r = cv::boundingRect(contours[i]);
                                        double ratio = (double)r.width / r.height;
                                        //double imratio= (double)(r.width * r.height)/(src.cols *src.rows);

                                        if(ratio >minAspectRatio && ratio<maxAspectRatio)
                                            // && imratio >minimageRatio && imratio<maximageRatio)
                                            fin.push_back(contours[i]);

                                        //	std::cout<<ratio<<std::endl;//<<" "<<maxcos<<endl;
                                }
                    }
            }
        }
    //std::sort(fin.begin(), fin.end());
}
auto time_in=std::chrono::high_resolution_clock::now();
//***************************************
std::vector<Mat> MainWindow::affine_transform(cv::Mat src,std::vector<std::vector<cv::Point> >& fin)
{
    std::vector<Mat> res;
    std::vector<RotatedRect> num_list;
    Mat rotated;
    for(unsigned long i=0;i<fin.size();i++)
            {
            int flag=0;
            Mat roi= src(boundingRect(fin[i]));
            RotatedRect rec=minAreaRect(fin[i]);
            Point2f vertices[4];
            rec.points(vertices);
            Point2f src_vertices[3];
            Point2f cent;
            cent=rec.center;
            for(int z=0;z<4;z++)
            {
                if(vertices[z].x<cent.x && vertices[z].y < cent.y)
                    src_vertices[0]=vertices[z];
                else if(vertices[z].x>cent.x && vertices[z].y < cent.y)
                    src_vertices[1]=vertices[z];
                else if(vertices[z].x<cent.x && vertices[z].y > cent.y)
                    src_vertices[2]=vertices[z];
            }
            Point2f dst_vertices[3];
            dst_vertices[0] = Point(0, 0);
            dst_vertices[1] = Point(rec.boundingRect().width-1, 0);
            dst_vertices[2] = Point(0, rec.boundingRect().height-1);
            Mat warpAffineMatrix = getAffineTransform(src_vertices, dst_vertices);

            Size size(rec.boundingRect().width, rec.boundingRect().height);
            warpAffine(src, rotated, warpAffineMatrix, size, INTER_LINEAR, BORDER_CONSTANT);
            //prevent repetition
            for(unsigned long z=0;z<num_list.size();z++)
            {
                if(fabs(num_list[z].center.x-cent.x)<4 && fabs(num_list[z].center.y-cent.y)<4)
                    flag=1;
            }
            if(!flag)
            {
                res.push_back(rotated);
                num_list.push_back(rec);
            }

        }
    return res;
}
//***************************************
std::vector<cv::Mat> MainWindow::image_recognize(cv::Mat tmpp)
{
    Mat detected_edges[4];
//    std::thread lpp(detect_edges,tmpp,detected_edges[0],detected_edges[1],detected_edges[2],detected_edges[3]);
//    lpp.join();
    detect_edges(tmpp,detected_edges[0],detected_edges[1],detected_edges[2],detected_edges[3]);
    std::vector<std::vector<cv::Point> > fin;
    extract_contours(detected_edges[0],detected_edges[1],detected_edges[2],detected_edges[3],fin);
    std::vector<cv::Mat> re;
    if(fin.size()!=0)
        re=affine_transform(tmpp,fin);
    return re;
}
//***************************************
void MainWindow::image_recognize_v1(cv::Mat _src)
{
    freq = cv::getTickFrequency();
    CvSize ss;
    if(plate_Ent)
    {
         auto time_out = std::chrono::high_resolution_clock::now();
         std::chrono::duration<double> elapsed1 = time_out - time_in;
//         ui->lineEdit_3->setText(QString::number(elapsed1.count()));
         if(elapsed1.count()>time_elpased)
             plate_Ent=false;
         else
         {
             frame_count++;
             ss.width=(int)ui->label_v1->size().width();
             ss.height=(int)ui->label_v1->size().height();
             cv::resize(_src,_src,ss);
             cv::cvtColor(_src,_src,CV_BGR2RGB);
             QImage Qorg((uchar*)_src.data,_src.cols,_src.rows,_src.step,QImage::Format_RGB888);
             ui->label_v1->setPixmap(QPixmap::fromImage(Qorg));
             res.clear();
             return;
         }
    }

    cv::Mat tmpp2;
    cv::Mat src_car;
    if(record)
    {
        cv::Mat src=_src.clone();
        if(frame_rec<100)
        {
            video_rec->write(src);
            ui->lineEdit_2->setText(QString::number(frame_rec));
            frame_rec++;
        }
        else
        {
           record=false;
           video_rec->release();
           ui->record_camera_1->setEnabled(true);
        }
    }
    if(ui->time_interval_check->isChecked())
    {
        if(!interval_proc)
        {
            frame_count++;
            ss.width=(int)ui->label_v1->size().width();
            ss.height=(int)ui->label_v1->size().height();
            cv::resize(_src,_src,ss);
            cv::cvtColor(_src,_src,CV_BGR2RGB);
            QImage Qorg((uchar*)_src.data,_src.cols,_src.rows,_src.step,QImage::Format_RGB888);
            ui->label_v1->setPixmap(QPixmap::fromImage(Qorg));
            res.clear();
            return;
        }
    }
    cv::resize(_src,src_car,cv::Size(0.25 * _src.cols, 0.25 * _src.rows), cv::INTER_LANCZOS4);
    auto start = std::chrono::high_resolution_clock::now();
    cv::Mat res_plate=videoSortDetecotDemo->Process(src_car);
//    if(!res_plate.empty() && !first_detect)
//            first_detect=true;
//    bool is_same=true;
//    if(!detected_car.empty() && first_detect)
//    {
//        first_detect=false;
//        Mat image,image2;
//        cv::resize(detected_car, image, Size(64, 128));
//        Mat img;
//        cvtColor(image, img, CV_RGB2GRAY);

//        cv::resize(res_plate, image2, Size(64, 128));
//        Mat img2;
//        cvtColor(image2, img2, CV_RGB2GRAY);

//        std::vector<float> features;
//        std::vector<Point> locations;
//        std::vector<float> features2;
//        std::vector<Point> locations2;

//        HOGDescriptor *hog = new HOGDescriptor();
//        HOGDescriptor *hog2 = new HOGDescriptor();
//        hog->compute(img, features, Size(32, 32), Size(0, 0), locations);
//        hog2->compute(img2, features2, Size(32, 32), Size(0, 0), locations2);
//        Mat Hogfeat;
//        Hogfeat.create(features.size(), 1, CV_32FC1);
//        for (int i = 0; i<features.size(); i++)
//            Hogfeat.at<float>(i, 0) = features.at(i);
//        Mat Hogfeat2;
//        Hogfeat2.create(features2.size(), 1, CV_32FC1);
//        for (int i = 0; i<features2.size(); i++)
//            Hogfeat2.at<float>(i, 0) = features2.at(i);
//        double distance = 0;
//        for (int i = 0; i<Hogfeat.rows; i++)
//            distance += abs(Hogfeat.at<float>(i, 0) - Hogfeat2.at<float>(i, 0));
//        if(distance>50)
//            is_same=false;
//        std::cout<<"distance "<<distance<<std::endl;

//    }
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
//    std::cout<<"SortDetecotDemo "<<elapsed.count()`<<std::endl;
    ui->lineEdit_2->setText(QString::number(elapsed.count()));
   //if(!is_same || first_detect)
    {
       if(interval_proc)
          interval_proc=false;
      detected_car=res_plate;
      first_detect=false;

//       if(image_listt.size()>10)
//           image_listt.erase(image_listt.begin());
//       image_listt.push_back(_src);
      // for(unsigned long j = 0; j < image_listt.size()-1; ++j)
       {
           cv::Mat src=_src;//image_listt[j];
//           std::cout<< "image Number "+std::to_string(j) <<std::endl;
        if(src.cols>1000)
            cv::resize(src,tmpp2,cv::Size(0.5 * src.cols, 0.5 * src.rows), cv::INTER_LANCZOS4);
        int px=tmpp2.cols*0.1;
        cv::Rect cut_rect(px,0,tmpp2.cols-px,tmpp2.rows);
        GaussianBlur( tmpp2, tmpp2, cv::Size( 3, 3 ), 0, 0 );
        cv::Mat cutimg=tmpp2(cut_rect);
//        cv::cvtColor(tmpp2,tmpp2,CV_RGB2BGR);
//        if(!ui->checkBox_6->isChecked())
//            res2=image_recognize(tmpp2.clone());
//        else
        {
            cv_image<bgr_pixel> cimg(tmpp2);
            std::vector<rect_detection> dets;
            auto start = std::chrono::high_resolution_clock::now();
            evaluate_detectors(detectors_plate, cimg, dets,0.1);//-0.12
            auto finish = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed = finish - start;
            ui->lineEdit_3->setText(QString::number(elapsed.count()));
            if(dets.size()==0)
            {
                ss.width=(int)ui->label_v1->size().width();
                ss.height=(int)ui->label_v1->size().height();
                cv::resize(tmpp2,tmpp2,ss);
                QImage Qorg((uchar*)tmpp2.data,tmpp2.cols,tmpp2.rows,tmpp2.step,QImage::Format_RGB888);
                ui->label_v1->setPixmap(QPixmap::fromImage(Qorg));
                return;
            }
            std::sort(dets.begin(), dets.end(), myfunction1);
            cv::Rect plate_rect = dlib_rect_detectionToOpenCV(dets[0]);
            plate_rect.x=plate_rect.x-0.1*plate_rect.width;
            plate_rect.y=plate_rect.y-0.1*plate_rect.height;
            plate_rect.width=1.2*plate_rect.width;
            plate_rect.height=1.2*plate_rect.height;
            if(plate_rect.x>0 && plate_rect.y>0 && plate_rect.x+plate_rect.width<tmpp2.cols
                    && plate_rect.y+plate_rect.height<tmpp2.rows)
            {
                cv::Mat plate_img=tmpp2(plate_rect);
                res.push_back(plate_img);
            }
        }
        if(res.size()>0)
        {
            Mat gray,gray1,tmp1;
            std::sort(res.begin(), res.end(), myfunction0);
            for(unsigned long j=0;j<res.size();j++)
            {
                tmp1=res[j].clone();
                GaussianBlur( tmp1, tmp1, cv::Size( 3, 3 ), 0, 0 );
                cvtColor(tmp1,gray,CV_BGR2GRAY);
                copyMakeBorder(gray, gray, 15, 15, 15, 15, BORDER_CONSTANT, Scalar(0));
                if(tmp1.cols<250)
                {
                    cv::resize(gray, gray1, cv::Size(5 * gray.cols, 5 * gray.rows), cv::INTER_LANCZOS4);
                }
                else
                {
                    if(tmp1.cols<350)
                    {
                        cv::resize(gray, gray1,cv::Size(4 * gray.cols, 4 * gray.rows), cv::INTER_LANCZOS4);
                    }
                    else
                    {
                        if(tmp1.cols<450)
                        {
                            cv::resize(gray, gray1,cv::Size(3 * gray.cols, 3 * gray.rows), cv::INTER_LANCZOS4);
                        }
                        else
                        {
                            cv::resize(gray, gray1,cv::Size(2 * gray.cols, 2 * gray.rows), cv::INTER_LANCZOS4);
                        }
                    }
                }
                std::vector<cv::Rect> rects1;
                double vv;
                plate_digits = recognize_plate(gray1, detectors_number, digits,rects1,vv);
                if(plate_digits=="Unrecognized" || plate_digits.empty() || plate_digits.length()<=5)
                {
                    frame_count++;
                    ss.width=(int)ui->label_v1->size().width();
                    ss.height=(int)ui->label_v1->size().height();
                    cv::cvtColor(src,src,CV_BGR2RGB);
                    cv::resize(src,src,ss);
                    QImage Qorg((uchar*)src.data,src.cols,src.rows,src.step,QImage::Format_RGB888);
                    ui->label_v1->setPixmap(QPixmap::fromImage(Qorg));
//                    if( plate_digits.length()>2)
//                         imwrite("no_plate_"+QString::number(frame_count).toStdString()+".jpg", res[j]);
                    return;
                }
                double conf;
                cv::Rect bound_;
                cv::Scalar color_city;
                int mm=0;
                plate_region = recognize_region(gray1, detectors_region, regions,conf,bound_,color_city,mm);
                string plate_region1="none";
                if( plate_region=="none")
                {
                    frame_count++;
//                    l_cit++;
//                    ss.width=(int)ui->label_v1->size().width();
//                    ss.height=(int)ui->label_v1->size().height();
//                    cv::cvtColor(src,src,CV_BGR2RGB);
//                    cv::resize(src,src,ss);
//                    QImage Qorg((uchar*)src.data,src.cols,src.rows,src.step,QImage::Format_RGB888);
//                    ui->label_v1->setPixmap(QPixmap::fromImage(Qorg));
//                    //imwrite("no_plate_"+QString::number(l_cit).toStdString()+".jpg", res[j]);
//                    plate_region1="none";
//                    return;
                    continue;
                }
                else
                    plate_region1=Cities[mm].toStdString();
                plate_img=res[j].clone();
                new_plate=new Form(this);
                cv::cvtColor(res[j],res[j],CV_BGR2RGB);
                QImage Qorg((uchar*)res[j].data,res[j].cols,res[j].rows,res[j].step,QImage::Format_RGB888);
                QString data_path=".\\database\\"+QString::fromStdString(plate_digits)+
                        "_"+QString::fromStdString(plate_region1)+".jpg";
                QString data_path_all=".\\database\\"+QString::fromStdString(plate_digits)+
                        "_"+QString::fromStdString(plate_region1)+"_all.jpg";
                QFile immm(data_path);

                m_db.close();
                m_db.setDatabaseName(home_folder+"\\Stolen_Car.sqlite");

                if( !m_db.open() )
                {
                    qFatal("Failed to connect." );
                    return;
                }

                s_qry=QSqlQuery( m_db );
                s_qry.prepare("select * from Plate where Number ='"+QString::fromStdString(plate_digits)+"'");
                s_qry.exec();
                if(s_qry.next())
                {
                    s_qry.prepare("select * from Plate where City ='"+QString::fromStdString(plate_region)+"'");
                    s_qry.exec();
                    if(s_qry.next())
                    {
                        cpThread->tmrTimer->stop();
                        QMessageBox::warning(this,tr("تحذير"),tr("كشف سيارة مسروقة"),
                                                      QMessageBox::Ok);
                    }
                }

                m_db.close();
                m_db.setDatabaseName(home_folder+"\\Cars.sqlite");
                bool isexisted=false;
                m_db.open();
                qry = QSqlQuery( m_db );
                s_qry.prepare("select * from Plate where Number ='"+QString::fromStdString(plate_digits)+"'");
                s_qry.exec();
                if(s_qry.next())
                {
                    s_qry.prepare("select * from Plate where City ='"+QString::fromStdString(plate_region)+"'");
                    s_qry.exec();
                    if(s_qry.next())
                    {
                        isexisted=true;
                    }
                }
                if(immm.exists() && isexisted)
                {
                    plate_Ent=true;
                    time_in=std::chrono::high_resolution_clock::now();
                    frame_count++;
                    ss.width=(int)ui->label_v1->size().width();
                    ss.height=(int)ui->label_v1->size().height();
                    cv::resize(src,src,ss);
                    QImage Qorg((uchar*)src.data,src.cols,src.rows,src.step,QImage::Format_RGB888);
                    ui->label_v1->setPixmap(QPixmap::fromImage(Qorg));
                    return;
                }
                QString current = QDateTime::currentDateTime().toString(" MM/dd/yyyy - h:m:s");
                imwrite(data_path.toStdString(), plate_img);
                imwrite(data_path_all.toStdString(), tmpp2);
                qry.prepare("INSERT INTO Plate(Number,City,Date,Image,ACC,City_ACC,Image_) VALUES (?,?,?,?,?,?,?)");
                qry.addBindValue(QString::fromStdString(plate_digits));
                qry.addBindValue(QString::fromStdString(plate_region));
                qry.addBindValue( current);
                qry.addBindValue( data_path );
                qry.addBindValue( vv );
                qry.addBindValue( conf );
                qry.addBindValue( data_path_all );
                qDebug() << conf;
                if( !qry.exec() )
                    qDebug() << qry.lastError();
                else
                    qDebug( "Inserted!" );
                plate_Ent=true;
                time_in=std::chrono::high_resolution_clock::now();
                new_plate->plate=Qorg;
                new_plate->number=QString::fromStdString(plate_digits);
                new_plate->city=QString::fromStdString(plate_region);
                new_plate->conf=vv;
                new_plate->update();
                new_plate->showNormal();
                QListWidgetItem *item = new QListWidgetItem();
                item->setSizeHint(QSize(300,300));
                ui->listWidget_v1->insertItem(0,item);
                ui->listWidget_v1->setItemWidget(item,new_plate);
                item->setTextAlignment(Qt::AlignCenter);
            }
        }
       }
    }
    frame_count++;
    ss.width=(int)ui->label_v1->size().width();
    ss.height=(int)ui->label_v1->size().height();
    cv::resize(_src,_src,ss);
    cv::cvtColor(_src,_src,CV_BGR2RGB);
    QImage Qorg((uchar*)_src.data,_src.cols,_src.rows,_src.step,QImage::Format_RGB888);
    ui->label_v1->setPixmap(QPixmap::fromImage(Qorg));
    res.clear();
}
//***************************************
void MainWindow::on_open_v1_clicked()
{
    fileName.clear();
    fileName = QFileDialog::getOpenFileName(this,
         tr("Open Video"),"D:\\Company_work\\Cars",tr("Images (*.avi *.mp4)"));
    if(!fileName.isEmpty()){

        fileName =QDir::toNativeSeparators(fileName);
        std::string fileName_string = fileName.toStdString();
        videoSortDetecotDemo = new VideoSortDetector();
        videoSortDetecotDemo->setMinTraceSize(8);
        videoSortDetecotDemo->setDetectorParam("net/yolo-voc.cfg", "net/yolo-voc.weights", "net/voc.names", "car");
        if (!videoSortDetecotDemo->InitTracker(cv::Mat()))
            return;
        ui->listWidget_v1->clear();
        cpThread = new CaptureThread(fileName_string);
        video_proc=true;
        ui->open_v1->setEnabled(false);
        ui->open_ip_camera1->setEnabled(false);
        if(ui->time_interval_check->isChecked())
        {
            int_tmr=new QTimer(this);
            QObject::connect(int_tmr,SIGNAL(timeout()),this,SLOT(int_finish()));
            std::string::size_type sz;   // alias of size_t
            int i_dec = std::stoi (((ui->interval_time_combo->currentText()).toStdString()),&sz);
            int_tmr->start(i_dec*1000);
            interval_proc=false;

        }
        if(ui->checkBox_2->isChecked())
             QObject::connect(cpThread, SIGNAL(frameCaptured(cv::Mat)),this, SLOT(camera_show(cv::Mat)));
        else
            QObject::connect(cpThread, SIGNAL(frameCaptured(cv::Mat)),this, SLOT(image_recognize_v1(cv::Mat)));
        QObject::connect(cpThread, SIGNAL(finished()),this, SLOT(fini()));

        cpThread->start();
        frame_count=0;
        frame_proc=0;
        ui->stop_v1->setEnabled(true);
        ui->pause_v1->setEnabled(true);
        ui->record_camera_1->setEnabled(true);
        ui->time_interval_check->setEnabled(false);
        ui->interval_time_combo->setEnabled(false);
    }
}
//***************************************
void MainWindow::on_stop_v1_clicked()
{
    if(!ip)
    {
        if(ui->checkBox_2->isChecked())
            QObject::disconnect(cpThread, SIGNAL(frameCaptured(cv::Mat)),this, SLOT(camera_show(cv::Mat)));
        else
            QObject::disconnect(cpThread, SIGNAL(frameCaptured(cv::Mat)),this, SLOT(image_recognize_v1(cv::Mat)));
    }
    else
    {
        cpThread->release_ip();
        if(ui->checkBox_2->isChecked())
            QObject::disconnect(cpThread, SIGNAL(frameCaptured(cv::Mat)),this, SLOT(camera_show(cv::Mat)));
        else
            QObject::disconnect(cpThread, SIGNAL(frameCaptured(cv::Mat)),this, SLOT(image_recognize_v1(cv::Mat)));
    }
    video_proc=false;
    ip=false;
    cpThread->tmrTimer->stop();
    cpThread->stop=true;
    fileName.clear();
    ui->label_v1->clear();
    ui->listWidget_v1->clear();
    ui->stop_v1->setEnabled(false);
    ui->pause_v1->setEnabled(false);
    ui->open_v1->setEnabled(true);
    ui->open_ip_camera1->setEnabled(true);    
    ui->time_interval_check->setEnabled(true);
    if(ui->time_interval_check->isChecked())
        ui->interval_time_combo->setEnabled(true);
    l_cit=0;
    return;
}
//***************************************
void MainWindow::fini()
{
    fileName.clear();
    QMessageBox::StandardButton reply;

    reply = QMessageBox::information(this, "تحذير", "انتهاء الفيديو",
                                  QMessageBox::Ok);
    if(ip)
       cpThread->release_ip();

    if(ui->checkBox_2->isChecked())
        QObject::disconnect(cpThread, SIGNAL(frameCaptured(cv::Mat)),this, SLOT(camera_show(cv::Mat)));
    else
        QObject::disconnect(cpThread, SIGNAL(frameCaptured(cv::Mat)),this, SLOT(image_recognize_v1(cv::Mat)));
    cpThread->tmrTimer->stop();
    cpThread->stop=true;
    cpThread->tmrTimer->stop();
    cpThread->stop=true;
    ui->open_v1->setEnabled(true);
    ui->stop_v1->setEnabled(false);
    ui->pause_v1->setEnabled(false);
    ui->open_ip_camera1->setEnabled(true);
    ui->label_v1->clear();
    ip=false;
    l_cit=0;
    ui->record_camera_1->setEnabled(true);
    return;
}
//***************************************
void MainWindow::fini1()
{
    fileName.clear();
    QMessageBox::StandardButton reply;

    reply = QMessageBox::information(this, "تحذير", "انتهاء الفيديو",
                                  QMessageBox::Ok);
    if(ip1)
        cpThread1->release_ip();

    if(ui->checkBox_3->isChecked())
        QObject::disconnect(cpThread1, SIGNAL(frameCaptured(cv::Mat)),this, SLOT(camera_show1(cv::Mat)));
    else
        QObject::disconnect(cpThread1, SIGNAL(frameCaptured(cv::Mat)),this, SLOT(image_recognize_v2(cv::Mat)));
    cpThread1->tmrTimer->stop();
    cpThread1->stop=true;
    ui->open_v2->setEnabled(true);
    ui->stop_v2->setEnabled(false);
    ui->pause_v2->setEnabled(false);
    ui->open_ip_camera2->setEnabled(true);
    ui->label_v2->clear();
    ip1=false;
    l_cit1=0;
    ui->record_camera_2->setEnabled(true);
    return;
}
//***************************************
void MainWindow::fini2()
{
    fileName.clear();
    QMessageBox::StandardButton reply;

    reply = QMessageBox::information(this, "تحذير", "انتهاء الفيديو",
                                  QMessageBox::Ok);
    if(ip2)
     cpThread2->release_ip();

    if(ui->checkBox_5->isChecked())
        QObject::disconnect(cpThread2, SIGNAL(frameCaptured(cv::Mat)),this, SLOT(camera_show2(cv::Mat)));
    else
        QObject::disconnect(cpThread2, SIGNAL(frameCaptured(cv::Mat)),this, SLOT(image_recognize_v3(cv::Mat)));
    cpThread2->tmrTimer->stop();
    cpThread2->stop=true;
    ui->open_v3->setEnabled(true);
    ui->stop_v3->setEnabled(false);
    ui->pause_v3->setEnabled(false);
    ui->open_ip_camera3->setEnabled(true);
    ui->label_v3->clear();
    ip2=false;

    l_cit2=0;
    ui->record_camera_3->setEnabled(true);
    return;
}
//***************************************
void MainWindow::on_pause_v1_clicked()
{
    if(!ip)
    {
        if(cpThread->tmrTimer->isActive())
            cpThread->tmrTimer->stop();
        else
            cpThread->tmrTimer->start(40);
    }
    else
        cpThread->pause_ip();
}
//***************************************
void MainWindow::on_open_ip_camera1_clicked()
{
    ipp=new IP_Camera();
    ipp->setAttribute( Qt::WA_DeleteOnClose );
    ipp->showNormal();
    QObject::connect(ipp, SIGNAL(destroyed(QObject*)),this, SLOT(done()));
}
//***************************************
void MainWindow::done()
{
    if(ipp->my_closs)
    {
        const std::string videoStreamAddress = "rtsp://"+ipp->user.toStdString()+":"
                +ipp->pass.toStdString()+"@"+ipp->Ip.toStdString()+":554";///MediaInput/h264";
        qDebug() << QString::fromStdString(videoStreamAddress);
        cv::Size rtspRess(ipp->width, ipp->height);
        cpThread = new CaptureThread(QString::fromStdString(videoStreamAddress),rtspRess);
        video_proc=true;
        ui->open_v1->setEnabled(false);
        ui->open_ip_camera1->setEnabled(false);

        if(ui->checkBox_2->isChecked())
            QObject::connect(cpThread, SIGNAL(frameCaptured(cv::Mat)),this, SLOT(camera_show(cv::Mat)));
        else
        {
            videoSortDetecotDemo = new VideoSortDetector();
            videoSortDetecotDemo->setMinTraceSize(8);
            videoSortDetecotDemo->setDetectorParam("net/yolo-voc.cfg", "net/yolo-voc.weights", "net/voc.names", "car");
            if (!videoSortDetecotDemo->InitTracker(cv::Mat()))
                return;
            if(ui->time_interval_check->isChecked())
            {
                int_tmr=new QTimer(this);
                QObject::connect(int_tmr,SIGNAL(timeout()),this,SLOT(int_finish()));
                std::string::size_type sz;   // alias of size_t
                int i_dec = std::stoi (((ui->interval_time_combo->currentText()).toStdString()),&sz);
                int_tmr->start(i_dec*1000);
                interval_proc=false;

            }
            QObject::connect(cpThread, SIGNAL(frameCaptured(cv::Mat)),this, SLOT(image_recognize_v1(cv::Mat)));
        }

        cpThread->start();
        frame_count=0;
        frame_proc=0;
        ui->stop_v1->setEnabled(true);
        ui->pause_v1->setEnabled(true);
        ip=true;
        ui->time_interval_check->setEnabled(false);
        ui->interval_time_combo->setEnabled(false);
    }
    return;
}
//***************************************
void MainWindow::done1()
{
    if(ipp->my_closs)
    {
        const std::string videoStreamAddress = "rtsp://"+ipp->user.toStdString()+":"+ipp->pass.toStdString()
                +"@"+ipp->Ip.toStdString()+"/MediaInput/h264";
        qDebug() << QString::fromStdString(videoStreamAddress);
        cv::Size rtspRess(ipp->width, ipp->height);

        cpThread1 = new CaptureThread1(QString::fromStdString(videoStreamAddress),rtspRess);
            video_proc1=true;
            ui->open_v2->setEnabled(false);
            ui->open_ip_camera2->setEnabled(false);
            if(ui->checkBox_3->isChecked())
                QObject::connect(cpThread1, SIGNAL(frameCaptured(cv::Mat)),this, SLOT(camera_show1(cv::Mat)));
            else
                QObject::connect(cpThread1, SIGNAL(frameCaptured(cv::Mat)),this, SLOT(image_recognize_v2(cv::Mat)));

            cpThread1->start();
            frame_count1=0;
            frame_proc=0;
            ui->stop_v2->setEnabled(true);
            ui->pause_v2->setEnabled(true);
            ip1=true;
            img_num=ui->Pic_Num->text().toInt();
    }
    return;
}
//***************************************
void MainWindow::done2()
{
    if(ipp->my_closs)
    {
        const std::string videoStreamAddress = "rtsp://"+ipp->user.toStdString()+":"+ipp->pass.toStdString()
                +"@"+ipp->Ip.toStdString()+"/MediaInput/h264";
        qDebug() << QString::fromStdString(videoStreamAddress);
        cv::Size rtspRess(ipp->width, ipp->height);

        cpThread2 = new CaptureThread2(QString::fromStdString(videoStreamAddress),rtspRess);
            video_proc2=true;
            ui->open_v3->setEnabled(false);
            ui->open_ip_camera3->setEnabled(false);
            if(ui->checkBox_5->isChecked())
                QObject::connect(cpThread2, SIGNAL(frameCaptured(cv::Mat)),this, SLOT(camera_show2(cv::Mat)));
            else
                QObject::connect(cpThread2, SIGNAL(frameCaptured(cv::Mat)),this, SLOT(image_recognize_v3(cv::Mat)));

            cpThread2->start();
            frame_count2=0;
            frame_proc=0;
            ui->stop_v3->setEnabled(true);
            ui->pause_v3->setEnabled(true);
            ip2=true;
    }
    return;
}
//***************************************
void MainWindow::camera_show(cv::Mat src1)
{
    frame_count++;
    if(record)
    {
        if(frame_rec<100)
        {
            video_rec->write(src1);
            ui->lineEdit_2->setText(QString::number(frame_rec));
            frame_rec++;
        }
        else
        {
           record=false;
           video_rec->release();
           ui->record_camera_1->setEnabled(true);
        }
    }
    CvSize ss;
    cv::Mat src;
    ss.width=(int)ui->label_v1->size().width();
    ss.height=(int)ui->label_v1->size().height();
    if(src1.empty())
        return;
    cv::resize(src1,src,ss);
    cv::cvtColor(src,src,CV_BGR2RGB);
    QImage Qorg((uchar*)src.data,src.cols,src.rows,src.step,QImage::Format_RGB888);
//    Qorg.invertPixels();
    ui->label_v1->setPixmap(QPixmap::fromImage(Qorg));
}
//***************************************
void MainWindow::on_record_camera_1_clicked()
{

    QString current_t = QTime::currentTime().toString("h_m");
    record=true;
    string gg=home_folder.toStdString()+"\\"+current_t.toStdString()+".avi";
    cv::Size ss;
    if(ip)
        ss=cv::Size (ipp->width,ipp->height);
    else
        ss=cv::Size(cpThread->capWebam.get(CV_CAP_PROP_FRAME_WIDTH),cpThread->capWebam.get(CV_CAP_PROP_FRAME_WIDTH));
    video_rec = new cv::VideoWriter( gg.c_str(), CV_FOURCC('m','p', '4', 'v'), 15, ss );
    ui->record_camera_1->setEnabled(false);
    frame_rec=0;

}
//***************************************
void MainWindow::on_open_v2_clicked()
{
    fileName.clear();
    fileName = QFileDialog::getOpenFileName(this,
         tr("Open Video"),"D:\\Company_work\\Cars",tr("Images (*.avi *.mp4)"));
    if(!fileName.isEmpty()){

        fileName =QDir::toNativeSeparators(fileName);
        std::string fileName_string = fileName.toStdString();

        ui->listWidget_v2->clear();
        cpThread1 = new CaptureThread1(fileName_string);
        video_proc1=true;
        ui->open_v2->setEnabled(false);
        ui->open_ip_camera2->setEnabled(false);

        if(ui->checkBox_3->isChecked())
             QObject::connect(cpThread1, SIGNAL(frameCaptured(cv::Mat)),this, SLOT(camera_show1(cv::Mat)));
        else
            QObject::connect(cpThread1, SIGNAL(frameCaptured(cv::Mat)),this, SLOT(image_recognize_v2(cv::Mat)));
        QObject::connect(cpThread1, SIGNAL(finished()),this, SLOT(fini1()));
        cpThread1->start();
        frame_count=0;
        frame_proc=0;
        ui->stop_v2->setEnabled(true);
        ui->pause_v2->setEnabled(true);
        ui->record_camera_2->setEnabled(true);
        img_num=ui->Pic_Num->text().toInt();
    }
}
//***************************************
void MainWindow::camera_show1(cv::Mat src1)
{
    frame_count1++;
    CvSize ss;
    cv::Mat src;
    ss.width=(int)ui->label_v2->size().width();
    ss.height=(int)ui->label_v2->size().height();
    if(src1.empty())
        return;
    cv::resize(src1,src,ss);
    cv::cvtColor(src,src,CV_BGR2RGB);
    QImage Qorg((uchar*)src.data,src.cols,src.rows,src.step,QImage::Format_RGB888);
    //    Qorg.invertPixels();
    ui->label_v2->setPixmap(QPixmap::fromImage(Qorg));
}
//***************************************
void MainWindow::image_recognize_v2(cv::Mat src)
{
    cv::Mat tmpp1;
    CvSize ss;
 /*   if(record)
    {
        if(frame_rec<100)
        {
            IplImage* image2=cvCloneImage(&(IplImage)src);
            cvWriteFrame(video_rec,image2);
            ui->lineEdit_2->setText(QString::number(frame_rec));
            frame_rec++;
        }
        else
        {
           record=false;
           cvReleaseVideoWriter( &video_rec );
           ui->record_camera_1->setEnabled(true);
        }
    }*/
   if(obstacle && !prev_obstacle && img_num>0)
    {
       img_num--;
       if(img_num==1)
           prev_obstacle=true;
       //img_num=ui->Pic_Num->text().toInt();
       ui->num_pic->setText(QString::number(img_num));
       if(src.cols>1000)
//            cv::pyrDown(src, tmpp1, cv::Size(0.5 * src.cols, 0.5 * src.rows ) );
            cv::resize(src,tmpp1,cv::Size(0.5 * src.cols, 0.5 * src.rows), cv::INTER_LANCZOS4);
        GaussianBlur( tmpp1, tmpp1, cv::Size( 3, 3 ), 0, 0 );
        cv::cvtColor(tmpp1,tmpp1,CV_RGB2BGR);
        if(!ui->checkBox_4->isChecked())
            res1=image_recognize(tmpp1.clone());
        else
        {
            cv_image<bgr_pixel> cimg(tmpp1);
            std::vector<rect_detection> dets;
            evaluate_detectors(detectors_plate, cimg, dets,-0.12);//-0.12
            if(dets.size()==0)
            {
                ss.width=(int)ui->label_v2->size().width();
                ss.height=(int)ui->label_v2->size().height();
                cv::resize(tmpp1,tmpp1,ss);
                QImage Qorg((uchar*)tmpp1.data,tmpp1.cols,tmpp1.rows,tmpp1.step,QImage::Format_RGB888);
                ui->label_v2->setPixmap(QPixmap::fromImage(Qorg));
                return;
            }
            std::sort(dets.begin(), dets.end(), myfunction1);
            cv::Rect plate_rect = dlib_rect_detectionToOpenCV(dets[0]);
            if(plate_rect.x>0 && plate_rect.y>0 && plate_rect.x+plate_rect.width<tmpp1.cols
                    && plate_rect.y+plate_rect.height<tmpp1.rows)
            {
                cv::Mat plate_img=tmpp1(plate_rect);
                res1.push_back(plate_img);
            }
        }
        if(res1.size()>0)
        {
            Mat gray,gray1,tmp1;
            std::sort(res1.begin(), res1.end(), myfunction0);
            for(unsigned long j=0;j<res1.size();j++)
            {
                tmp1=res1[j].clone();
                cvtColor(tmp1,gray,CV_BGR2GRAY);
                if(tmp1.cols<250)
                {
    //                cv::pyrUp( gray, gray1, cv::Size(8 * gray.cols, 8 * gray.rows ) );
                    cv::resize(gray, gray1, cv::Size(5 * gray.cols, 5 * gray.rows), cv::INTER_LANCZOS4);
                }
                else
                {
                    if(tmp1.cols<350)
                    {
    //                    cv::pyrUp( gray, gray1, cv::Size(6 * gray.cols, 6 * gray.rows ) );
                        cv::resize(gray, gray1,cv::Size(4 * gray.cols, 4 * gray.rows), cv::INTER_LANCZOS4);
                    }
                    else
                    {
                        if(tmp1.cols<450)
                        {
    //                        cv::pyrUp( gray, gray1, cv::Size(4 * gray.cols, 4 * gray.rows ) );
                            cv::resize(gray, gray1,cv::Size(3 * gray.cols, 3 * gray.rows), cv::INTER_LANCZOS4);
                        }
                        else
                        {
    //                        cv::pyrUp( gray, gray1, cv::Size(2 * gray.cols, 2 * gray.rows ) );
                            cv::resize(gray, gray1,cv::Size(2 * gray.cols, 2 * gray.rows), cv::INTER_LANCZOS4);
                        }
                    }
                }
                std::vector<cv::Rect> rects1;
                double vv;
                plate_digits = recognize_plate(gray1, detectors_number, digits,rects1,vv);
                if(plate_digits=="Unrecognized" || plate_digits.empty() || plate_digits.length()<=3)
                {

//                    frame_count++;
//                    ss.width=(int)ui->label_v1->size().width();
//                    ss.height=(int)ui->label_v1->size().height();
//                    cv::cvtColor(src,src,CV_BGR2RGB);
//                    cv::resize(src,src,ss);
//                    QImage Qorg((uchar*)src.data,src.cols,src.rows,src.step,QImage::Format_RGB888);
//                    ui->label_v1->setPixmap(QPixmap::fromImage(Qorg));
//                    if( plate_digits.length()>2)
//                         imwrite("no_plate_"+QString::number(frame_count).toStdString()+".jpg", res[j]);
                    continue;
                }
                double conf;
                cv::Rect bound_;
                cv::Scalar color_city;
                int mm=0;
                plate_region = recognize_region(gray1, detectors_region, regions,conf,bound_,color_city,mm);
                string plate_region1;
                if( plate_region=="none" )
                {
//                    frame_count++;
                    l_cit++;
//                    ss.width=(int)ui->label_v1->size().width();
//                    ss.height=(int)ui->label_v1->size().height();
//                    cv::cvtColor(src,src,CV_BGR2RGB);
//                    cv::resize(src,src,ss);
//                    QImage Qorg((uchar*)src.data,src.cols,src.rows,src.step,QImage::Format_RGB888);
//                    ui->label_v1->setPixmap(QPixmap::fromImage(Qorg));
//                    ui->lineEdit_4->setText(QString::number(l_cit));
                    //imwrite("no_plate_"+QString::number(l_cit).toStdString()+".jpg", res[j]);
                    plate_region1="none";
                    //continue;
                }
                else
                    plate_region1=Cities[mm].toStdString();
                plate_img=res1[j].clone();
                new_plate=new Form(this);
                cv::cvtColor(res1[j],res1[j],CV_BGR2RGB);
                QImage Qorg((uchar*)res1[j].data,res1[j].cols,res1[j].rows,res1[j].step,QImage::Format_RGB888);
                QString data_path=".\\database\\"+QString::fromStdString(plate_digits)
                        +"_"+QString::fromStdString(plate_region1)+".jpg";
                QString data_path_all=".\\database\\"+QString::fromStdString(plate_digits)
                        +"_"+QString::fromStdString(plate_region1)+"_all.jpg";
                QFile immm(data_path);

                m_db.close();
                m_db.setDatabaseName(home_folder+"\\Stolen_Car.sqlite");

                if( !m_db.open() )
                {
                    qFatal("Failed to connect." );
                    return;
                }

                s_qry=QSqlQuery( m_db );
                s_qry.prepare("select * from Plate where Number ='"+QString::fromStdString(plate_digits)+"'");
                s_qry.exec();
                if(s_qry.next())
                {
                    s_qry.prepare("select * from Plate where City ='"+QString::fromStdString(plate_region)+"'");
                    s_qry.exec();
                    if(s_qry.next())
                    {
                        cpThread->tmrTimer->stop();
                        QMessageBox::warning(this,tr("تحذير"),tr("كشف سيارة مسروقة"),
                                                      QMessageBox::Ok);
                    }
                }

                m_db.close();
                m_db.setDatabaseName(home_folder+"\\Cars.sqlite");
                m_db.open();
                qry = QSqlQuery( m_db );

                if(immm.exists())
                {
//                    frame_count++;
//                    ss.width=(int)ui->label_v1->size().width();
//                    ss.height=(int)ui->label_v1->size().height();
//                    cv::resize(src,src,ss);
//                    QImage Qorg((uchar*)src.data,src.cols,src.rows,src.step,QImage::Format_RGB888);
//                    ui->label_v1->setPixmap(QPixmap::fromImage(Qorg));
                    continue;
                }
                QString current = QDateTime::currentDateTime().toString(" MM/dd/yyyy - h:m");
                imwrite(data_path.toStdString(), plate_img);
                imwrite(data_path_all.toStdString(), tmpp1);
                qry.prepare("INSERT INTO Plate(Number,City,Date,Image) VALUES (?,?,?,?)");
                qry.addBindValue(QString::fromStdString(plate_digits));
                qry.addBindValue(QString::fromStdString(plate_region));
                qry.addBindValue( current);
                qry.addBindValue( data_path );
                if( !qry.exec() )
                    qDebug() << qry.lastError();
                else
                    qDebug( "Inserted!" );
                new_plate->plate=Qorg;
                new_plate->number=QString::fromStdString(plate_digits);
                new_plate->city=QString::fromStdString(plate_region);
                new_plate->conf=vv;
                new_plate->update();
                new_plate->showNormal();
                QListWidgetItem *item = new QListWidgetItem();
                item->setSizeHint(QSize(300,300));
                ui->listWidget_v2->insertItem(0,item);
                ui->listWidget_v2->setItemWidget(item,new_plate);
                item->setTextAlignment(Qt::AlignCenter);

            }

        }
    }

    frame_count1++;
    ss.width=(int)ui->label_v2->size().width();
    ss.height=(int)ui->label_v2->size().height();
    cv::resize(src,src,ss);
    cv::cvtColor(src,src,CV_BGR2RGB);
    QImage Qorg((uchar*)src.data,src.cols,src.rows,src.step,QImage::Format_RGB888);
    ui->label_v2->setPixmap(QPixmap::fromImage(Qorg));
    res1.clear();
}
//***************************************
void MainWindow::on_pause_v2_clicked()
{
    if(!ip1)
    {
        if(cpThread1->tmrTimer->isActive())
            cpThread1->tmrTimer->stop();
        else
            cpThread1->tmrTimer->start(40);
    }
    else
        cpThread1->pause_ip();
}
//***************************************
void MainWindow::on_stop_v2_clicked()
{
    if(!ip1)
    {
        if(ui->checkBox_3->isChecked())
            QObject::disconnect(cpThread1, SIGNAL(frameCaptured(cv::Mat)),this, SLOT(camera_show1(cv::Mat)));
        else
            QObject::disconnect(cpThread1, SIGNAL(frameCaptured(cv::Mat)),this, SLOT(image_recognize_v2(cv::Mat)));
    }
    else
    {
        cpThread1->release_ip();
        if(ui->checkBox_3->isChecked())
            QObject::disconnect(cpThread1, SIGNAL(frameCaptured(cv::Mat)),this, SLOT(camera_show1(cv::Mat)));
        else
            QObject::disconnect(cpThread1, SIGNAL(frameCaptured(cv::Mat)),this, SLOT(image_recognize_v2(cv::Mat)));
    }
    video_proc1=false;
    ip1=false;
    cpThread1->tmrTimer->stop();
    cpThread1->stop=true;
    fileName.clear();
    ui->label_v2->clear();
    ui->listWidget_v2->clear();
    ui->stop_v2->setEnabled(false);
    ui->pause_v2->setEnabled(false);
    ui->open_v2->setEnabled(true);
    ui->open_ip_camera2->setEnabled(true);
    l_cit1=0;
    return;
}
//***************************************
void MainWindow::on_open_ip_camera2_clicked()
{
    ipp=new IP_Camera();
    ipp->setAttribute( Qt::WA_DeleteOnClose );
    ipp->showNormal();
    QObject::connect(ipp, SIGNAL(destroyed(QObject*)),this, SLOT(done1()));
}
//***************************************
void MainWindow::on_open_v3_clicked()
{
    fileName.clear();
    fileName = QFileDialog::getOpenFileName(this,
         tr("Open Video"),"D:\\Company_work\\Cars",tr("Images (*.avi *.mp4)"));
    if(!fileName.isEmpty()){

        fileName =QDir::toNativeSeparators(fileName);
        std::string fileName_string = fileName.toStdString();

        ui->listWidget_v3->clear();
        cpThread2 = new CaptureThread2(fileName_string);
        video_proc2=true;
        ui->open_v3->setEnabled(false);
        ui->open_ip_camera3->setEnabled(false);

        if(ui->checkBox_5->isChecked())
             QObject::connect(cpThread2, SIGNAL(frameCaptured(cv::Mat)),this, SLOT(camera_show2(cv::Mat)));
        else
            QObject::connect(cpThread2, SIGNAL(frameCaptured(cv::Mat)),this, SLOT(image_recognize_v3(cv::Mat)));
        QObject::connect(cpThread2, SIGNAL(finished()),this, SLOT(fini2()));
        cpThread2->start();
        frame_count2=0;
        frame_proc=0;
        ui->stop_v3->setEnabled(true);
        ui->pause_v3->setEnabled(true);
        ui->record_camera_3->setEnabled(true);
    }

}
//***************************************
void MainWindow::camera_show2(cv::Mat src1)
{
    frame_count2++;
    CvSize ss;
    cv::Mat src;
    ss.width=(int)ui->label_v3->size().width();
    ss.height=(int)ui->label_v3->size().height();
    if(src1.empty())
        return;
    cv::resize(src1,src,ss);
    cv::cvtColor(src,src,CV_BGR2RGB);
    QImage Qorg((uchar*)src.data,src.cols,src.rows,src.step,QImage::Format_RGB888);
    //    Qorg.invertPixels();
    ui->label_v3->setPixmap(QPixmap::fromImage(Qorg));
}
//***************************************
void MainWindow::image_recognize_v3(cv::Mat _src)
{
    CvSize ss;
    cv::Mat tmpp2;
 /*   if(record)
    {
        if(frame_rec<100)
        {
            IplImage* image2=cvCloneImage(&(IplImage)src);
            cvWriteFrame(video_rec,image2);
            ui->lineEdit_2->setText(QString::number(frame_rec));
            frame_rec++;
        }
        else
        {
           record=false;
           cvReleaseVideoWriter( &video_rec );
           ui->record_camera_1->setEnabled(true);
        }
    }*/
//   bool car_exist=false;//find_cars_dnn(_src);
//   matrix<rgb_pixel> img;
//   cv_image<rgb_pixel> image(_src);
//   assign_image(img, image);
//   for (auto&& d : net(img))
//   {
//      auto fd = sp(img,d);
//      if (d.label=="front" && d.detection_confidence>0.5)
//      {
//         car_exist=true;
//         break;
//      }
//   }
   if(Trigger)
    {
       Trigger=false;
//       if(image_listt.size()>10)
//           image_listt.erase(image_listt.begin());
//       image_listt.push_back(_src);
      // for(unsigned long j = 0; j < image_listt.size()-1; ++j)
       {
           cv::Mat src=_src;//image_listt[j];
//           std::cout<< "image Number "+std::to_string(j) <<std::endl;
        if(src.cols>1000)
//            cv::pyrDown(src, tmpp2, cv::Size(0.5 * src.cols, 0.5 * src.rows ) );
            cv::resize(src,tmpp2,cv::Size(0.5 * src.cols, 0.5 * src.rows), cv::INTER_LANCZOS4);
        GaussianBlur( tmpp2, tmpp2, cv::Size( 3, 3 ), 0, 0 );
//        cv::cvtColor(tmpp2,tmpp2,CV_RGB2BGR);
//        if(!ui->checkBox_6->isChecked())
//            res2=image_recognize(tmpp2.clone());
//        else
        {
            cv_image<bgr_pixel> cimg(tmpp2);
            std::vector<rect_detection> dets;
            evaluate_detectors(detectors_plate, cimg, dets,0.1);//-0.12
            if(dets.size()==0)
            {
                ss.width=(int)ui->label_v3->size().width();
                ss.height=(int)ui->label_v3->size().height();
                cv::resize(tmpp2,tmpp2,ss);
                QImage Qorg((uchar*)tmpp2.data,tmpp2.cols,tmpp2.rows,tmpp2.step,QImage::Format_RGB888);
                ui->label_v3->setPixmap(QPixmap::fromImage(Qorg));
                return;
            }
            std::sort(dets.begin(), dets.end(), myfunction1);
            cv::Rect plate_rect = dlib_rect_detectionToOpenCV(dets[0]);
            plate_rect.x=plate_rect.x-0.1*plate_rect.width;
            plate_rect.y=plate_rect.y-0.1*plate_rect.height;
            plate_rect.width=1.2*plate_rect.width;
            plate_rect.height=1.2*plate_rect.height;
            if(plate_rect.x>0 && plate_rect.y>0 && plate_rect.x+plate_rect.width<tmpp2.cols
                    && plate_rect.y+plate_rect.height<tmpp2.rows)
            {
                cv::Mat plate_img=tmpp2(plate_rect);
                res2.push_back(plate_img);
            }
        }
        if(res2.size()>0)
        {
            Mat gray,gray1,tmp1;
            std::sort(res2.begin(), res2.end(), myfunction0);
            for(unsigned long j=0;j<res2.size();j++)
            {
                tmp1=res2[j].clone();
                cvtColor(tmp1,gray,CV_BGR2GRAY);
                copyMakeBorder(gray, gray, 15, 15, 15, 15, BORDER_CONSTANT, Scalar(0));
                if(tmp1.cols<250)
                {
    //                cv::pyrUp( gray, gray1, cv::Size(8 * gray.cols, 8 * gray.rows ) );
                    cv::resize(gray, gray1, cv::Size(5 * gray.cols, 5 * gray.rows), cv::INTER_LANCZOS4);
                }
                else
                {
                    if(tmp1.cols<350)
                    {
    //                    cv::pyrUp( gray, gray1, cv::Size(6 * gray.cols, 6 * gray.rows ) );
                        cv::resize(gray, gray1,cv::Size(4 * gray.cols, 4 * gray.rows), cv::INTER_LANCZOS4);
                    }
                    else
                    {
                        if(tmp1.cols<450)
                        {
    //                        cv::pyrUp( gray, gray1, cv::Size(4 * gray.cols, 4 * gray.rows ) );
                            cv::resize(gray, gray1,cv::Size(3 * gray.cols, 3 * gray.rows), cv::INTER_LANCZOS4);
                        }
                        else
                        {
    //                        cv::pyrUp( gray, gray1, cv::Size(2 * gray.cols, 2 * gray.rows ) );
                            cv::resize(gray, gray1,cv::Size(2 * gray.cols, 2 * gray.rows), cv::INTER_LANCZOS4);
                        }
                    }
                }
                std::vector<cv::Rect> rects1;
                double vv;
                plate_digits = recognize_plate(gray1, detectors_number, digits,rects1,vv);
                if(plate_digits=="Unrecognized" || plate_digits.empty() || plate_digits.length()<=3)
                {

//                    frame_count++;
//                    ss.width=(int)ui->label_v1->size().width();
//                    ss.height=(int)ui->label_v1->size().height();
//                    cv::cvtColor(src,src,CV_BGR2RGB);
//                    cv::resize(src,src,ss);
//                    QImage Qorg((uchar*)src.data,src.cols,src.rows,src.step,QImage::Format_RGB888);
//                    ui->label_v1->setPixmap(QPixmap::fromImage(Qorg));
//                    if( plate_digits.length()>2)
//                         imwrite("no_plate_"+QString::number(frame_count).toStdString()+".jpg", res[j]);
                    continue;
                }
                double conf;
                cv::Rect bound_;
                cv::Scalar color_city;
                int mm=0;
                plate_region = recognize_region(gray1, detectors_region, regions,conf,bound_,color_city,mm);
                string plate_region1;
                if( plate_region=="none" )
                {
//                    frame_count++;
                    l_cit++;
//                    ss.width=(int)ui->label_v1->size().width();
//                    ss.height=(int)ui->label_v1->size().height();
//                    cv::cvtColor(src,src,CV_BGR2RGB);
//                    cv::resize(src,src,ss);
//                    QImage Qorg((uchar*)src.data,src.cols,src.rows,src.step,QImage::Format_RGB888);
//                    ui->label_v1->setPixmap(QPixmap::fromImage(Qorg));
//                    ui->lineEdit_4->setText(QString::number(l_cit));
                    //imwrite("no_plate_"+QString::number(l_cit).toStdString()+".jpg", res[j]);
                    plate_region1="none";
                    //continue;
                }
                else
                    plate_region1=Cities[mm].toStdString();
                plate_img=res2[j].clone();
                new_plate=new Form(this);
                cv::cvtColor(res2[j],res2[j],CV_BGR2RGB);
                QImage Qorg((uchar*)res2[j].data,res2[j].cols,res2[j].rows,res2[j].step,QImage::Format_RGB888);
                QString data_path=".\\database\\"+QString::fromStdString(plate_digits)
                        +"_"+QString::fromStdString(plate_region1)+".jpg";
                QString data_path_all=".\\database\\"+QString::fromStdString(plate_digits)
                        +"_"+QString::fromStdString(plate_region1)+"_all.jpg";
                QFile immm(data_path);

                m_db.close();
                m_db.setDatabaseName(home_folder+"\\Stolen_Car.sqlite");

                if( !m_db.open() )
                {
                    qFatal("Failed to connect." );
                    return;
                }

                s_qry=QSqlQuery( m_db );
                s_qry.prepare("select * from Plate where Number ='"+QString::fromStdString(plate_digits)+"'");
                s_qry.exec();
                if(s_qry.next())
                {
                    s_qry.prepare("select * from Plate where City ='"+QString::fromStdString(plate_region)+"'");
                    s_qry.exec();
                    if(s_qry.next())
                    {
                        cpThread->tmrTimer->stop();
                        QMessageBox::warning(this,tr("تحذير"),tr("كشف سيارة مسروقة"),
                                                      QMessageBox::Ok);
                    }
                }

                m_db.close();
                m_db.setDatabaseName(home_folder+"\\Cars.sqlite");
                m_db.open();
                qry = QSqlQuery( m_db );

                if(immm.exists())
                {
//                    frame_count++;
//                    ss.width=(int)ui->label_v1->size().width();
//                    ss.height=(int)ui->label_v1->size().height();
//                    cv::resize(src,src,ss);
//                    QImage Qorg((uchar*)src.data,src.cols,src.rows,src.step,QImage::Format_RGB888);
//                    ui->label_v1->setPixmap(QPixmap::fromImage(Qorg));
                    continue;
                }
                QString current = QDateTime::currentDateTime().toString(" MM/dd/yyyy - h:m");
                imwrite(data_path.toStdString(), plate_img);
                imwrite(data_path_all.toStdString(), tmpp2);
                qry.prepare("INSERT INTO Plate(Number,City,Date,Image) VALUES (?,?,?,?)");
                qry.addBindValue(QString::fromStdString(plate_digits));
                qry.addBindValue(QString::fromStdString(plate_region));
                qry.addBindValue( current);
                qry.addBindValue( data_path );
                if( !qry.exec() )
                    qDebug() << qry.lastError();
                else
                    qDebug( "Inserted!" );
                new_plate->plate=Qorg;
                new_plate->number=QString::fromStdString(plate_digits);
                new_plate->city=QString::fromStdString(plate_region);
                new_plate->conf=vv;
                new_plate->update();
                new_plate->showNormal();
                QListWidgetItem *item = new QListWidgetItem();
                item->setSizeHint(QSize(300,300));
                ui->listWidget_v3->insertItem(0,item);
                ui->listWidget_v3->setItemWidget(item,new_plate);
                item->setTextAlignment(Qt::AlignCenter);
            }
        }
       }
    }
//   if(!Trigger)
//   {
//       if(image_listt.size()>=10)
//           image_listt.erase(image_listt.begin());
//       image_listt.push_back(_src);
//   }
//   Trigger=false;
//    std::cout<<std::to_string(image_listt.size())<<std::endl;
    frame_count2++;
    ss.width=(int)ui->label_v3->size().width();
    ss.height=(int)ui->label_v3->size().height();
    cv::resize(_src,_src,ss);
    cv::cvtColor(_src,_src,CV_BGR2RGB);
    QImage Qorg((uchar*)_src.data,_src.cols,_src.rows,_src.step,QImage::Format_RGB888);
    ui->label_v3->setPixmap(QPixmap::fromImage(Qorg));
    res2.clear();
}
//***************************************
void MainWindow::on_pause_v3_clicked()
{
    if(!ip2)
    {
        if(cpThread2->tmrTimer->isActive())
            cpThread2->tmrTimer->stop();
        else
            cpThread2->tmrTimer->start(40);
    }
    else
        cpThread2->pause_ip();
}
//***************************************
void MainWindow::on_stop_v3_clicked()
{
    if(!ip2)
    {
        if(ui->checkBox_5->isChecked())
            QObject::disconnect(cpThread2, SIGNAL(frameCaptured(cv::Mat)),this, SLOT(camera_show2(cv::Mat)));
        else
            QObject::disconnect(cpThread2, SIGNAL(frameCaptured(cv::Mat)),this, SLOT(image_recognize_v3(cv::Mat)));
    }
    else
    {
        cpThread2->release_ip();
        if(ui->checkBox_5->isChecked())
            QObject::disconnect(cpThread2, SIGNAL(frameCaptured(cv::Mat)),this, SLOT(camera_show2(cv::Mat)));
        else
            QObject::disconnect(cpThread2, SIGNAL(frameCaptured(cv::Mat)),this, SLOT(image_recognize_v3(cv::Mat)));
    }
    video_proc2=false;
    ip2=false;
    cpThread2->tmrTimer->stop();
    cpThread2->stop=true;
    fileName.clear();
    ui->label_v3->clear();
    ui->listWidget_v3->clear();
    ui->stop_v3->setEnabled(false);
    ui->pause_v3->setEnabled(false);
    ui->open_v3->setEnabled(true);
    ui->open_ip_camera3->setEnabled(true);
    l_cit2=0;
}
//***************************************
void MainWindow::on_open_ip_camera3_clicked()
{
    ipp=new IP_Camera();
    ipp->setAttribute( Qt::WA_DeleteOnClose );
    ipp->showNormal();
    QObject::connect(ipp, SIGNAL(destroyed(QObject*)),this, SLOT(done2()));
    ip_num=2;
}
//***************************************
void MainWindow::on_Trigger_bt_clicked()
{
     Trigger=true;
}
//***************************************
void MainWindow::on_Serial_bT_clicked()
{
    if(ser->isOpen())
    {
        ser->close();
        disconnect(ser_tmr, SIGNAL(timeout()), this, SLOT(readrec()));
        ui->Por_name->setEnabled(true);
        obstacle=false;
        num_obs=0;
        ser_tmr->stop();
    }
    else
    {
        ui->Por_name->setEnabled(false);
        ser->setBaudRate(QSerialPort::Baud19200);
        ser->setDataBits(QSerialPort::Data8);
        ser->setParity(QSerialPort::NoParity);
        ser->setStopBits(QSerialPort::OneStop);
        ser->setFlowControl(QSerialPort::NoFlowControl);
        ser->setPortName(ui->Por_name->currentText());
        if(ser->open(QIODevice::ReadOnly))
            int reply = QMessageBox::warning(this,tr("تنويه"),tr("البوابة متصلة"),
                                          QMessageBox::Ok);
        else
            int reply = QMessageBox::warning(this,tr("تحذير"),tr("البوابة غير متصلة"),
                                          QMessageBox::Ok);
        connect(ser_tmr,SIGNAL(timeout()),this,SLOT(readrec()));
        ser_tmr->start(100);
        num_obs=0;
        counterr=0;
        obstacle=false;
        prev_obstacle=false;
        sub_counter=0;
    }
}
//***************************************
void MainWindow::name_port()
{
    std::vector< QString > list;
    Q_FOREACH(QSerialPortInfo port, QSerialPortInfo::availablePorts())
        list.push_back(port.portName());
    if(list.size()!=ui->Por_name->count())
    {
        ui->Por_name->clear();
        Q_FOREACH(QSerialPortInfo port, QSerialPortInfo::availablePorts())
            ui->Por_name->addItem(port.portName());
    }
}
//***************************************
void MainWindow::readrec()
{
    char buf_read1[4];
    ser->read(buf_read1,4);
    unsigned char buf_read[4];
    buf_read[0]=reinterpret_cast<unsigned char&>(buf_read1[0]);
    buf_read[1]=reinterpret_cast<unsigned char&>(buf_read1[1]);
    buf_read[2]=reinterpret_cast<unsigned char&>(buf_read1[2]);
    buf_read[3]=reinterpret_cast<unsigned char&>(buf_read1[3]);
    QString  valuee="";
    if(QString::number(buf_read[0]).toStdString()=="170")
        valuee=QString::number(buf_read[3]);
    if(QString::number(buf_read[1]).toStdString()=="170")
        valuee=QString::number(buf_read[0]);
    if(QString::number(buf_read[2]).toStdString()=="170")
        valuee=QString::number(buf_read[1]);
    if(QString::number(buf_read[3]).toStdString()=="170")
        valuee=QString::number(buf_read[2]);
    if(counterr%25==0)
    {
        sub_counter++;
        if(num_obs>20)
        {
            obstacle=true;
            img_num=ui->Pic_Num->text().toInt();
        }
        else
            obstacle=false;
        num_obs=0;
    }
    if(valuee=="221")
        num_obs++;

    if(sub_counter%10==0)
        prev_obstacle=false;
    if(obstacle)
        ui->ob_txt->setText("OBS");
    else
        ui->ob_txt->setText("NON");
    counterr++;
    ser->clear();

}
//***************************************
void MainWindow::on_pushButton_6_clicked()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QString query1 ="select * from Plate";
    if(ui->checkBox_9->isChecked())
    {
        QString city = Cities_ar[ui->comboBox_2->currentIndex()];
        query1+= " where City='"+city+"'";
    }
    if(ui->checkBox_7->isChecked())
    {
        QDateTime dateFrom = ui->dateTimeEdit->dateTime();
        QDateTime dateTo = ui->dateTimeEdit_2->dateTime();
        if(!query1.contains("where"))
            query1+= " where Date between '"+dateFrom.toString(" MM/dd/yyyy - HH:mm ")+"' and '"+dateTo.toString(" MM/dd/yyyy - HH:mm ")+"'";
        else
            query1+= " and Date between '"+dateFrom.toString(" MM/dd/yyyy - HH:mm ")+"' and '"+dateTo.toString(" MM/dd/yyyy - HH:mm ")+"'";
    }
    qDebug() <<query1;
    qry.prepare(query1);
    if(qry.exec())
    {
        model->setQuery(qry);
        model->setHeaderData(1, Qt::Horizontal, tr("الرقم"));
        model->setHeaderData(2, Qt::Horizontal, tr("المحافظة"));
        model->setHeaderData(3, Qt::Horizontal, tr("وقت الدخول"));
        model->setHeaderData(5, Qt::Horizontal, tr("دقة"));
        model->setHeaderData(6, Qt::Horizontal, tr("د.محافظة"));
        ui->tableView->setModel(model);
        ui->tableView->setColumnHidden(4, true);
        ui->tableView->setColumnHidden(0, true);
        ui->tableView->setColumnHidden(7, true);
        ui->tableView->setColumnWidth(1, (ui->tableView->width()/5)-5);
        ui->tableView->setColumnWidth(2, (ui->tableView->width()/5)-5);
        ui->tableView->setColumnWidth(3, (ui->tableView->width()/4)-5);
        ui->tableView->setColumnWidth(5, (ui->tableView->width()/6)-5);
        ui->tableView->setColumnWidth(6, (ui->tableView->width()/6)-5);
    }
    else
    {
         qDebug() << "select Car error:  "
                  << qry.lastError();
    }
}
//***************************************
void MainWindow::on_pushButton_4_clicked()
{
    qry.prepare("select * from Plate");
    QSqlQueryModel *model = new QSqlQueryModel();
    if(qry.exec())
    {
        model->setQuery(qry);
        model->setHeaderData(1, Qt::Horizontal, tr("الرقم"));
        model->setHeaderData(2, Qt::Horizontal, tr("المحافظة"));
        model->setHeaderData(3, Qt::Horizontal, tr("وقت الدخول"));
        model->setHeaderData(5, Qt::Horizontal, tr("دقة"));
        model->setHeaderData(6, Qt::Horizontal, tr("د.محافظة"));
        ui->tableView->setModel(model);
        ui->tableView->setColumnHidden(4, true);
        ui->tableView->setColumnHidden(0, true);
        ui->tableView->setColumnHidden(7, true);
        ui->tableView->setColumnWidth(1, (ui->tableView->width()/5)-5);
        ui->tableView->setColumnWidth(2, (ui->tableView->width()/5)-5);
        ui->tableView->setColumnWidth(3, (ui->tableView->width()/4)-5);
        ui->tableView->setColumnWidth(5, (ui->tableView->width()/6)-5);
        ui->tableView->setColumnWidth(6, (ui->tableView->width()/6)-5);
        ui->label_4->clear();
    }
    else
    {
         qDebug() << "select Car error:  "
                  << qry.lastError();
    }

    ui->pushButton->setEnabled(false);
}
//***************************************
void MainWindow::on_pushButton_5_clicked()
{
    if(check_all)
    {
        ui->pushButton_5->setEnabled(false);
        ui->checkBox_8->setChecked(false);
        ui->pushButton->setEnabled(false);
        check_all=false;
        qry.prepare("delete from Plate");
        if(qry.exec())
        {
            QDir dir(home_folder+"\\database");
            dir.removeRecursively();
            qDebug() << "deleted";
            qry.prepare("select * from Plate");
            QSqlQueryModel *model = new QSqlQueryModel();
            if(qry.exec())
            {
                model->setQuery(qry);
                model->setHeaderData(1, Qt::Horizontal, tr("الرقم"));
                model->setHeaderData(2, Qt::Horizontal, tr("المحافظة"));
                model->setHeaderData(3, Qt::Horizontal, tr("وقت الدخول"));
                model->setHeaderData(5, Qt::Horizontal, tr("دقة"));
                model->setHeaderData(6, Qt::Horizontal, tr("د.محافظة"));
                ui->tableView->setModel(model);
                ui->tableView->setColumnHidden(4, true);
                ui->tableView->setColumnHidden(0, true);
                ui->tableView->setColumnHidden(7, true);
                ui->tableView->setColumnWidth(1, (ui->tableView->width()/5)-5);
                ui->tableView->setColumnWidth(2, (ui->tableView->width()/5)-5);
                ui->tableView->setColumnWidth(3, (ui->tableView->width()/4)-5);
                ui->tableView->setColumnWidth(5, (ui->tableView->width()/6)-5);
                ui->tableView->setColumnWidth(6, (ui->tableView->width()/6)-5);
                QDir().mkdir(home_folder+"\\database");
                ui->label_4->clear();
            }
            else
            {
                 qDebug() << "select Car error:  "
                          << qry.lastError();
            }
        }

    }
    else
    {
        qry.prepare("delete from Plate WHERE Id ='"+QString::number(id_del)+"'");
        if(qry.exec())
        {
            QFile file (image_value);
            file.remove();
            string image_valuee=image_value.toStdString();
            image_valuee.erase(image_valuee.end()-1);
            image_valuee.erase(image_valuee.end()-1);
            image_valuee.erase(image_valuee.end()-1);
            image_valuee.erase(image_valuee.end()-1);
            image_valuee.append("_all.jpg");
            QFile file1 (QString::fromStdString(image_valuee));
            file1.remove();
            qDebug() << "deleted";
            qry.prepare("select * from Plate");
            QSqlQueryModel *model = new QSqlQueryModel();
            if(qry.exec())
            {
                model->setQuery(qry);
                model->setHeaderData(1, Qt::Horizontal, tr("الرقم"));
                model->setHeaderData(2, Qt::Horizontal, tr("المحافظة"));
                model->setHeaderData(3, Qt::Horizontal, tr("وقت الدخول"));
                model->setHeaderData(5, Qt::Horizontal, tr("دقة"));
                model->setHeaderData(6, Qt::Horizontal, tr("د.محافظة"));
                ui->tableView->setModel(model);
                ui->tableView->setColumnHidden(4, true);
                ui->tableView->setColumnHidden(0, true);
                ui->tableView->setColumnHidden(7, true);
                ui->tableView->setColumnWidth(1, (ui->tableView->width()/5)-5);
                ui->tableView->setColumnWidth(2, (ui->tableView->width()/5)-5);
                ui->tableView->setColumnWidth(3, (ui->tableView->width()/4)-5);
                ui->tableView->setColumnWidth(5, (ui->tableView->width()/6)-5);
                ui->tableView->setColumnWidth(6, (ui->tableView->width()/6)-5);
                ui->label_4->clear();
            }
            else
            {
                 qDebug() << "select Car error:  "
                          << qry.lastError();
            }
        }
        else
            qDebug() << qry.lastError();
        ui->pushButton_5->setEnabled(false);
        ui->pushButton->setEnabled(false);
    }

}
//***************************************
void MainWindow::on_tableView_clicked(const QModelIndex &index)
{
    int row = index.row();
    ui->tableWidget->clear();
    ui->pushButton_5->setEnabled(true);
    ui->pushButton->setEnabled(true);
    image_value = index.sibling(row, 4).data().toString();
    image_value_all=index.sibling(row, 7).data().toString();
    city_id=index.sibling(row, 2).data().toString();
    numb_id=index.sibling(row, 1).data().toString();
    id_del= index.sibling(row, 0).data().toInt();
//    image_value =QDir::toNativeSeparators(image_value);
    std::string image_value_string = image_value.toStdString();
    qDebug()<<image_value;
    cv::Mat tmpp=cv::imread(image_value_string);
    if(tmpp.empty())
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::information(this, "خطأ", "الصورة غير موجودة",
                                         QMessageBox::Ok);
        return;
    }
    std::string image_value_all_string = image_value_all.toStdString();
    cv::Mat tmpp_all=cv::imread(image_value_all_string);
    if(tmpp_all.empty())
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::information(this, "خطأ", "الصورة غير موجودة1",
                                         QMessageBox::Ok);
        return;
    }
    CvSize ss;
    ss.width=(int)ui->label_4->size().width();
    ss.height=(int)ui->label_4->size().height();
    cv::resize(tmpp,tmpp,ss);
    QImage Qorg((uchar*)tmpp.data,tmpp.cols,tmpp.rows,tmpp.step,QImage::Format_RGB888);
    ui->label_4->setPixmap(QPixmap::fromImage(Qorg));
    ss.width=(int)ui->label_5->size().width();
    ss.height=(int)ui->label_5->size().height();
    cv::resize(tmpp_all,tmpp_all,ss);
    QImage Qorg1((uchar*)tmpp_all.data,tmpp_all.cols,tmpp_all.rows,tmpp_all.step,QImage::Format_RGB888);
    ui->label_5->setPixmap(QPixmap::fromImage(Qorg1));
}
//***************************************
void MainWindow::on_checkBox_8_clicked(bool checked)
{
    if(checked)
    {
        ui->pushButton_5->setEnabled(true);
        check_all=true;
    }
    else
    {
        ui->pushButton_5->setEnabled(false);        
        ui->pushButton->setEnabled(false);
        check_all=false;
    }
}
//***************************************
void MainWindow::on_checkBox_9_clicked(bool checked)
{
    if(checked)
    {
        ui->comboBox_2->setEnabled(true);
        ui->pushButton_6->setEnabled(true);
    }
    else
    {
        ui->comboBox_2->setEnabled(false);
        if(!ui->checkBox_7->isChecked())
            ui->pushButton_6->setEnabled(false);
    }
}
//***************************************
void MainWindow::on_checkBox_7_clicked(bool checked)
{
    if(checked)
    {
        ui->dateTimeEdit->setEnabled(true);
        ui->dateTimeEdit_2->setEnabled(true);
        ui->pushButton_6->setEnabled(true);
    }
    else
    {
        ui->dateTimeEdit->setEnabled(false);
        ui->dateTimeEdit_2->setEnabled(false);
        if(!ui->checkBox_9->isChecked())
            ui->pushButton_6->setEnabled(false);
    }
}
//***************************************
void MainWindow::on_time_interval_check_clicked(bool checked)
{
    if(checked)
        ui->interval_time_combo->setEnabled(true);
    else
        ui->interval_time_combo->setEnabled(false);
}
//***************************************
void MainWindow::int_finish()
{
   interval_proc=true;
}
//***************************************
bool MainWindow::find_cars_dnn(cv::Mat _Src)
{
    net_type1 net;
    shape_predictor sp;
    dlib::deserialize(home_folder.toStdString()+"\\mmod_front_and_rear_end_vehicle_detector.dat") >> net >> sp;
    matrix<rgb_pixel> img;
    cv_image<bgr_pixel> image(_Src);
    assign_image(img, image);
    for (auto&& d : net(img))
    {
       auto fd = sp(img,d);

       if (d.label == "rear")
           return false;
       else
           return true;
    }
}
//***************************************
void MainWindow::on_pushButton_clicked()
{
    QString cc;
    for(int l=0;l<14;l++)
    {
        if(city_id==Cities_ar[l])
        {
            cc=Cities_ID[l];
            break;
        }
    }
    QString url_str = "http://"+web_service_ip+"/MCRS/api/clearance/all?USERNAME=ggg&CARID=&CITYID="+cc+"&TABNO="+numb_id;
        HttpRequestInput input(url_str, "GET");

        HttpRequestWorker *worker = new HttpRequestWorker(this);
        connect(worker, SIGNAL(on_execution_finished(HttpRequestWorker*)), this, SLOT(handle_result(HttpRequestWorker*)));
        worker->execute(&input);
}
//***************************************
void MainWindow::handle_result(HttpRequestWorker *worker) {
    QString msg;

    if (worker->error_type == QNetworkReply::NoError)
    {
        // communication was successful
        msg = "Success - Response: " + worker->response;
        std::string ss=worker->response.toStdString();
//        std::cout<<ss<<std::endl;
        QStringList propertyNames;
        QStringList propertyKeys;
        QJsonDocument jsonResponse = QJsonDocument::fromJson(worker->response);
        QJsonObject jsonObject = jsonResponse.object();
        QJsonArray jsonArray = jsonObject["result"].toArray();
//        ui->tableWidget->setColumnCount(jsonArray.size() );
        int i=0;

        foreach (const QJsonValue & value, jsonArray)
        {
            QJsonObject obj = value.toObject();
            int size=obj["row"].toArray().size();
            std::cout<<size<<std::endl;
            foreach (const QJsonValue & value, obj["row"].toArray())
            {
                QJsonObject obj = value.toObject();
                if(obj["columnName"].toString()=="KIND")
                {
                    propertyNames.append("النوع");
                    propertyKeys.append(obj["columnValue"].toString());
                }
                else if(obj["columnName"].toString()=="MODEL")
                {
                    propertyNames.append("الموديل");
                    propertyKeys.append(obj["columnValue"].toString());
                }
                else if(obj["columnName"].toString()=="REGTYP")
                {
                    propertyNames.append("الفئة");
                    propertyKeys.append(obj["columnValue"].toString());
                }
                else if(obj["columnName"].toString()=="ENGINNO")
                {
                    propertyNames.append("رقم المحرك");
                    propertyKeys.append(obj["columnValue"].toString());
                }
                else if(obj["columnName"].toString()=="BODYNO")
                {
                    propertyNames.append("رقم الهيكل");
                    propertyKeys.append(obj["columnValue"].toString());
                }
                else if(obj["columnName"].toString()=="FUEL")
                {
                    propertyNames.append("نوع الوقود");
                    propertyKeys.append(obj["columnValue"].toString());
                }
                else if(obj["columnName"].toString()=="COLOR")
                {
                    propertyNames.append("اللون");
                    propertyKeys.append(obj["columnValue"].toString());
                }
                else if(obj["columnName"].toString()=="MAKER")
                {
                    propertyNames.append("الصانع");
                    propertyKeys.append(obj["columnValue"].toString());
                }
                else if(obj["columnName"].toString()=="REGEDATE")
                {
                    propertyNames.append("تاريخ التسجيل");
                    propertyKeys.append(obj["columnValue"].toString());
                }
                else if(obj["columnName"].toString()=="IS_CLEAR")
                {
                    propertyNames.append("خالية من المخالفات");
                    propertyKeys.append(obj["columnValue"].toString());
                }
            }
        }
        ui->tableWidget->setRowCount(10);
        ui->tableWidget->setColumnCount(jsonArray.size()+1);
        ui->tableWidget->setColumnWidth(0, (ui->tableWidget->width()/2)-5);
        ui->tableWidget->setColumnWidth(1, (ui->tableWidget->width()/2)-5);
        for( int i = 0 ; i < propertyKeys.size() ; i++ )
        {
            // First Column
            ui->tableWidget->setItem(  i, 0 , new QTableWidgetItem( propertyNames[i] ) );
            // Second Column
            ui->tableWidget->setItem( i , 1 , new QTableWidgetItem( propertyKeys[i] ) );
        }

    }
    else {
        // an error occurred
        msg = "Error: " + worker->error_str;
        QMessageBox::information(this, "", msg);
    }

}
