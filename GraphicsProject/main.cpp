#include "mainwindow.h"
#include <QApplication>
#include "OpenGL.h"
#include <GL/glut.h>


int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("First Graphics Demo");
    w.show();
    return a.exec();
}
