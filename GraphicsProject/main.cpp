#include "mainwindow.h"
#include <QApplication>
#include "OpenGL.h"

/* Engine Classes */
#include "Engine.hpp"
/* Utilities and Common */
#include "model.hpp"
#include "InitShader.hpp"
#include "glut_callbacks.h"

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("First Graphics Demo");
    w.show();
    return a.exec();;
}
