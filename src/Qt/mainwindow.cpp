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
    connect(this, SIGNAL(sigMorphPercentageOut(int)),
            ui->morphPercentageSlider, SLOT(setValue(int)));

}

MainWindow::~MainWindow()
{
    delete ui;
}
