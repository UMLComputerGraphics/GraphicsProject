/**
 @file MONOLITH.cpp
 @author UMLComputerGraphics <https://github.com/UMLComputerGraphics>
 @date 2013-03-29
 @brief This is a monolith of every component.
 @details Final project rough draft.
 Original engine based on Ed Angel's book code.
 **/

/* OpenGL and "The Engine" */
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
  
  // Get handles to the Scene and the Screen.
  Scene *rootScene = Engine::instance()->rootScene();
  Screen *primScreen = Engine::instance()->mainScreen();
  
  // Load shaders and use the resulting shader program. 
  GLuint gShader = Angel::InitShader( "shaders/vMONOLITH.glsl",
                                      "shaders/fMONOLITH.glsl" /*,    HAD TO REMOVE THE GEOMETRY SHADER BECAUSE IT'S not done.
                                      "shaders/gMONOLITH.glsl"*/ );

  tick.setTimeUniform(glGetUniformLocation( gShader, "ftime" ));

  // Let the other objects know which shader to use by default.
  rootScene->shader( gShader );
  primScreen->_camList.shader( gShader );
 
  // We start with no cameras, by default. Add one and set it "active" by using next().
  primScreen->_camList.addCamera( "Camera1" );
  primScreen->_camList.next();

  // Create an object and add it to the scene with the name "bottle".
  Object *bottle = rootScene->addObject( "bottle" );
  
  // Use the object loader to actually fill out the vertices and-so-on of the bottle.
  ObjLoader::loadModelFromFile( bottle, "../models/bottle-a.obj" );
  
  // Scale the bottle down!
  bottle->_trans._scale.set( 0.01 );
  
  for (uint i=0; i < bottle->_colors.size(); i++)
  {
    bottle->_colors[i].w = 0.4;
  }

  // _buffer the object onto the GPU. This does not happen by default,
  // To allow you to make many changes and _buffer only once,
  // or to _buffer changes selectively.
  bottle->buffer();
  
  // Object class has-a pointer to an object which is the morph target.
  // they are created and buffered as follows:
  
  // this makes a new object and links it to the source object. it returns the addr of the new obj..
  bottle->genMorphTarget( gShader );

  // tell the shaders to handle the bottle differently than a candle or something.
  GLuint morphing =  glGetUniformLocation(bottle->shader(), "isMorphing");
  glUniform1f(morphing, true);
  
  // this obscure allusion to "the thong song" brought to you by Eric McCann
  GLuint sisqo =  glGetUniformLocation(bottle->shader(), "letMeSeeThatPhong");
  glUniform1f(sisqo, true);

  // we can get the addr of the morph object like this, also.
  Object *bottleMorphTarget = bottle->morphTarget();
  
  // with this model, we can use all the preexisting Object class functionality
  ObjLoader::loadModelFromFile( bottleMorphTarget, "../models/bottle-b.obj" );
  bottleMorphTarget->_trans._scale.set( 0.01 );
  
  for (uint i=0; i < bottle->_colors.size(); i++)
  {
    bottleMorphTarget->_colors[i].w = 0.4;
  }

  // YES THIS IS THE REAL OBJECT, NOT THE TARGET. 
  // IT SENDS THE MORPH VERTICES TO THE SHADER, NOT TO THE DRAW LIST TO BE DRAWN!
  bottle->bufferMorphOnly();
  
  // Generic OpenGL setup: Enable the depth _buffer and set a nice background color.
  glEnable( GL_DEPTH_TEST );
  glClearColor( 0.0, 0.0, 0.0, 1.0 );
  
}

/**
 * Cleans up our scene graph.
 */
void cleanup( void ) {
  Engine::instance()->rootScene()->delObject();
}


/**
 * Apply animations and whatever else your heart desires.
 */
void monolith_idle( void ) {
  
  static Scene *rootScene = Engine::instance()->rootScene();
  
  // Animation variables.
  double timer = glutGet( GLUT_ELAPSED_TIME ) / 500.0;
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

  fprintf( stderr, "Error: Your project is unfinished and you're going to fail and everyone is going to laugh at you and it's a really bad day sorry\n" );

  Engine::instance()->init( &argc, argv, "WE ARE THE BORG. RESISTANCE IS FUTILE!" );
  Engine::instance()->registerIdle( monolith_idle );
  init();
  
  /* PULL THE TRIGGER */
  glutMainLoop();
  return EXIT_SUCCESS;
  
}
