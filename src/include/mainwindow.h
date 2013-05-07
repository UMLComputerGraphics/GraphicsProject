#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

signals:
	/* General Settings */
	/* Engine */
	void sigEnableMorphing(bool isEnabled);
	void sigEnableRaytracing(bool isEnabled);
	void sigEnableParticleSystem(bool isEnabled);
    void sigEnableMorphMatching(bool isEnabled);
	
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
    void sigUserDefVecParams( void );
    void sigFlameVecParams( void );
    void sigFlameVecParams(double[3], double, float, float);
    void sigTornadoVecParams( void );
    void sigTornadoVecParams( float, float, float );
    void sigSetParticleLife( float, float );

    // void sigParticleFieldFunction(int index);

private slots:
    void on_addObjectButton_clicked();

    void on_particleFieldFunctionComboBox_currentIndexChanged(int index);

    //void on_userDefinedShowButton_clicked();

    void on_flameShowButton_clicked();
    void on_flameDefaultButton_clicked();

    void on_tornadoShowButton_clicked();

    void on_tornadoDefaultButton_clicked();

    void on_setLifespansButton_clicked();

    void on_defaultLifespansButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
