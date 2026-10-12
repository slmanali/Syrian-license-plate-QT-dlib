/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QDate>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateTimeEdit>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QTabWidget *tabWidget;
    QWidget *tab;
    QWidget *widget_3;
    QLabel *label_v3;
    QPushButton *pause_v3;
    QListWidget *listWidget_v3;
    QPushButton *Trigger_bt;
    QCheckBox *checkBox_5;
    QPushButton *open_v3;
    QPushButton *open_ip_camera3;
    QPushButton *stop_v3;
    QPushButton *record_camera_3;
    QWidget *widget;
    QLabel *label_v1;
    QPushButton *pause_v1;
    QPushButton *stop_v1;
    QPushButton *open_ip_camera1;
    QPushButton *open_v1;
    QPushButton *record_camera_1;
    QListWidget *listWidget_v1;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout_2;
    QCheckBox *checkBox_2;
    QCheckBox *checkBox;
    QCheckBox *dat_check;
    QWidget *gridLayoutWidget_2;
    QGridLayout *gridLayout_3;
    QComboBox *interval_time_combo;
    QCheckBox *time_interval_check;
    QWidget *formLayoutWidget;
    QFormLayout *formLayout_4;
    QLineEdit *lineEdit_2;
    QLineEdit *lineEdit_3;
    QLineEdit *lineEdit_5;
    QLineEdit *lineEdit_6;
    QWidget *widget_2;
    QLabel *label_v2;
    QCheckBox *checkBox_3;
    QLineEdit *ob_txt;
    QPushButton *pause_v2;
    QPushButton *record_camera_2;
    QPushButton *Serial_bT;
    QComboBox *Por_name;
    QLineEdit *Pic_Num;
    QPushButton *open_v2;
    QListWidget *listWidget_v2;
    QLineEdit *num_pic;
    QPushButton *stop_v2;
    QPushButton *open_ip_camera2;
    QCheckBox *checkBox_4;
    QWidget *tab_2;
    QCheckBox *checkBox_7;
    QLabel *label_6;
    QCheckBox *checkBox_8;
    QPushButton *pushButton_4;
    QCheckBox *checkBox_9;
    QPushButton *pushButton_6;
    QComboBox *comboBox_2;
    QDateTimeEdit *dateTimeEdit_2;
    QLabel *label_4;
    QPushButton *pushButton_5;
    QDateTimeEdit *dateTimeEdit;
    QLabel *label_7;
    QTableView *tableView;
    QWidget *additional_Details;
    QPushButton *pushButton;
    QTableWidget *tableWidget;
    QLabel *label_5;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1400, 900);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setMinimumSize(QSize(1400, 900));
        MainWindow->setMaximumSize(QSize(1400, 900));
        QIcon icon;
        icon.addFile(QStringLiteral(":/new/prefix1/17270235_1252166278236188_1196388094_n.ico"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setGeometry(QRect(0, 0, 1400, 900));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(tabWidget->sizePolicy().hasHeightForWidth());
        tabWidget->setSizePolicy(sizePolicy1);
        tabWidget->setTabShape(QTabWidget::Rounded);
        tabWidget->setElideMode(Qt::ElideNone);
        tabWidget->setUsesScrollButtons(true);
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        sizePolicy1.setHeightForWidth(tab->sizePolicy().hasHeightForWidth());
        tab->setSizePolicy(sizePolicy1);
        widget_3 = new QWidget(tab);
        widget_3->setObjectName(QStringLiteral("widget_3"));
        widget_3->setGeometry(QRect(930, 10, 421, 781));
        label_v3 = new QLabel(widget_3);
        label_v3->setObjectName(QStringLiteral("label_v3"));
        label_v3->setGeometry(QRect(0, 0, 400, 300));
        QPalette palette;
        QBrush brush(QColor(255, 255, 255, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush1(QColor(100, 100, 100, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Button, brush1);
        QBrush brush2(QColor(150, 150, 150, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Light, brush2);
        QBrush brush3(QColor(125, 125, 125, 255));
        brush3.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Midlight, brush3);
        QBrush brush4(QColor(50, 50, 50, 255));
        brush4.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Dark, brush4);
        QBrush brush5(QColor(66, 66, 66, 255));
        brush5.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Mid, brush5);
        palette.setBrush(QPalette::Active, QPalette::Text, brush);
        palette.setBrush(QPalette::Active, QPalette::BrightText, brush);
        palette.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        QBrush brush6(QColor(0, 0, 0, 255));
        brush6.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Base, brush6);
        palette.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette.setBrush(QPalette::Active, QPalette::Shadow, brush6);
        palette.setBrush(QPalette::Active, QPalette::AlternateBase, brush4);
        QBrush brush7(QColor(255, 255, 220, 255));
        brush7.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::ToolTipBase, brush7);
        palette.setBrush(QPalette::Active, QPalette::ToolTipText, brush6);
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::Midlight, brush3);
        palette.setBrush(QPalette::Inactive, QPalette::Dark, brush4);
        palette.setBrush(QPalette::Inactive, QPalette::Mid, brush5);
        palette.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette.setBrush(QPalette::Inactive, QPalette::BrightText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush6);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Shadow, brush6);
        palette.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush4);
        palette.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush7);
        palette.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush6);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::Midlight, brush3);
        palette.setBrush(QPalette::Disabled, QPalette::Dark, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::Mid, brush5);
        palette.setBrush(QPalette::Disabled, QPalette::Text, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::BrightText, brush);
        palette.setBrush(QPalette::Disabled, QPalette::ButtonText, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Shadow, brush6);
        palette.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush7);
        palette.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush6);
        label_v3->setPalette(palette);
        label_v3->setAutoFillBackground(true);
        pause_v3 = new QPushButton(widget_3);
        pause_v3->setObjectName(QStringLiteral("pause_v3"));
        pause_v3->setGeometry(QRect(20, 310, 41, 31));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/new/prefix1/Hopstarter-Button-Button-Pause.ico"), QSize(), QIcon::Normal, QIcon::Off);
        pause_v3->setIcon(icon1);
        pause_v3->setIconSize(QSize(24, 24));
        listWidget_v3 = new QListWidget(widget_3);
        listWidget_v3->setObjectName(QStringLiteral("listWidget_v3"));
        listWidget_v3->setGeometry(QRect(40, 390, 320, 341));
        listWidget_v3->setLineWidth(1);
        listWidget_v3->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
        Trigger_bt = new QPushButton(widget_3);
        Trigger_bt->setObjectName(QStringLiteral("Trigger_bt"));
        Trigger_bt->setGeometry(QRect(300, 310, 61, 31));
        checkBox_5 = new QCheckBox(widget_3);
        checkBox_5->setObjectName(QStringLiteral("checkBox_5"));
        checkBox_5->setGeometry(QRect(210, 360, 51, 17));
        open_v3 = new QPushButton(widget_3);
        open_v3->setObjectName(QStringLiteral("open_v3"));
        open_v3->setGeometry(QRect(170, 310, 51, 31));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/new/prefix1/Oxygen-Icons.org-Oxygen-Mimetypes-inode-directory.ico"), QSize(), QIcon::Normal, QIcon::Off);
        open_v3->setIcon(icon2);
        open_v3->setIconSize(QSize(32, 32));
        open_ip_camera3 = new QPushButton(widget_3);
        open_ip_camera3->setObjectName(QStringLiteral("open_ip_camera3"));
        open_ip_camera3->setGeometry(QRect(220, 310, 51, 31));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/new/prefix1/Icons-Land-Vista-Hardware-Devices-Security-Camera.ico"), QSize(), QIcon::Normal, QIcon::Off);
        open_ip_camera3->setIcon(icon3);
        open_ip_camera3->setIconSize(QSize(32, 32));
        stop_v3 = new QPushButton(widget_3);
        stop_v3->setObjectName(QStringLiteral("stop_v3"));
        stop_v3->setGeometry(QRect(70, 310, 41, 31));
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/new/prefix1/Hopstarter-Button-Button-Stop.ico"), QSize(), QIcon::Normal, QIcon::Off);
        stop_v3->setIcon(icon4);
        stop_v3->setIconSize(QSize(24, 24));
        record_camera_3 = new QPushButton(widget_3);
        record_camera_3->setObjectName(QStringLiteral("record_camera_3"));
        record_camera_3->setGeometry(QRect(100, 350, 51, 31));
        QIcon icon5;
        icon5.addFile(QStringLiteral(":/new/prefix1/Oxygen-Icons.org-Oxygen-Actions-media-record.ico"), QSize(), QIcon::Normal, QIcon::Off);
        record_camera_3->setIcon(icon5);
        record_camera_3->setIconSize(QSize(32, 32));
        label_v3->raise();
        pause_v3->raise();
        Trigger_bt->raise();
        checkBox_5->raise();
        open_v3->raise();
        open_ip_camera3->raise();
        stop_v3->raise();
        record_camera_3->raise();
        listWidget_v3->raise();
        widget = new QWidget(tab);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(10, 10, 441, 781));
        label_v1 = new QLabel(widget);
        label_v1->setObjectName(QStringLiteral("label_v1"));
        label_v1->setGeometry(QRect(10, 0, 400, 300));
        QPalette palette1;
        palette1.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette1.setBrush(QPalette::Active, QPalette::Button, brush1);
        palette1.setBrush(QPalette::Active, QPalette::Light, brush2);
        palette1.setBrush(QPalette::Active, QPalette::Midlight, brush3);
        palette1.setBrush(QPalette::Active, QPalette::Dark, brush4);
        palette1.setBrush(QPalette::Active, QPalette::Mid, brush5);
        palette1.setBrush(QPalette::Active, QPalette::Text, brush);
        palette1.setBrush(QPalette::Active, QPalette::BrightText, brush);
        palette1.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette1.setBrush(QPalette::Active, QPalette::Base, brush6);
        palette1.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette1.setBrush(QPalette::Active, QPalette::Shadow, brush6);
        palette1.setBrush(QPalette::Active, QPalette::AlternateBase, brush4);
        palette1.setBrush(QPalette::Active, QPalette::ToolTipBase, brush7);
        palette1.setBrush(QPalette::Active, QPalette::ToolTipText, brush6);
        palette1.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette1.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::Midlight, brush3);
        palette1.setBrush(QPalette::Inactive, QPalette::Dark, brush4);
        palette1.setBrush(QPalette::Inactive, QPalette::Mid, brush5);
        palette1.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::BrightText, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::Base, brush6);
        palette1.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette1.setBrush(QPalette::Inactive, QPalette::Shadow, brush6);
        palette1.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush4);
        palette1.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush7);
        palette1.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush6);
        palette1.setBrush(QPalette::Disabled, QPalette::WindowText, brush4);
        palette1.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette1.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::Midlight, brush3);
        palette1.setBrush(QPalette::Disabled, QPalette::Dark, brush4);
        palette1.setBrush(QPalette::Disabled, QPalette::Mid, brush5);
        palette1.setBrush(QPalette::Disabled, QPalette::Text, brush4);
        palette1.setBrush(QPalette::Disabled, QPalette::BrightText, brush);
        palette1.setBrush(QPalette::Disabled, QPalette::ButtonText, brush4);
        palette1.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette1.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        palette1.setBrush(QPalette::Disabled, QPalette::Shadow, brush6);
        palette1.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush1);
        palette1.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush7);
        palette1.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush6);
        label_v1->setPalette(palette1);
        label_v1->setAutoFillBackground(true);
        pause_v1 = new QPushButton(widget);
        pause_v1->setObjectName(QStringLiteral("pause_v1"));
        pause_v1->setGeometry(QRect(20, 310, 41, 31));
        pause_v1->setIcon(icon1);
        pause_v1->setIconSize(QSize(24, 24));
        stop_v1 = new QPushButton(widget);
        stop_v1->setObjectName(QStringLiteral("stop_v1"));
        stop_v1->setGeometry(QRect(90, 310, 41, 31));
        stop_v1->setIcon(icon4);
        stop_v1->setIconSize(QSize(24, 24));
        open_ip_camera1 = new QPushButton(widget);
        open_ip_camera1->setObjectName(QStringLiteral("open_ip_camera1"));
        open_ip_camera1->setGeometry(QRect(220, 310, 51, 31));
        open_ip_camera1->setIcon(icon3);
        open_ip_camera1->setIconSize(QSize(32, 32));
        open_v1 = new QPushButton(widget);
        open_v1->setObjectName(QStringLiteral("open_v1"));
        open_v1->setGeometry(QRect(170, 310, 51, 31));
        open_v1->setIcon(icon2);
        open_v1->setIconSize(QSize(32, 32));
        record_camera_1 = new QPushButton(widget);
        record_camera_1->setObjectName(QStringLiteral("record_camera_1"));
        record_camera_1->setGeometry(QRect(290, 310, 51, 31));
        record_camera_1->setIcon(icon5);
        record_camera_1->setIconSize(QSize(32, 32));
        listWidget_v1 = new QListWidget(widget);
        listWidget_v1->setObjectName(QStringLiteral("listWidget_v1"));
        listWidget_v1->setGeometry(QRect(10, 380, 320, 361));
        listWidget_v1->setLineWidth(1);
        listWidget_v1->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
        gridLayoutWidget = new QWidget(widget);
        gridLayoutWidget->setObjectName(QStringLiteral("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(340, 520, 91, 82));
        gridLayout_2 = new QGridLayout(gridLayoutWidget);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        checkBox_2 = new QCheckBox(gridLayoutWidget);
        checkBox_2->setObjectName(QStringLiteral("checkBox_2"));

        gridLayout_2->addWidget(checkBox_2, 0, 0, 1, 1);

        checkBox = new QCheckBox(gridLayoutWidget);
        checkBox->setObjectName(QStringLiteral("checkBox"));
        checkBox->setEnabled(false);
        checkBox->setChecked(true);

        gridLayout_2->addWidget(checkBox, 1, 0, 1, 1);

        dat_check = new QCheckBox(gridLayoutWidget);
        dat_check->setObjectName(QStringLiteral("dat_check"));
        dat_check->setEnabled(false);

        gridLayout_2->addWidget(dat_check, 2, 0, 1, 1);

        gridLayoutWidget_2 = new QWidget(widget);
        gridLayoutWidget_2->setObjectName(QStringLiteral("gridLayoutWidget_2"));
        gridLayoutWidget_2->setGeometry(QRect(340, 410, 93, 80));
        gridLayout_3 = new QGridLayout(gridLayoutWidget_2);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        gridLayout_3->setSizeConstraint(QLayout::SetFixedSize);
        gridLayout_3->setContentsMargins(0, 0, 0, 0);
        interval_time_combo = new QComboBox(gridLayoutWidget_2);
        interval_time_combo->setObjectName(QStringLiteral("interval_time_combo"));
        interval_time_combo->setEnabled(false);
        interval_time_combo->setLayoutDirection(Qt::RightToLeft);

        gridLayout_3->addWidget(interval_time_combo, 1, 0, 1, 1);

        time_interval_check = new QCheckBox(gridLayoutWidget_2);
        time_interval_check->setObjectName(QStringLiteral("time_interval_check"));

        gridLayout_3->addWidget(time_interval_check, 0, 0, 1, 1);

        formLayoutWidget = new QWidget(widget);
        formLayoutWidget->setObjectName(QStringLiteral("formLayoutWidget"));
        formLayoutWidget->setGeometry(QRect(340, 610, 91, 131));
        formLayout_4 = new QFormLayout(formLayoutWidget);
        formLayout_4->setSpacing(6);
        formLayout_4->setContentsMargins(11, 11, 11, 11);
        formLayout_4->setObjectName(QStringLiteral("formLayout_4"));
        formLayout_4->setContentsMargins(0, 0, 0, 0);
        lineEdit_2 = new QLineEdit(formLayoutWidget);
        lineEdit_2->setObjectName(QStringLiteral("lineEdit_2"));
        lineEdit_2->setReadOnly(true);

        formLayout_4->setWidget(0, QFormLayout::LabelRole, lineEdit_2);

        lineEdit_3 = new QLineEdit(formLayoutWidget);
        lineEdit_3->setObjectName(QStringLiteral("lineEdit_3"));
        lineEdit_3->setReadOnly(true);

        formLayout_4->setWidget(1, QFormLayout::LabelRole, lineEdit_3);

        lineEdit_5 = new QLineEdit(formLayoutWidget);
        lineEdit_5->setObjectName(QStringLiteral("lineEdit_5"));
        lineEdit_5->setReadOnly(true);

        formLayout_4->setWidget(2, QFormLayout::LabelRole, lineEdit_5);

        lineEdit_6 = new QLineEdit(formLayoutWidget);
        lineEdit_6->setObjectName(QStringLiteral("lineEdit_6"));
        lineEdit_6->setReadOnly(true);

        formLayout_4->setWidget(3, QFormLayout::LabelRole, lineEdit_6);

        widget_2 = new QWidget(tab);
        widget_2->setObjectName(QStringLiteral("widget_2"));
        widget_2->setEnabled(false);
        widget_2->setGeometry(QRect(470, 10, 421, 781));
        label_v2 = new QLabel(widget_2);
        label_v2->setObjectName(QStringLiteral("label_v2"));
        label_v2->setGeometry(QRect(0, 0, 400, 300));
        QPalette palette2;
        palette2.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette2.setBrush(QPalette::Active, QPalette::Button, brush1);
        palette2.setBrush(QPalette::Active, QPalette::Light, brush2);
        palette2.setBrush(QPalette::Active, QPalette::Midlight, brush3);
        palette2.setBrush(QPalette::Active, QPalette::Dark, brush4);
        palette2.setBrush(QPalette::Active, QPalette::Mid, brush5);
        palette2.setBrush(QPalette::Active, QPalette::Text, brush);
        palette2.setBrush(QPalette::Active, QPalette::BrightText, brush);
        palette2.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette2.setBrush(QPalette::Active, QPalette::Base, brush6);
        palette2.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette2.setBrush(QPalette::Active, QPalette::Shadow, brush6);
        palette2.setBrush(QPalette::Active, QPalette::AlternateBase, brush4);
        palette2.setBrush(QPalette::Active, QPalette::ToolTipBase, brush7);
        palette2.setBrush(QPalette::Active, QPalette::ToolTipText, brush6);
        palette2.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette2.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette2.setBrush(QPalette::Inactive, QPalette::Midlight, brush3);
        palette2.setBrush(QPalette::Inactive, QPalette::Dark, brush4);
        palette2.setBrush(QPalette::Inactive, QPalette::Mid, brush5);
        palette2.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::BrightText, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::Base, brush6);
        palette2.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette2.setBrush(QPalette::Inactive, QPalette::Shadow, brush6);
        palette2.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush4);
        palette2.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush7);
        palette2.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush6);
        palette2.setBrush(QPalette::Disabled, QPalette::WindowText, brush4);
        palette2.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette2.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette2.setBrush(QPalette::Disabled, QPalette::Midlight, brush3);
        palette2.setBrush(QPalette::Disabled, QPalette::Dark, brush4);
        palette2.setBrush(QPalette::Disabled, QPalette::Mid, brush5);
        palette2.setBrush(QPalette::Disabled, QPalette::Text, brush4);
        palette2.setBrush(QPalette::Disabled, QPalette::BrightText, brush);
        palette2.setBrush(QPalette::Disabled, QPalette::ButtonText, brush4);
        palette2.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette2.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        palette2.setBrush(QPalette::Disabled, QPalette::Shadow, brush6);
        palette2.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush1);
        palette2.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush7);
        palette2.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush6);
        label_v2->setPalette(palette2);
        label_v2->setAutoFillBackground(true);
        checkBox_3 = new QCheckBox(widget_2);
        checkBox_3->setObjectName(QStringLiteral("checkBox_3"));
        checkBox_3->setGeometry(QRect(270, 320, 51, 17));
        ob_txt = new QLineEdit(widget_2);
        ob_txt->setObjectName(QStringLiteral("ob_txt"));
        ob_txt->setGeometry(QRect(105, 315, 51, 20));
        pause_v2 = new QPushButton(widget_2);
        pause_v2->setObjectName(QStringLiteral("pause_v2"));
        pause_v2->setGeometry(QRect(10, 310, 41, 31));
        pause_v2->setIcon(icon1);
        pause_v2->setIconSize(QSize(24, 24));
        record_camera_2 = new QPushButton(widget_2);
        record_camera_2->setObjectName(QStringLiteral("record_camera_2"));
        record_camera_2->setGeometry(QRect(340, 345, 51, 31));
        record_camera_2->setIcon(icon5);
        record_camera_2->setIconSize(QSize(32, 32));
        Serial_bT = new QPushButton(widget_2);
        Serial_bT->setObjectName(QStringLiteral("Serial_bT"));
        Serial_bT->setGeometry(QRect(270, 345, 51, 31));
        QIcon icon6;
        icon6.addFile(QStringLiteral(":/new/prefix1/Fatcow-Farm-Fresh-Port.ico"), QSize(), QIcon::Normal, QIcon::Off);
        Serial_bT->setIcon(icon6);
        Serial_bT->setIconSize(QSize(32, 32));
        Por_name = new QComboBox(widget_2);
        Por_name->setObjectName(QStringLiteral("Por_name"));
        Por_name->setGeometry(QRect(40, 350, 111, 22));
        Pic_Num = new QLineEdit(widget_2);
        Pic_Num->setObjectName(QStringLiteral("Pic_Num"));
        Pic_Num->setGeometry(QRect(160, 350, 31, 20));
        Pic_Num->setAlignment(Qt::AlignCenter);
        open_v2 = new QPushButton(widget_2);
        open_v2->setObjectName(QStringLiteral("open_v2"));
        open_v2->setGeometry(QRect(160, 310, 51, 31));
        open_v2->setIcon(icon2);
        open_v2->setIconSize(QSize(32, 32));
        listWidget_v2 = new QListWidget(widget_2);
        listWidget_v2->setObjectName(QStringLiteral("listWidget_v2"));
        listWidget_v2->setGeometry(QRect(60, 380, 320, 351));
        listWidget_v2->setLineWidth(1);
        listWidget_v2->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
        num_pic = new QLineEdit(widget_2);
        num_pic->setObjectName(QStringLiteral("num_pic"));
        num_pic->setGeometry(QRect(200, 350, 51, 20));
        stop_v2 = new QPushButton(widget_2);
        stop_v2->setObjectName(QStringLiteral("stop_v2"));
        stop_v2->setGeometry(QRect(60, 310, 41, 31));
        stop_v2->setIcon(icon4);
        stop_v2->setIconSize(QSize(24, 24));
        open_ip_camera2 = new QPushButton(widget_2);
        open_ip_camera2->setObjectName(QStringLiteral("open_ip_camera2"));
        open_ip_camera2->setGeometry(QRect(210, 310, 51, 31));
        open_ip_camera2->setIcon(icon3);
        open_ip_camera2->setIconSize(QSize(32, 32));
        checkBox_4 = new QCheckBox(widget_2);
        checkBox_4->setObjectName(QStringLiteral("checkBox_4"));
        checkBox_4->setGeometry(QRect(335, 320, 91, 17));
        checkBox_4->setChecked(true);
        label_v2->raise();
        checkBox_3->raise();
        ob_txt->raise();
        pause_v2->raise();
        record_camera_2->raise();
        Serial_bT->raise();
        Por_name->raise();
        Pic_Num->raise();
        open_v2->raise();
        num_pic->raise();
        stop_v2->raise();
        open_ip_camera2->raise();
        checkBox_4->raise();
        listWidget_v2->raise();
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        sizePolicy1.setHeightForWidth(tab_2->sizePolicy().hasHeightForWidth());
        tab_2->setSizePolicy(sizePolicy1);
        checkBox_7 = new QCheckBox(tab_2);
        checkBox_7->setObjectName(QStringLiteral("checkBox_7"));
        checkBox_7->setGeometry(QRect(1220, 250, 111, 31));
        checkBox_7->setLayoutDirection(Qt::RightToLeft);
        label_6 = new QLabel(tab_2);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(1155, 250, 40, 31));
        checkBox_8 = new QCheckBox(tab_2);
        checkBox_8->setObjectName(QStringLiteral("checkBox_8"));
        checkBox_8->setGeometry(QRect(1240, 70, 91, 31));
        checkBox_8->setLayoutDirection(Qt::RightToLeft);
        pushButton_4 = new QPushButton(tab_2);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));
        pushButton_4->setGeometry(QRect(990, 320, 71, 61));
        QIcon icon7;
        icon7.addFile(QStringLiteral(":/new/prefix1/Icons8-Ios7-Programming-Show-Property.ico"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_4->setIcon(icon7);
        pushButton_4->setIconSize(QSize(48, 48));
        checkBox_9 = new QCheckBox(tab_2);
        checkBox_9->setObjectName(QStringLiteral("checkBox_9"));
        checkBox_9->setGeometry(QRect(1250, 190, 81, 31));
        checkBox_9->setLayoutDirection(Qt::RightToLeft);
        pushButton_6 = new QPushButton(tab_2);
        pushButton_6->setObjectName(QStringLiteral("pushButton_6"));
        pushButton_6->setGeometry(QRect(800, 320, 71, 61));
        QIcon icon8;
        icon8.addFile(QStringLiteral(":/new/prefix1/Oxygen-Icons.org-Oxygen-Actions-view-filter.ico"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_6->setIcon(icon8);
        pushButton_6->setIconSize(QSize(48, 48));
        comboBox_2 = new QComboBox(tab_2);
        comboBox_2->setObjectName(QStringLiteral("comboBox_2"));
        comboBox_2->setEnabled(false);
        comboBox_2->setGeometry(QRect(830, 190, 241, 31));
        comboBox_2->setLayoutDirection(Qt::RightToLeft);
        comboBox_2->setMaxVisibleItems(15);
        comboBox_2->setSizeAdjustPolicy(QComboBox::AdjustToContentsOnFirstShow);
        comboBox_2->setModelColumn(0);
        dateTimeEdit_2 = new QDateTimeEdit(tab_2);
        dateTimeEdit_2->setObjectName(QStringLiteral("dateTimeEdit_2"));
        dateTimeEdit_2->setEnabled(false);
        dateTimeEdit_2->setGeometry(QRect(750, 250, 181, 31));
        dateTimeEdit_2->setLayoutDirection(Qt::RightToLeft);
        dateTimeEdit_2->setAlignment(Qt::AlignCenter);
        dateTimeEdit_2->setDate(QDate(2017, 1, 1));
        label_4 = new QLabel(tab_2);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(710, 30, 271, 111));
        label_4->setAutoFillBackground(true);
        label_4->setFrameShape(QFrame::Box);
        label_4->setFrameShadow(QFrame::Sunken);
        label_4->setLineWidth(1);
        label_4->setMidLineWidth(1);
        pushButton_5 = new QPushButton(tab_2);
        pushButton_5->setObjectName(QStringLiteral("pushButton_5"));
        pushButton_5->setGeometry(QRect(1190, 320, 71, 61));
        QIcon icon9;
        icon9.addFile(QStringLiteral(":/new/prefix1/Blackvariant-Button-Ui-System-Apps-X11.ico"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_5->setIcon(icon9);
        pushButton_5->setIconSize(QSize(48, 48));
        dateTimeEdit = new QDateTimeEdit(tab_2);
        dateTimeEdit->setObjectName(QStringLiteral("dateTimeEdit"));
        dateTimeEdit->setEnabled(false);
        dateTimeEdit->setGeometry(QRect(980, 250, 181, 31));
        dateTimeEdit->setFocusPolicy(Qt::WheelFocus);
        dateTimeEdit->setLayoutDirection(Qt::RightToLeft);
        dateTimeEdit->setAlignment(Qt::AlignCenter);
        dateTimeEdit->setButtonSymbols(QAbstractSpinBox::PlusMinus);
        dateTimeEdit->setDate(QDate(2017, 1, 1));
        label_7 = new QLabel(tab_2);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(935, 250, 40, 31));
        tableView = new QTableView(tab_2);
        tableView->setObjectName(QStringLiteral("tableView"));
        tableView->setGeometry(QRect(0, 7, 671, 501));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(11);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(tableView->sizePolicy().hasHeightForWidth());
        tableView->setSizePolicy(sizePolicy2);
        tableView->setMaximumSize(QSize(16777213, 16777215));
        tableView->setLayoutDirection(Qt::LeftToRight);
        tableView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        tableView->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
        tableView->setSelectionMode(QAbstractItemView::SingleSelection);
        tableView->verticalHeader()->setVisible(false);
        additional_Details = new QWidget(tab_2);
        additional_Details->setObjectName(QStringLiteral("additional_Details"));
        additional_Details->setGeometry(QRect(680, 400, 691, 391));
        QPalette palette3;
        palette3.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush8(QColor(59, 68, 21, 255));
        brush8.setStyle(Qt::SolidPattern);
        palette3.setBrush(QPalette::Active, QPalette::Button, brush8);
        QBrush brush9(QColor(88, 102, 31, 255));
        brush9.setStyle(Qt::SolidPattern);
        palette3.setBrush(QPalette::Active, QPalette::Light, brush9);
        QBrush brush10(QColor(73, 85, 26, 255));
        brush10.setStyle(Qt::SolidPattern);
        palette3.setBrush(QPalette::Active, QPalette::Midlight, brush10);
        QBrush brush11(QColor(29, 34, 10, 255));
        brush11.setStyle(Qt::SolidPattern);
        palette3.setBrush(QPalette::Active, QPalette::Dark, brush11);
        QBrush brush12(QColor(39, 45, 14, 255));
        brush12.setStyle(Qt::SolidPattern);
        palette3.setBrush(QPalette::Active, QPalette::Mid, brush12);
        palette3.setBrush(QPalette::Active, QPalette::Text, brush);
        palette3.setBrush(QPalette::Active, QPalette::BrightText, brush);
        palette3.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette3.setBrush(QPalette::Active, QPalette::Base, brush6);
        palette3.setBrush(QPalette::Active, QPalette::Window, brush8);
        palette3.setBrush(QPalette::Active, QPalette::Shadow, brush6);
        palette3.setBrush(QPalette::Active, QPalette::AlternateBase, brush11);
        palette3.setBrush(QPalette::Active, QPalette::ToolTipBase, brush7);
        palette3.setBrush(QPalette::Active, QPalette::ToolTipText, brush6);
        palette3.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::Button, brush8);
        palette3.setBrush(QPalette::Inactive, QPalette::Light, brush9);
        palette3.setBrush(QPalette::Inactive, QPalette::Midlight, brush10);
        palette3.setBrush(QPalette::Inactive, QPalette::Dark, brush11);
        palette3.setBrush(QPalette::Inactive, QPalette::Mid, brush12);
        palette3.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::BrightText, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::Base, brush6);
        palette3.setBrush(QPalette::Inactive, QPalette::Window, brush8);
        palette3.setBrush(QPalette::Inactive, QPalette::Shadow, brush6);
        palette3.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush11);
        palette3.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush7);
        palette3.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush6);
        palette3.setBrush(QPalette::Disabled, QPalette::WindowText, brush11);
        palette3.setBrush(QPalette::Disabled, QPalette::Button, brush8);
        palette3.setBrush(QPalette::Disabled, QPalette::Light, brush9);
        palette3.setBrush(QPalette::Disabled, QPalette::Midlight, brush10);
        palette3.setBrush(QPalette::Disabled, QPalette::Dark, brush11);
        palette3.setBrush(QPalette::Disabled, QPalette::Mid, brush12);
        palette3.setBrush(QPalette::Disabled, QPalette::Text, brush11);
        palette3.setBrush(QPalette::Disabled, QPalette::BrightText, brush);
        palette3.setBrush(QPalette::Disabled, QPalette::ButtonText, brush11);
        palette3.setBrush(QPalette::Disabled, QPalette::Base, brush8);
        palette3.setBrush(QPalette::Disabled, QPalette::Window, brush8);
        palette3.setBrush(QPalette::Disabled, QPalette::Shadow, brush6);
        palette3.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush8);
        palette3.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush7);
        palette3.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush6);
        additional_Details->setPalette(palette3);
        additional_Details->setContextMenuPolicy(Qt::NoContextMenu);
        additional_Details->setLayoutDirection(Qt::RightToLeft);
        additional_Details->setAutoFillBackground(true);
        pushButton = new QPushButton(additional_Details);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(0, 0, 691, 30));
        QIcon icon10;
        icon10.addFile(QStringLiteral(":/new/prefix1/Icons8-Windows-8-Messaging-More.ico"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton->setIcon(icon10);
        pushButton->setIconSize(QSize(80, 80));
        tableWidget = new QTableWidget(additional_Details);
        tableWidget->setObjectName(QStringLiteral("tableWidget"));
        tableWidget->setGeometry(QRect(0, 30, 691, 361));
        tableWidget->horizontalHeader()->setVisible(false);
        tableWidget->verticalHeader()->setVisible(false);
        label_5 = new QLabel(tab_2);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(30, 540, 621, 221));
        label_5->setAutoFillBackground(true);
        label_5->setFrameShape(QFrame::Box);
        label_5->setFrameShadow(QFrame::Sunken);
        label_5->setLineWidth(1);
        label_5->setMidLineWidth(1);
        tabWidget->addTab(tab_2, QString());
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1400, 25));
        menuBar->setMouseTracking(false);
        menuBar->setFocusPolicy(Qt::TabFocus);
        menuBar->setAcceptDrops(false);
