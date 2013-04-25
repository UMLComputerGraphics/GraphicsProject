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
}


MainWindow::~MainWindow()
{
    delete ui;
}
