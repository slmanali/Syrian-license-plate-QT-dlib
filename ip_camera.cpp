#include "ip_camera.h"
#include "ui_ip_camera.h"

IP_Camera::IP_Camera(QWidget *parent) :
    QWidget(parent, Qt::Window),
    ui(new Ui::IP_Camera)
{
    ui->setupUi(this);
    ui->toolBox->setCurrentWidget(ui->page_2);
    ui->page->setVisible(false);
    my_closs=false;
    width=1280;
    height=960;
}

IP_Camera::~IP_Camera()
{
    delete ui;
}

void IP_Camera::on_pushButton_clicked()
{
    if(ui->toolBox->currentIndex()==0)
    {
        Ip=ui->lineEdit->text();
        user=ui->lineEdit_2->text();
        pass=ui->lineEdit_3->text();
        if(user!=QString::fromStdString("admin") || pass!=QString::fromStdString("12345"))
        {
            QMessageBox::StandardButton reply;

            reply = QMessageBox::information(this, " خطأ", "ادخال خاطئ",
                                          QMessageBox::Ok);
            return;
        }
        my_closs=true;
        emit cameraAccepted();
        this->close();
    }
    else
    {
        Ip=ui->lineEdit_5->text();
        QString home_folderr=QCoreApplication::applicationDirPath();
        QString line1,line,line0;
        QFile inputFile(home_folderr+"/info.txt");
        if (inputFile.open(QIODevice::ReadOnly))
        {
           QTextStream in(&inputFile);
           while (!in.atEnd())
           {
               line0=in.readLine();
               if(line0==Ip)
               {
                   line = in.readLine();
                   line1 = in.readLine();
                   break;
               }
           }
                   inputFile.close();
        }
        else
        {
            QMessageBox::StandardButton reply;
            reply = QMessageBox::information(this, "خطأ", "info.txt لايوجد الرجاء الاتصال بالشركة",
                                             QMessageBox::Ok);
        }
        user=ui->lineEdit_4->text();
        pass=ui->lineEdit_6->text();
        if(user!=line || pass!=line1)
        {
            QMessageBox::StandardButton reply;

            reply = QMessageBox::information(this, " خطأ", "ادخال خاطئ",
                                          QMessageBox::Ok);
            return;
        }
        width=ui->lineEdit_7->text().toInt();
        height=ui->lineEdit_8->text().toInt();
        my_closs=true;
        emit cameraAccepted();
        this->close();
    }

}

void IP_Camera::on_comboBox_currentIndexChanged(int index)
{
    if(index==0)
    {
        width=1280;
        height=960;
    }
    if(index==1)
    {
        width=1920;
        height=960;
    }
}

void IP_Camera::on_password_mode_clicked(bool checked)
{
    if(checked)
    {
        ui->password_mode->setText("إخفاء");
        ui->lineEdit_6->setEchoMode(QLineEdit::Normal);
    }
    else
    {
        ui->password_mode->setText("إظهار");
        ui->lineEdit_6->setEchoMode(QLineEdit::Password);
    }
}
