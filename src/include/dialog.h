#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    
private slots:
    void on_objLocationButton_clicked();

public slots:

private:
    Ui::Dialog *ui;

    cv::VideoCapture capWebcam;

    cv::Mat matOriginal;
    cv::Mat matProcessed;

    QImage qimgOriginal;
    QImage qimgProcessed;

    std::vector<cv::Vec3f> vecCircles;
    std::vector<cv::Vec3f>::iterator itrCircles;

    QTimer* tmrTimer;
};

#endif // DIALOG_H
