/********************************************************************************
** Form generated from reading UI file 'reg.ui'
**
** Created by: Qt User Interface Compiler version 5.9.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REG_H
#define UI_REG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_reg
{
public:
    QLabel *label_2;
    QLineEdit *lineEdit;
    QLabel *label_3;
    QPushButton *pushButton;

    void setupUi(QWidget *reg)
    {
        if (reg->objectName().isEmpty())
            reg->setObjectName(QStringLiteral("reg"));
        reg->resize(421, 269);
        QIcon icon;
        icon.addFile(QStringLiteral(":/new/prefix1/17270235_1252166278236188_1196388094_n.ico"), QSize(), QIcon::Normal, QIcon::Off);
        reg->setWindowIcon(icon);
        label_2 = new QLabel(reg);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(310, 10, 91, 51));
        lineEdit = new QLineEdit(reg);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setGeometry(QRect(20, 21, 291, 41));
        lineEdit->setReadOnly(true);
        label_3 = new QLabel(reg);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(100, 90, 211, 91));
        label_3->setLayoutDirection(Qt::LeftToRight);
        label_3->setTextFormat(Qt::RichText);
        label_3->setScaledContents(false);
        label_3->setAlignment(Qt::AlignCenter);
        label_3->setWordWrap(false);
        pushButton = new QPushButton(reg);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(170, 220, 91, 31));

        retranslateUi(reg);

        QMetaObject::connectSlotsByName(reg);
    } // setupUi

    void retranslateUi(QWidget *reg)
    {
        reg->setWindowTitle(QApplication::translate("reg", "                             \330\247\331\204\330\261\330\254\330\247\330\241 \330\252\330\263\330\254\331\212\331\204 \330\247\331\204\331\205\331\206\330\252\330\254", Q_NULLPTR));
        label_2->setText(QApplication::translate("reg", "\330\247\331\204\330\261\331\202\331\205 \330\247\331\204\330\252\330\263\331\204\330\263\331\204\331\212:", Q_NULLPTR));
        label_3->setText(QApplication::translate("reg", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"center\" style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">\330\247\331\204\330\250\330\261\331\206\330\247\331\205\330\254 \330\272\331\212\330\261 \331\205\331\201\330\271\331\204 </p>\n"
"<p align=\"center\" style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">\330\247\331\204\330\261\330\254\330\247\330\241 \330\245\330\261\330\263\330\247\331\204 \330\247\331\204\330\261\331\202\331\205 \330\247\331\204\330\252\330\263\331\204\330\263\331\204\331\212 \330\245\331\204\331\211</p>\n"
"<p align=\"center\" style=\" margin-top:12p"
                        "x; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">slmanaliR@yahoo.com</p>\n"
"<p align=\"center\" style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">\330\261\331\202\331\205 \330\247\331\204\331\207\330\247\330\252\331\201</p>\n"
"<p align=\"center\" style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">0956670561<br /></p></body></html>", Q_NULLPTR));
        pushButton->setText(QApplication::translate("reg", "\331\205\331\210\330\247\331\201\331\202", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class reg: public Ui_reg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REG_H
