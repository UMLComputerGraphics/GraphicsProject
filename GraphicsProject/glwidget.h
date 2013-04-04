#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include "OpenGL.h"

class GLWIdget : public QGLWidget
{
    Q_OBJECT
public:
    explicit GLWIdget(QWidget *parent = 0);
    
    void initializeGL();
    void paintGL();
    void updateGL();
};

#endif // GLWIDGET_H
