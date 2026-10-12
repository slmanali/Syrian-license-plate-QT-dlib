/********************************************************************************
** Form generated from reading UI file 'ip_camera.ui'
**
** Created by: Qt User Interface Compiler version 5.9.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_IP_CAMERA_H
#define UI_IP_CAMERA_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QToolBox>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_IP_Camera
{
public:
    QPushButton *pushButton;
    QToolBox *toolBox;
    QWidget *page;
    QComboBox *comboBox;
    QLabel *label_5;
    QLabel *label_4;
    QLabel *label_3;
    QLineEdit *lineEdit;
    QLabel *label_2;
    QLineEdit *lineEdit_3;
    QLineEdit *lineEdit_2;
    QWidget *page_2;
    QLineEdit *lineEdit_4;
    QLineEdit *lineEdit_5;
    QLineEdit *lineEdit_6;
    QLineEdit *lineEdit_7;
    QLineEdit *lineEdit_8;
    QLabel *label;
    QLabel *label_10;
    QLabel *label_9;
    QLabel *label_8;
    QLabel *label_6;
    QLabel *label_7;
    QCheckBox *password_mode;

    void setupUi(QWidget *IP_Camera)
    {
        if (IP_Camera->objectName().isEmpty())
            IP_Camera->setObjectName(QStringLiteral("IP_Camera"));
        IP_Camera->resize(310, 390);
        QIcon icon;
        icon.addFile(QStringLiteral(":/new/prefix1/Icons-Land-Vista-Hardware-Devices-Security-Camera.ico"), QSize(), QIcon::Normal, QIcon::Off);
        IP_Camera->setWindowIcon(icon);
        pushButton = new QPushButton(IP_Camera);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(120, 330, 51, 41));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/new/prefix1/Graphicloads-100-Flat-2-Ok-2.ico"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton->setIcon(icon1);
        pushButton->setIconSize(QSize(32, 32));
        toolBox = new QToolBox(IP_Camera);
        toolBox->setObjectName(QStringLiteral("toolBox"));
        toolBox->setGeometry(QRect(10, 10, 291, 301));
        page = new QWidget();
        page->setObjectName(QStringLiteral("page"));
        page->setGeometry(QRect(0, 0, 291, 239));
        comboBox = new QComboBox(page);
        comboBox->setObjectName(QStringLiteral("comboBox"));
        comboBox->setGeometry(QRect(20, 190, 141, 31));
        label_5 = new QLabel(page);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(200, 180, 61, 51));
        label_5->setAlignment(Qt::AlignCenter);
        label_4 = new QLabel(page);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(190, 120, 81, 51));
        label_4->setAlignment(Qt::AlignCenter);
        label_3 = new QLabel(page);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(190, 60, 91, 51));
        label_3->setAlignment(Qt::AlignCenter);
        lineEdit = new QLineEdit(page);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setGeometry(QRect(20, 10, 141, 31));
        lineEdit->setAlignment(Qt::AlignCenter);
        label_2 = new QLabel(page);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(190, 0, 91, 51));
        label_2->setAlignment(Qt::AlignCenter);
        lineEdit_3 = new QLineEdit(page);
        lineEdit_3->setObjectName(QStringLiteral("lineEdit_3"));
        lineEdit_3->setGeometry(QRect(20, 130, 141, 31));
        lineEdit_3->setEchoMode(QLineEdit::Password);
        lineEdit_3->setAlignment(Qt::AlignCenter);
        lineEdit_2 = new QLineEdit(page);
        lineEdit_2->setObjectName(QStringLiteral("lineEdit_2"));
        lineEdit_2->setGeometry(QRect(20, 70, 141, 31));
        lineEdit_2->setAlignment(Qt::AlignCenter);
        toolBox->addItem(page, QString::fromUtf8("\330\247\331\204\330\247\331\201\330\252\330\261\330\247\330\266\331\212"));
        page_2 = new QWidget();
        page_2->setObjectName(QStringLiteral("page_2"));
        page_2->setGeometry(QRect(0, 0, 291, 239));
        lineEdit_4 = new QLineEdit(page_2);
        lineEdit_4->setObjectName(QStringLiteral("lineEdit_4"));
        lineEdit_4->setGeometry(QRect(20, 70, 141, 31));
        lineEdit_4->setAlignment(Qt::AlignCenter);
        lineEdit_5 = new QLineEdit(page_2);
        lineEdit_5->setObjectName(QStringLiteral("lineEdit_5"));
        lineEdit_5->setGeometry(QRect(20, 10, 141, 31));
        lineEdit_5->setAlignment(Qt::AlignCenter);
        lineEdit_6 = new QLineEdit(page_2);
        lineEdit_6->setObjectName(QStringLiteral("lineEdit_6"));
        lineEdit_6->setGeometry(QRect(20, 130, 141, 31));
        lineEdit_6->setEchoMode(QLineEdit::Password);
        lineEdit_6->setAlignment(Qt::AlignCenter);
        lineEdit_7 = new QLineEdit(page_2);
        lineEdit_7->setObjectName(QStringLiteral("lineEdit_7"));
        lineEdit_7->setGeometry(QRect(20, 180, 141, 25));
        lineEdit_7->setEchoMode(QLineEdit::Normal);
        lineEdit_7->setAlignment(Qt::AlignCenter);
        lineEdit_8 = new QLineEdit(page_2);
        lineEdit_8->setObjectName(QStringLiteral("lineEdit_8"));
        lineEdit_8->setGeometry(QRect(20, 210, 141, 25));
        lineEdit_8->setEchoMode(QLineEdit::Normal);
        lineEdit_8->setAlignment(Qt::AlignCenter);
        label = new QLabel(page_2);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(180, 190, 31, 16));
        label->setAlignment(Qt::AlignCenter);
        label_10 = new QLabel(page_2);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setGeometry(QRect(170, 220, 41, 16));
        label_10->setAlignment(Qt::AlignCenter);
        label_9 = new QLabel(page_2);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setGeometry(QRect(190, 120, 81, 51));
        label_9->setAlignment(Qt::AlignCenter);
        label_8 = new QLabel(page_2);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(210, 180, 61, 51));
        label_8->setAlignment(Qt::AlignCenter);
        label_6 = new QLabel(page_2);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(190, 0, 91, 51));
        label_6->setAlignment(Qt::AlignCenter);
        label_7 = new QLabel(page_2);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(190, 60, 91, 51));
        label_7->setAlignment(Qt::AlignCenter);
        password_mode = new QCheckBox(page_2);
        password_mode->setObjectName(QStringLiteral("password_mode"));
        password_mode->setGeometry(QRect(200, 160, 81, 20));
        password_mode->setContextMenuPolicy(Qt::NoContextMenu);
        password_mode->setLayoutDirection(Qt::RightToLeft);
        toolBox->addItem(page_2, QString::fromUtf8("\330\247\331\204\331\205\330\271\330\257\331\204"));

        retranslateUi(IP_Camera);

        toolBox->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(IP_Camera);
    } // setupUi

    void retranslateUi(QWidget *IP_Camera)
    {
        IP_Camera->setWindowTitle(QApplication::translate("IP_Camera", "\331\203\330\247\331\205\331\212\330\261\330\247\330\252 \330\247\331\204\330\264\330\250\331\203\330\251", Q_NULLPTR));
        pushButton->setText(QString());
        comboBox->clear();
        comboBox->insertItems(0, QStringList()
         << QApplication::translate("IP_Camera", "1280X960", Q_NULLPTR)
         << QApplication::translate("IP_Camera", "1920X960", Q_NULLPTR)
        );
        label_5->setText(QApplication::translate("IP_Camera", "\330\247\331\204\330\257\331\202\330\251", Q_NULLPTR));
        label_4->setText(QApplication::translate("IP_Camera", "\331\203\331\204\331\205\330\251 \330\247\331\204\330\263\330\261", Q_NULLPTR));
        label_3->setText(QApplication::translate("IP_Camera", "\330\247\330\263\331\205 \330\247\331\204\331\205\330\263\330\252\330\256\330\257\331\205", Q_NULLPTR));
        lineEdit->setText(QApplication::translate("IP_Camera", "172.16.140.1", Q_NULLPTR));
        label_2->setText(QApplication::translate("IP_Camera", "\330\247\331\204\330\271\331\206\331\210\330\247\331\206 \330\247\331\204\331\205\331\206\330\267\331\202\331\212", Q_NULLPTR));
        lineEdit_3->setText(QApplication::translate("IP_Camera", "12345", Q_NULLPTR));
        lineEdit_2->setText(QApplication::translate("IP_Camera", "admin", Q_NULLPTR));
        toolBox->setItemText(toolBox->indexOf(page), QApplication::translate("IP_Camera", "\330\247\331\204\330\247\331\201\330\252\330\261\330\247\330\266\331\212", Q_NULLPTR));
        lineEdit_4->setText(QApplication::translate("IP_Camera", "admin", Q_NULLPTR));
        lineEdit_5->setText(QApplication::translate("IP_Camera", "172.16.140.1", Q_NULLPTR));
        lineEdit_6->setText(QApplication::translate("IP_Camera", "12345", Q_NULLPTR));
        lineEdit_7->setText(QString());
        lineEdit_8->setText(QString());
        label->setText(QApplication::translate("IP_Camera", "\330\247\331\204\330\271\330\261\330\266", Q_NULLPTR));
        label_10->setText(QApplication::translate("IP_Camera", "\330\247\331\204\330\247\330\261\330\252\331\201\330\247\330\271", Q_NULLPTR));
        label_9->setText(QApplication::translate("IP_Camera", "\331\203\331\204\331\205\330\251 \330\247\331\204\330\263\330\261", Q_NULLPTR));
        label_8->setText(QApplication::translate("IP_Camera", "\330\247\331\204\330\257\331\202\330\251", Q_NULLPTR));
        label_6->setText(QApplication::translate("IP_Camera", "\330\247\331\204\330\271\331\206\331\210\330\247\331\206 \330\247\331\204\331\205\331\206\330\267\331\202\331\212", Q_NULLPTR));
        label_7->setText(QApplication::translate("IP_Camera", "\330\247\330\263\331\205 \330\247\331\204\331\205\330\263\330\252\330\256\330\257\331\205", Q_NULLPTR));
        password_mode->setText(QApplication::translate("IP_Camera", "\330\245\330\270\331\207\330\247\330\261", Q_NULLPTR));
        toolBox->setItemText(toolBox->indexOf(page_2), QApplication::translate("IP_Camera", "\330\247\331\204\331\205\330\271\330\257\331\204", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class IP_Camera: public Ui_IP_Camera {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IP_CAMERA_H
