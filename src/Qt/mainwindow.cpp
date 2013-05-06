#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialog.h"

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

    connect(ui->maxSpeedSlider, SIGNAL(valueChanged(int)),
            this, SIGNAL(sigSetMaxSpeed(int)));
    
    connect(ui->userDefinedShowButton, SIGNAL(clicke()),
            this, SIGNAL(sigUserDefVecParams(Parameters*)));

//    connect(ui->tornadoShowButton, SIGNAL(clicked()),
//            this, SIGNAL(sigTornadoVecParams(Parameters*)));

    //connect(ui->particleFieldFunctionComboBox, SIGNAL(currentIndexChanged(int)),
    //        this, SIGNAL(sigParticleFieldFunction(int)));

    /* Attempt to get the 'connect' work the other way */
    //connect(this, SIGNAL(sigMorphPercentageOut(int)),
    //        ui->morphPercentageSlider, SLOT(setValue(int)));


}

void MainWindow::setMorphPercentageOut(int pct)
{
    ui->morphPercentageSlider->setValue(pct);
}

MainWindow::~MainWindow()
{
    delete ui;
}

int MainWindow::getDefaultNumOfParticles()
{
    return ui->numberOfParticlesSpinBox->value();
}

void MainWindow::on_addObjectButton_clicked()
{
    Dialog addObjectDialog;
    addObjectDialog.setModal(true);
    addObjectDialog.exec();
}

void MainWindow::on_updateVectorFieldButton_clicked()
{
    std::string temp[3] = {ui->vxTextInput->toPlainText().toStdString(), ui->vyTextInput->toPlainText().toStdString(), ui->vzTextInput->toPlainText().toStdString()};
    sigUpdateVectorField(temp);
}

void MainWindow::on_particleFieldFunctionComboBox_currentIndexChanged(int index)
{
    // Clear the window
    ui->userDefinedBox->setVisible(false);
    ui->flameParamsGroup->setVisible(false);
//    ui->tornadoParamsGroup->setvisible(false);

    // Show the appropriate widget box for each selection.
    // Index 0 = Flame function
    // Index 1 = Tornado function
    // index 2 = User defined function
    switch (index) {
        case 0:
            ui->flameParamsGroup->setVisible(true);
            break;
        case 1:
  //          ui->tornadoParamsGroup->setVisible(true);
            break;
        case 2:
            ui->userDefinedBox->setVisible(true);
            break;
    }
}


void MainWindow::on_flameShowButton_clicked()
{
    double vec3Pos[3] = (ui->xFieldTextInput->toPlainText().toDouble(),
                         ui->yFieldTextInput->toPlainText().toDouble(),
                         ui->zFieldTextInput->toPlainText().toDouble());
    double scale = ui->scaleSlider->value() / 100.0;
    float power = ui->powerSlider->value() / 100.0;
    float range = ui->rangeSlider->value() / 100.0;

    sigFlameVecParams(vec3Pos, scale, power, range);
}
