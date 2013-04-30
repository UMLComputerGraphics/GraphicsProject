#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->numberOfParticlesSpinBox, SIGNAL(valueChanged(int)),
            this, SIGNAL(sigChangeNumberOfParticles(int)));
    connect(ui->freezeParticlesCheckBox, SIGNAL(toggled(bool)),
            this, SIGNAL(sigFreezeParticles(bool)));
    connect(ui->morphingEnabler, SIGNAL(toggled(bool)),
            this, SIGNAL(sigEnableMorphing(bool)));
    connect(ui->morphPercentageSlider, SIGNAL(valueChanged(int)),
            this, SIGNAL(sigMorphPercentage(int)));
    connect(ui->rayTracerEnabler, SIGNAL(toggled(bool)),
            this, SIGNAL(sigEnableRaytracing(bool)));
    connect(ui->particleSystemEnabler, SIGNAL(toggled(bool)),
            this, SIGNAL(sigEnableParticleSystem(bool)));

    connect(ui->WineBottleRadioButton, SIGNAL(clicked()),
            this, SIGNAL(sigMorphToWineBottle()));

    connect(ui->WhiskyBottleRadioButton, SIGNAL(clicked()),
            this, SIGNAL(sigMorphToWhiskyBottle()));
    connect(ui->maxAccelerationSlider, SIGNAL(valueChanged(int)),
            this, SIGNAL(sigSetMaxAcceleration(int)));
    connect(ui->frictionHorizontalSlider, SIGNAL(valueChanged(int)),
            this, SIGNAL(sigSetFrictionMagnitude(int)));
	connect(ui->speedHorizontalSlider, SIGNAL(valueChanged(int)),
            this, SIGNAL(sigCameraSpeed(int)));
    

    /* Attempt to get the 'connect' work the other way */
    //connect(this, SIGNAL(sigMorphPercentageOut(int)),
    //        ui->morphPercentageSlider, SLOT(setValue(int)));


}

MainWindow::~MainWindow()
{
    delete ui;
}
