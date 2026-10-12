#ifndef REG_H
#define REG_H

#include <QWidget>

namespace Ui {
class reg;
}

class reg : public QWidget
{
    Q_OBJECT

signals:
       void WidgetClosed();
public:
    explicit reg(QWidget *parent = 0);
    ~reg();
    QString text;

private slots:
    void on_pushButton_clicked();

public slots:
    void  update();

private:
    Ui::reg *ui;
};

#endif // REG_H
