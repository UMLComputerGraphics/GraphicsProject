#include "glwidget.h"
#include "OpenGL.h"

GLWIdget::GLWIdget(QWidget *parent) :
    QGLWidget(parent)
{
}


void GLWIdget::initializeGL()
{
    glClearColor(0.2, 0.2, 0.2, 1);
}

void GLWIdget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);


    glBegin(GL_TRIANGLES);
    glColor3f(1,0,0);
    glVertex3f(-0.5, -0.5, 0);
    glColor3f(0,1,0);
    glVertex3f(0.5, -0.5, 0);
    glColor3f(0,0,1);
    glVertex3f(0.0, 0.5, 0);
    glEnd();
}

void GLWIdget::updateGL()
{

}
