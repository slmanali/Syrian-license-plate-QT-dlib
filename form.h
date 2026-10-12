#ifndef FORM_H
#define FORM_H

#include <QWidget>

namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT

signals:
       void WidgetClosed();
public:
    explicit Form(QWidget *parent = 0);
    ~Form();
    QString number;
    QString city;
    QImage plate;
    double conf;

public slots:
    void  update();

private:
    Ui::Form *ui;
};

#endif // FORM_H
