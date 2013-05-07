#include "dialog.h"
#include "ui_dialog.h"
#include <QFileDialog>

#include <QtCore>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_objLocationButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "",
                                                   tr("Files (*.obj*)"));
}
