/**
 * @file sg.cpp
 * @author John Huston
 * @date 2013-04-23
 * @brief Scene Graph Demo
 * @details Showcases a new Scene Graph API,
 * as well as demonstrates a new reduced line-count for
 * a minimalistic Engine API.
 */

#include <cstdio>
#include <unistd.h>
#include <cstdlib>
#include "Engine.hpp"

void simpleRotate( TransCache &obj ) {
  
  RotMat newrot;
  newrot.rotateY( tick.scale() * 1.5 );
  newrot.rotateX( tick.scale() * 1.4 );
  newrot.rotateZ( tick.scale() * 1.3 );
  obj.push( newrot );
  
}

void sgIdle( void ) {
  Scene &s = *(Engine::instance()->rootScene());
  Object &cube = *(s["cube"]);
  cube.animation( simpleRotate );
}

int main( int argc, char *argv[] ) {
  
  Engine::instance()->init( &argc, argv, "wow!" );
  
  // New Initialization includes default Camera and Shader.
  // Scene and Camlist default to using this shader.
  Object *cube = Engine::instance()->rootScene()->addObject( "cube" );
  colorCube( cube, 1.0 );
  cube->buffer();
  
  Engine::instance()->registerIdle( sgIdle );
  glutMainLoop();
  return EXIT_SUCCESS;
}
