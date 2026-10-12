#include "form.h"
#include "ui_form.h"

Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);
}

Form::~Form()
{
    delete ui;
}

void Form::update()
{
    QImage img2 = plate.scaled(ui->label->width(),ui->label->height(),Qt::KeepAspectRatio);
    ui->lineEdit->setText(number);
    ui->lineEdit_2->setText(city);
    ui->lineEdit_3->setText(QString::number(conf));
    ui->label->setPixmap(QPixmap::fromImage(img2));
}
