#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDialog>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    int getDefaultNumOfParticles();
    ~MainWindow();
    void setMorphPercentageOut(int pct);

public slots:
    void processFrameAndUpdateGUI();

private slots:
    /* VR stuff */
    void slotEnableVR(bool isEnabled);

signals:
	/* General Settings */
	/* Engine */
	void sigEnableMorphing(bool isEnabled);
	void sigEnableRaytracing(bool isEnabled);
	void sigEnableParticleSystem(bool isEnabled);
    void sigEnableMorphMatching(bool isEnabled);

    /*VR Stuff */
    void sigEnableVRCameraControl(bool isEnabled);
    void sigVRCameraCoordinates(float, float);
	
	/* Camera */
	void sigChangeAspectRatio(int value);
	void sigChangeCurrentView(int value);
	void sigSetMaxSpeed(int value);
	void sigSetMaxAcceleration(int value);
	void sigSetFrictionMagnitude(int value);
	void sigCameraSpeed(int value);
	
	/* Scene Graph */
	
    /* Morph stuff */
    void sigMorphPercentage(int value);
    void sigMorphPercentageOut(int value);
    void sigMorphToWineBottle(void);
    void sigMorphToWhiskyBottle(void);

    /* Particles */
	void sigChangeNumberOfParticles(int value);
	void sigFreezeParticles(bool isEnabled);
    void sigUserDefVecParams( std::string, std::string, std::string );
    void sigFlameVecParams( void );
    void sigFlameVecParams(float[3], double, float, float);
    void sigTornadoVecParams( void );
    void sigTornadoVecParams( float, float, float );
    void sigSetParticleLife( float, float );
    void sigMusicVolume( int );
    void sigPartColorFunc( int );

    // void sigParticleFieldFunction(int index);

private slots:
    void updateUdfMessage( QString );

    void on_addObjectButton_clicked();

    void on_particleFieldFunctionComboBox_currentIndexChanged(int index);

    void on_userDefinedShowButton_clicked();

    void on_flameShowButton_clicked();
    void on_flameDefaultButton_clicked();

    void on_VRXLocation_valueChanged(int value);

    void on_tornadoShowButton_clicked();

    void on_tornadoDefaultButton_clicked();

    void on_setLifespansButton_clicked();

    void on_defaultLifespansButton_clicked();

    void on_volumeSlider_sliderMoved(int position);

    void on_audioOnCheckbox_stateChanged(int arg1);

    void on_particleSystemEnabler_toggled_melted(bool checked);

    void on_userDefinedDefaultButton_clicked();

    void on_particleColorComboBox_currentIndexChanged(int index);

private:
    Ui::MainWindow *ui;

    cv::VideoCapture capWebcam;

    cv::Mat matOriginal;
    cv::Mat matProcessed;

    QImage qimgOriginal;
    QImage qimgProcessed;
    std::vector<cv::Vec3f> vecCircles;
    std::vector<cv::Vec3f>::iterator itrCircles;

    QTimer* tmrTimer;
};

#endif // MAINWINDOW_H
