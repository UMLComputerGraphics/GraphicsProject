#include <cstdio>
#include <QApplication>
#include <QWidget>
#include <QThread>
#include <unistd.h>
#include <cstdlib>

#include "Engine.hpp"

int _argc;
char **_argv;

void *launchQT( void *ptr ) {
   QApplication app(_argc, _argv);
   QWidget window;
   window.resize(250, 150);
   window.setWindowTitle("Simple example");
   window.show();
   app.exec();   
   return ptr;
}

void simpleRotate( TransCache &obj ) {
  RotMat *newrot = new RotMat;
  newrot->rotateY( tick.scale() * 1.5 );
  newrot->rotateX( tick.scale() * 1.4 );
  newrot->rotateZ( tick.scale() * 1.3 );

  obj.push( newrot );
  //obj._rotation.rotateY( tick.scale() * 1.5 );
  //obj._rotation.rotateX( tick.scale() * 1.4 );
  //obj._rotation.rotateZ( tick.scale() * 1.3 );
}

void qtLiteIdle( void ) {
  Scene &s = *(Engine::instance()->rootScene());
  Object &cube = *(s["cube"]);
  cube.animation( simpleRotate );
}

int main(int argc, char *argv[]) {
  int rc;
  pthread_t thread_id;

  _argc = argc;
  _argv = argv;
  rc = pthread_create( &thread_id, NULL, launchQT, NULL );
  Engine::instance()->init( &argc, argv, "wow!" );

  GLuint gShader = Angel::InitShader( "../Binaries/shaders/vEngine.glsl", "../Binaries/shaders/fEngine.glsl" );
  Engine::instance()->cams()->addCamera( "Camera1" );
  Object *cube = Engine::instance()->rootScene()->addObject( "cube", gShader );
  colorCube( cube, 2.0 );
  cube->buffer();

  Engine::instance()->registerIdle( qtLiteIdle );
  glutMainLoop();
  return EXIT_SUCCESS;
}
