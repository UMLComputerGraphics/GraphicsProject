#include <cstdio>
#include <unistd.h>
#include <cstdlib>

#include "Engine.hpp"

int _argc;
char **_argv;

void simpleRotate( TransCache &obj ) {
  RotMat *newrot = new RotMat;
  newrot->rotateY( tick.scale() * 1.5 );
  newrot->rotateX( tick.scale() * 1.4 );
  newrot->rotateZ( tick.scale() * 1.3 );
  obj.push( newrot );
}

void qtLiteIdle( void ) {
  Scene &s = *(Engine::instance()->rootScene());
  Object &cube = *(s["cube"]);
  cube.animation( simpleRotate );
}

int main(int argc, char *argv[]) {
  int rc;

  _argc = argc;
  _argv = argv;
  Engine::instance()->init( &argc, argv, "wow!" );

  // Initialization ...
  GLuint gShader = Angel::InitShader( "../Binaries/shaders/vEngine.glsl", "../Binaries/shaders/fEngine.glsl" );
  Engine::instance()->cams()->addCamera( "Camera1" );
  Object *cube = Engine::instance()->rootScene()->addObject( "cube", gShader );
  colorCube( cube, 1.0 );
  cube->buffer();

  Engine::instance()->registerIdle( qtLiteIdle );
  glutMainLoop();
  return EXIT_SUCCESS;
}
