#ifndef IP_CAMERA_H
#define IP_CAMERA_H

#include <QWidget>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>

namespace Ui {
class IP_Camera;
}

class IP_Camera : public QWidget
{
    Q_OBJECT

public:
    explicit IP_Camera(QWidget *parent = nullptr);
    ~IP_Camera();
    QString Ip;
    QString user;
    QString pass;
    bool my_closs;
    int width;
    int height;

signals:
    void cameraAccepted();

private slots:
    void on_pushButton_clicked();

    void on_comboBox_currentIndexChanged(int index);

    void on_password_mode_clicked(bool checked);

private:
    Ui::IP_Camera *ui;
};

#endif // IP_CAMERA_H
