#include "reg.h"
#include "ui_reg.h"

reg::reg(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::reg)
{
    ui->setupUi(this);
}

reg::~reg()
{
    delete ui;
}

void reg::on_pushButton_clicked()
{
     this->close();
}

void reg::update()
{
    ui->lineEdit->setText(text);
}
