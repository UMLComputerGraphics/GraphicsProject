#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSlider>
#include <QLabel>
#include <QHBoxLayout>
#include <stdio.h>
#include <QVBoxLayout>
#include <QCheckBox>

QLabel *lab;
QLabel *lab2;
QLabel *lab3;
QLabel *lab4;
QLabel *lab5;
QLabel *lab6;
QLabel *stateLabel;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QWidget *widge = new QWidget();

    QCheckBox *box1 = new QCheckBox();
    QCheckBox *box2 = new QCheckBox();
    QCheckBox *box3 = new QCheckBox();
    QLabel *lab13 = new QLabel("State 1");
    QLabel *lab14 = new QLabel("State 2");
    QLabel *lab15 = new QLabel("State 3");
    stateLabel = new QLabel("No state selected.");

    QHBoxLayout *mainLayoutBox = new QHBoxLayout(this);
    mainLayoutBox->addWidget(lab13);
    mainLayoutBox->addSpacing(2);
    mainLayoutBox->addWidget(box1);
    mainLayoutBox->addSpacing(10);
    mainLayoutBox->addWidget(lab14);
    mainLayoutBox->addSpacing(2);
    mainLayoutBox->addWidget(box2);
    mainLayoutBox->addSpacing(10);
    mainLayoutBox->addWidget(lab15);
    mainLayoutBox->addSpacing(2);
    mainLayoutBox->addWidget(box3);

    lab = new QLabel();
    lab->setText("TESTING");
    lab2 = new QLabel();
    lab2->setText("TESTING2");
    lab3 = new QLabel();
    lab3->setText("TESTING3");
    lab4 = new QLabel();
    lab4->setText("TESTING4");
    lab5 = new QLabel();
    lab5->setText("TESTING5");
    lab6 = new QLabel();
    lab6->setText("TESTING6");

    QLabel *lab7 = new QLabel("0");
    QLabel *lab8 = new QLabel("0");
    QLabel *lab9 = new QLabel("0");
    QLabel *lab10 = new QLabel("0");
    QLabel *lab11 = new QLabel("0");
    QLabel *lab12 = new QLabel("0");


    slider = new QSlider(Qt::Horizontal);
    slider->setRange(0, 10);
    QSlider *slider2 = new QSlider(Qt::Horizontal);
    slider2->setRange(0, 100);
    QSlider *slider3 = new QSlider(Qt::Horizontal);
    slider3->setRange(0, 30);
    QSlider *slider4 = new QSlider(Qt::Horizontal);
    slider4->setRange(0, 40);
    QSlider *slider5 = new QSlider(Qt::Horizontal);
    slider5->setRange(0, 50);
    QSlider *slider6 = new QSlider(Qt::Horizontal);
    slider6->setRange(0, 100);
    //QObject::connect(slider, SIGNAL(valueChanged(int)), this, SLOT(printTest(int)));
    QObject::connect(slider2, SIGNAL(valueChanged(int)), this, SLOT(printTest2(int)));
    QObject::connect(slider3, SIGNAL(valueChanged(int)), this, SLOT(printTest3(int)));
    QObject::connect(slider4, SIGNAL(valueChanged(int)), this, SLOT(printTest4(int)));
    QObject::connect(slider5, SIGNAL(valueChanged(int)), this, SLOT(printTest5(int)));
    QObject::connect(slider6, SIGNAL(valueChanged(int)), this, SLOT(printTest6(int)));
    QObject::connect(box1, SIGNAL(toggled(bool)), this, SLOT(checkChanged(bool)));
    QObject::connect(box2, SIGNAL(toggled(bool)), this, SLOT(checkChanged2(bool)));
    QObject::connect(box3, SIGNAL(toggled(bool)), this, SLOT(checkChanged3(bool)));


    QVBoxLayout *windowLayout = new QVBoxLayout(this);

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->addWidget(lab7);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(slider);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(lab);

    QHBoxLayout *mainLayout2 = new QHBoxLayout(this);
    mainLayout2->addWidget(lab8);
    mainLayout2->addSpacing(10);
    mainLayout2->addWidget(slider2);
    mainLayout2->addSpacing(10);
    mainLayout2->addWidget(lab2);

    QHBoxLayout *mainLayout3 = new QHBoxLayout(this);
    mainLayout3->addWidget(lab9);
    mainLayout3->addSpacing(10);
    mainLayout3->addWidget(slider3);
    mainLayout3->addSpacing(10);
    mainLayout3->addWidget(lab3);

    QHBoxLayout *mainLayout4 = new QHBoxLayout(this);
    mainLayout4->addWidget(lab10);
    mainLayout4->addSpacing(10);
    mainLayout4->addWidget(slider4);
    mainLayout4->addSpacing(10);
    mainLayout4->addWidget(lab4);

    QHBoxLayout *mainLayout5 = new QHBoxLayout(this);
    mainLayout5->addWidget(lab11);
    mainLayout5->addSpacing(10);
    mainLayout5->addWidget(slider5);
    mainLayout5->addSpacing(10);
    mainLayout5->addWidget(lab5);

    QHBoxLayout *mainLayout6 = new QHBoxLayout(this);
    mainLayout6->addWidget(lab12);
    mainLayout6->addSpacing(10);
    mainLayout6->addWidget(slider6);
    mainLayout6->addSpacing(10);
    mainLayout6->addWidget(lab6);

    windowLayout->addLayout(mainLayoutBox);
    windowLayout->addSpacing(20);
    windowLayout->addWidget(stateLabel);
    windowLayout->addSpacing(20);
    windowLayout->addLayout(mainLayout);
    windowLayout->addSpacing(20);
    windowLayout->addLayout(mainLayout2);
    windowLayout->addSpacing(20);
    windowLayout->addLayout(mainLayout3);
    windowLayout->addSpacing(20);
    windowLayout->addLayout(mainLayout4);
    windowLayout->addSpacing(20);
    windowLayout->addLayout(mainLayout5);
    windowLayout->addSpacing(20);
    windowLayout->addLayout(mainLayout6);
    widge->resize(500, 500);
    widge->setLayout(windowLayout);
    widge->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::printTest(int num)
{
    QString string = QString::number(num, 10);
    lab->setText(string);
    //QDebug << "ok";
}

void MainWindow::printTest2(int num)
{
    QString string = QString::number(num, 10);
    lab2->setText(string);
    //QDebug << "ok";
}

void MainWindow::printTest3(int num)
{
    QString string = QString::number(num, 10);
    lab3->setText(string);
    //QDebug << "ok";
}

void MainWindow::printTest4(int num)
{
    QString string = QString::number(num, 10);
    lab4->setText(string);
    //QDebug << "ok";
}

void MainWindow::printTest5(int num)
{
    QString string = QString::number(num, 10);
    lab5->setText(string);
    //QDebug << "ok";
}

void MainWindow::printTest6(int num)
{
    QString string = QString::number(num, 10);
    lab6->setText(string);
    //QDebug << "ok";
}

void MainWindow::checkChanged(bool b)
{
    stateLabel->setText("State 1 selected.");
}

void MainWindow::checkChanged2(bool b)
{
    stateLabel->setText("State 2 selected.");
}

void MainWindow::checkChanged3(bool b)
{
    stateLabel->setText("State 3 selected.");
}
