#pragma warning( push )
#pragma warning( disable : 193)
#include "mainwindow.h"
#include <QApplication>
#include <QDesktopWidget>
#include <fstream>
#include <QMessageBox>
#include <iostream>
#include <sstream>
#include <vector>
#include "md5.h"
#include "reg.h"

std::vector<char> encrypt(const std::string& toEncrypt) {
    char key = 'Q';
    std::vector<char> output;
    output.reserve(toEncrypt.size());

    for (const auto chr : toEncrypt){
        output.push_back(chr ^ key);
    }
    return output;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
/*    QString home_folderr=QCoreApplication::applicationDirPath();
//    home_folderr.replace(QString("/"), QString("\\"));
//    QString command= "wmic cpu get ProcessorId >"+home_folderr+"\\ProcessorId.txt" ;

//    QProcess consola;
//    #ifdef Q_OS_WIN
//        consola.start("cmd.exe /C " + command);
//    #else
//        consola.start(command);
//    #endif
//    consola.waitForFinished();
//    consola.waitForReadyRead();

//    command= "vol c:\>"+home_folderr+"\\vol.txt" ;
//    #ifdef Q_OS_WIN
//        consola.start("cmd.exe /C " + command);
//    #else
//        consola.start(command);
//    #endif
//    consola.waitForFinished();
//    consola.waitForReadyRead();

//    QFile inputFile(home_folderr+"\\ProcessorId.txt");
//    QString line;
//    if (inputFile.open(QIODevice::ReadOnly))
//    {
//       QTextStream in(&inputFile);
//       while (!in.atEnd())
//       {
//          line = in.readLine();
//       }
//       inputFile.close();
//    }

//    QString ID=line.left(16);
//    QFile inputFile1(home_folderr+"\\vol.txt");
//    if (inputFile1.open(QIODevice::ReadOnly))
//    {
//        QTextStream in(&inputFile1);
//        while (!in.atEnd())
//        {
//            line = in.readLine();
//        }
//        inputFile1.close();
//    }

//    QString vol=line.right(9);
//    vol.replace("-","");

//    QFile::remove(home_folderr+"\\ProcessorId.txt");
//    QFile::remove(home_folderr+"\\vol.txt");
//    std::vector<char> enc=encrypt(ID.toStdString()+vol.toStdString());
//    std::string str(enc.begin(),enc.end());
//    std::vector< int > buffer;
//    for (int i=0;i<str.length();i++)
//    {
//        buffer.push_back(str[i]);
//    }
//    string buf_text="";
//    for(int i=0;i<str.length();i++)
//    {
//        buf_text=buf_text+std::to_string(buffer[i]);
//    }
//    string enc_md5= md5(buf_text.c_str());
//    QFile file_reg (home_folderr+"\\reg.txt");

//    if(file_reg.exists())
//    {
//        if (file_reg.open(QIODevice::ReadOnly))
//        {
//            QTextStream in(&file_reg);
//            while (!in.atEnd())
//            {
//                line = in.readLine();
//            }
//            file_reg.close();
//        }
//        if(line.toStdString()==enc_md5)
//        {
//            w.show();
//            return a.exec();
//        }
//        else
//        {
//            QMessageBox::StandardButton reply;
//            reply = QMessageBox::information(NULL, "خطأ","غير مصرح",
//                                             QMessageBox::Ok);
//            return -1;
//        }
//    }
//    else
//    {
//        reg* ipp=new reg();

//        ipp->text= QString::fromStdString(buf_text);
//        ipp->setAttribute( Qt::WA_DeleteOnClose );
//        ipp->update();
//        ipp->showNormal();
//        return a.exec();
//    }*/

    QRect rec = QApplication::desktop()->screenGeometry();
    w.setMaximumWidth(rec.width());
    w.setMaximumHeight(rec.height());
    w.showMaximized();
    return a.exec();
}