#ifndef QT_NO_TOOLTIP
        menuBar->setToolTip(QStringLiteral(""));
#endif // QT_NO_TOOLTIP
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
#ifndef QT_NO_TOOLTIP
        mainToolBar->setToolTip(QStringLiteral(""));
#endif // QT_NO_TOOLTIP
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
#ifndef QT_NO_TOOLTIP
        statusBar->setToolTip(QStringLiteral(""));
#endif // QT_NO_TOOLTIP
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);
        comboBox_2->setCurrentIndex(-1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "\330\247\331\204\330\252\330\271\330\261\331\201 \330\271\331\204\331\211 \331\204\331\210\330\255\330\247\330\252 \330\247\331\204\330\263\331\212\330\247\330\261\330\247\330\252", Q_NULLPTR));
        label_v3->setText(QString());
        pause_v3->setText(QString());
        Trigger_bt->setText(QApplication::translate("MainWindow", "Trigger", Q_NULLPTR));
        checkBox_5->setText(QApplication::translate("MainWindow", "\330\245\330\270\331\207\330\247\330\261", Q_NULLPTR));
        open_v3->setText(QString());
        open_ip_camera3->setText(QString());
        stop_v3->setText(QString());
        record_camera_3->setText(QString());
        label_v1->setText(QString());
        pause_v1->setText(QString());
        stop_v1->setText(QString());
        open_ip_camera1->setText(QString());
        open_v1->setText(QString());
        record_camera_1->setText(QString());
        checkBox_2->setText(QApplication::translate("MainWindow", "\330\245\330\270\331\207\330\247\330\261", Q_NULLPTR));
        checkBox->setText(QApplication::translate("MainWindow", "\330\267\330\261\331\212\331\202\330\251 \330\253\330\247\331\206\331\212\330\251", Q_NULLPTR));
        dat_check->setText(QApplication::translate("MainWindow", "dnn", Q_NULLPTR));
        interval_time_combo->clear();
        interval_time_combo->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "10", Q_NULLPTR)
         << QApplication::translate("MainWindow", "15", Q_NULLPTR)
         << QApplication::translate("MainWindow", "20", Q_NULLPTR)
         << QApplication::translate("MainWindow", "25", Q_NULLPTR)
         << QApplication::translate("MainWindow", "30", Q_NULLPTR)
         << QApplication::translate("MainWindow", "35", Q_NULLPTR)
         << QApplication::translate("MainWindow", "40", Q_NULLPTR)
         << QApplication::translate("MainWindow", "45", Q_NULLPTR)
         << QApplication::translate("MainWindow", "50", Q_NULLPTR)
        );
        time_interval_check->setText(QApplication::translate("MainWindow", "\331\201\330\247\330\265\331\204 \330\262\331\205\331\206\331\212", Q_NULLPTR));
        label_v2->setText(QString());
        checkBox_3->setText(QApplication::translate("MainWindow", "\330\245\330\270\331\207\330\247\330\261", Q_NULLPTR));
        pause_v2->setText(QString());
        record_camera_2->setText(QString());
        Serial_bT->setText(QString());
        Pic_Num->setText(QApplication::translate("MainWindow", "5", Q_NULLPTR));
        open_v2->setText(QString());
        stop_v2->setText(QString());
        open_ip_camera2->setText(QString());
        checkBox_4->setText(QApplication::translate("MainWindow", "\330\267\330\261\331\212\331\202\330\251 \330\253\330\247\331\206\331\212\330\251", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("MainWindow", "\330\247\331\204\331\205\330\271\330\247\331\204\330\254\330\251", Q_NULLPTR));
        checkBox_7->setText(QApplication::translate("MainWindow", "\330\247\331\204\330\252\330\247\330\261\331\212\330\256 \331\210\330\247\331\204\331\210\331\202\330\252", Q_NULLPTR));
        label_6->setText(QApplication::translate("MainWindow", "   \331\205\331\206", Q_NULLPTR));
        checkBox_8->setText(QApplication::translate("MainWindow", "\330\252\330\255\330\257\331\212\330\257 \330\247\331\204\331\203\331\204", Q_NULLPTR));
        pushButton_4->setText(QString());
        checkBox_9->setText(QApplication::translate("MainWindow", "\330\247\331\204\331\205\330\255\330\247\331\201\330\270\330\251", Q_NULLPTR));
        pushButton_6->setText(QString());
        comboBox_2->clear();
        comboBox_2->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "\330\257\331\205\330\264\331\202", Q_NULLPTR)
         << QApplication::translate("MainWindow", "\330\257\330\261\330\271\330\247", Q_NULLPTR)
         << QApplication::translate("MainWindow", "\330\261\331\212\331\201 \330\257\331\205\330\264\331\202", Q_NULLPTR)
         << QApplication::translate("MainWindow", "\330\255\331\205\330\265", Q_NULLPTR)
         << QApplication::translate("MainWindow", "\330\255\331\205\330\247\330\251", Q_NULLPTR)
         << QApplication::translate("MainWindow", "\330\247\331\204\331\204\330\247\330\260\331\202\331\212\330\251", Q_NULLPTR)
         << QApplication::translate("MainWindow", "\330\267\330\261\330\267\331\210\330\263", Q_NULLPTR)
         << QApplication::translate("MainWindow", "\330\255\331\204\330\250", Q_NULLPTR)
         << QApplication::translate("MainWindow", "\330\247\331\204\330\261\331\202\330\251", Q_NULLPTR)
         << QApplication::translate("MainWindow", "\330\257\331\212\330\261 \330\247\331\204\330\262\331\210\330\261", Q_NULLPTR)
         << QApplication::translate("MainWindow", "\330\247\331\204\330\255\330\263\331\203\330\251", Q_NULLPTR)
         << QApplication::translate("MainWindow", "\330\247\331\204\330\263\331\210\331\212\330\257\330\247\330\241", Q_NULLPTR)
         << QApplication::translate("MainWindow", "\330\247\330\257\331\204\330\250", Q_NULLPTR)
         << QApplication::translate("MainWindow", "\330\247\331\204\331\202\331\206\331\212\330\267\330\261\330\251", Q_NULLPTR)
        );
        comboBox_2->setCurrentText(QString());
        label_4->setText(QString());
        pushButton_5->setText(QString());
        label_7->setText(QApplication::translate("MainWindow", "   \330\245\331\204\331\211", Q_NULLPTR));
        pushButton->setText(QString());
        label_5->setText(QString());
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("MainWindow", "\330\247\331\204\330\263\330\254\331\204", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
