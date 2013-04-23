/**
 @file morphlite.cpp
 @author Nicholas St.Pierre
 @authors John Huston, Nicholas VerVoort, Chris Compton
 @date 2012-12-06
 @brief This is a derivative of HSC's Fall 2012 project.
 @details This is a tech demo for morphing two objects back and forth.
 Original engine based on Ed Angel's book code.
 This file features a severely reduced linecount for demo purposes.
 @see terrain.cpp for a fully-featured example.
 **/

// The Engine!
#include "Engine.hpp"

/**
 * Initialization: load and compile shaders, initialize camera(s), load models.
 */
void init() {
  
  // Get handles to the Scene and the Screen.
  Scene *rootScene = Engine::instance()->rootScene();
  
  // Create an object and add it to the scene with the name "bottle".
  Object *bottle = rootScene->addObject( "bottle" );
  
  // Use the object loader to actually fill out the vertices and-so-on of the bottle.
  ObjLoader::loadModelFromFile( bottle, "../models/bottle-a.obj" );
  
  // Scale the bottle down!
  bottle->_trans._scale.set( 0.01 );
  
  // Object class has-a pointer to an object which is the morph target.
  // they are created and buffered as follows:
  
  // this makes a new object and links it to the source object. it returns the addr of the new obj..
  bottle->genMorphTarget();
  
  // we can get the addr of the morph object like this, also.
  Object *bottleMorphTarget = bottle->morphTarget();
  
  // with this model, we can use all the preexisting Object class functionality
  ObjLoader::loadModelFromFile( bottleMorphTarget, "../models/bottle-b.obj" );
  bottleMorphTarget->_trans._scale.set( 0.01 );
  
  // _buffer the object onto the GPU. This does not happen by default,
  // To allow you to make many changes and _buffer only once,
  // or to _buffer changes selectively.
  // Buffer the bottle AND the morph. Do this AFTER loading both.
  bottle->buffer();
  
  // Generic OpenGL setup: Enable the depth _buffer and set a nice background color.
  glClearColor( 0.3, 0.5, 0.9, 1.0 );
}

/**
 * Compute time since last idle, update camera positions, redisplay.
 * Apply new animations.
 */
void bottle_idle( void ) {
  
  static Scene *rootScene = Engine::instance()->rootScene();
  
  // Animation variables.
  static double timer = 0.0;
  if ( (timer += 0.005) > 360.0 ) timer = 0.0;
  float percent = (sin( timer ) + 1) / 2;
  
  // Update the morph percentage.
  (*rootScene)["bottle"]->morphPercentage( percent );
  
}

/**
 * This is a bottle morphing demo!
 * It illustrates simply how to do a simple linear interpolation morph.
 *
 * @param argc Not used.
 * @param argv Not used.
 * @return EXIT_SUCCESS.
 *
 */
int main( int argc, char **argv ) {
  
  Engine::init( &argc, argv, "Linear Interpolation Morphing Demo" );
  Engine::instance()->registerIdle( bottle_idle );
  init();
  
  glutMainLoop();
  return EXIT_SUCCESS;
  
}
