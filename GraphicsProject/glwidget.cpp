#include "glwidget.h"
#include "OpenGL.h"

/* Engine Classes */
#include "Engine.hpp"
/* Utilities and Common */
#include "model.hpp"
#include "InitShader.hpp"
#include "glut_callbacks.h"
#include "ObjLoader.hpp"


GLWIdget::GLWIdget(QWidget *parent) :
    QGLWidget(parent)
{
}

/**
 * Initialization: load and compile shaders, initialize camera(s), load models.
 */
void GLWIdget::initializeGL()
{

    GLuint shader[3];
    Cameras *camList = Engine::instance()->cams();
    Scene *rootScene = Engine::instance()->rootScene();

    shader[0] = Angel::InitShader( "shaders/vred.glsl", "shaders/fragment.glsl" );
    shader[1] = Angel::InitShader( "shaders/vblu.glsl", "shaders/fragment.glsl" );
    shader[2] = Angel::InitShader( "shaders/vtex.glsl", "shaders/ftex.glsl" );

    camList->addCamera( "Camera1" );
    camList->next();
    camList->active()->changePerspective( Camera::IDENTITY );

    // Adding objects without a default shader:
    Object *A = rootScene->addObject( "Object A (RED)", shader[0] );

    // Setting a default and adding objects without:
    rootScene->shader( shader[1] );
    Object *B = rootScene->addObject( "Object B (BLU)" );

    // Third Object, over-ride default shader.
    Object *C = rootScene->addObject( "Object C (TEX)", shader[2] );

    // Fourth Object.
    Object *D = rootScene->addObject( "Object D (TEX2)", shader[2] );

    // draw a square in the upper left
    triangle( A, vec4( -1, 0, 0, 1 ), vec4( 0, 0, 0, 1 ), vec4( -1, 1, 0, 1 ),
              0 );
    triangle( A, vec4( 0, 0, 0, 1 ), vec4( -1, 1, 0, 1 ), vec4( 0, 1, 0, 1 ), 0 );
    A->buffer();

    // draw a square in the lower right
    triangle( B, vec4( 0, -1, 0, 1 ), vec4( 1, -1, 0, 1 ), vec4( 0, 0, 0, 1 ),
              0 );
    triangle( B, vec4( 1, -1, 0, 1 ), vec4( 0, 0, 0, 1 ), vec4( 1, 0, 0, 1 ), 0 );
    B->buffer();

    // draw a triangle in the lower left, and texture it.
    triangle( C, vec4( -1, -1, 0, 1 ), vec4( 0, -1, 0, 1 ), vec4( -1, 0, 0, 1 ),
              0 );
    C->_texUVs.push_back( vec2( 0, 0 ) );
    C->_texUVs.push_back( vec2( 1, 1 ) );
    C->_texUVs.push_back( vec2( 0, 1 ) );
    C->buffer();
    const char *filename = "../Textures/GrassGreenTexture0002.jpg";
    C->texture( filename );

    // draw another triangle in the lower left, texture it differently.
    triangle( D, vec4( -1, 0, 0, 1), vec4( 0, 0, 0, 1 ), vec4( 0, -1, 0, 1 ), 0 );
    D->_texUVs.push_back( vec2( 0, 0 ) );
    D->_texUVs.push_back( vec2( 1, 1 ) );
    D->_texUVs.push_back( vec2( 0, 1 ) );
    D->buffer();
    D->texture( "../Textures/GoodTextures_0013291.jpg" );

    glEnable( GL_DEPTH_TEST );
    glClearColor( 0, 0, 0, 1.0 );

    Engine::instance()->texMan()->rebind();
}

/**
 * Implementation of drawing the display with regards to a single viewport.
 */
void draw( void ) {
  static Scene *theScene = Engine::instance()->rootScene();
  static Cameras *camList = Engine::instance()->cams();

  theScene->draw();
  camList->draw();
}

void GLWIdget::paintGL()
{
    static Cameras *camList = Engine::instance()->cams();

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // Tell camList to draw using our 'draw' rendering function.
    camList->view( draw );

    glutSwapBuffers();
}

void GLWIdget::updateGL()
{
    static Cameras *camList = Engine::instance()->cams();

    // Compute the time since last idle().
    tick.tock();

    // Move all camera(s).
    camList->idleMotion();
    glutPostRedisplay();
}
