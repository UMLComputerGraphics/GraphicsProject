/**
 * @file ds.cpp
 * @author John Huston
 * @authors John Huston, Greg Giannone
 * @date 2013-02-20
 * @brief Dual-shader demo.
 * @details Work in progress!
 * Based loosely on Ed Angel's tutorials.
 **/

/* Engine Classes */
#include "Engine.hpp"
/* Utilities and Common */
#include "model.hpp"
#include "InitShader.hpp"
#include "glut_callbacks.h"
#include "ObjLoader.hpp"

/**
 * Initialization: load and compile shaders, initialize camera(s), load models.
 */
void init() {
  
  GLuint shader[1];
  Cameras *camList = Engine::instance()->cams();
  Scene *rootScene = Engine::instance()->rootScene();
  
  shader[0] = Angel::InitShader( "shaders/vterrain.glsl", "shaders/ftex.glsl" );
  
  camList->addCamera( "Camera1" );
  camList->next();
  
  // Setting a default and adding objects without:
  rootScene->shader( shader[0] );

  Object *E = rootScene->addObject( "Bottle!" );
  ObjLoader::loadModelFromFile( E, "../models/bottle_liquor_high.obj" );
  E->_trans._scale.set( 0.10 );
  E->propagate();
  E->buffer();

  glEnable( GL_DEPTH_TEST );
  glClearColor( 0, 0, 0, 1.0 );

  Engine::instance()->texMan()->rebind();
}

/**
 * Cleans up our scene graph.
 */
void cleanup( void ) {
  //Engine::instance()->rootScene()->DestroyObject();
}

//--------------------------------------------------------------------

/**
 * This is a dual-shader demo! It looks very simple,
 * but it illustrates quickly and effectively how to use two shaders.
 *
 * @param argc Not used.
 * @param argv Not used.
 * @return EXIT_SUCCESS.
 *
 */
int main( int argc, char **argv ) {
  
  Engine::init( &argc, argv, "Model Demonstration" );
  init();

  /* PULL THE TRIGGER */
  glutMainLoop();
  return EXIT_SUCCESS;
  
}
