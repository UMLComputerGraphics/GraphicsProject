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
  GLuint gShader = Angel::InitShader( "shaders/vmorph.glsl",
                                      "shaders/fmorph.glsl" );
  
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
  
  // _buffer the object onto the GPU. This does not happen by default,
  // To allow you to make many changes and _buffer only once,
  // or to _buffer changes selectively.
  bottle->buffer();
  
  // Object class has-a pointer to an object which is the morph target.
  // they are created and buffered as follows:
  
  // this makes a new object and links it to the source object. it returns the addr of the new obj..
  bottle->genMorphTarget( gShader );
  
  // we can get the addr of the morph object like this, also.
  Object *bottleMorphTarget = bottle->morphTarget();
  
  // with this model, we can use all the preexisting Object class functionality
  ObjLoader::loadModelFromFile( bottleMorphTarget, "../models/bottle-b.obj" );
  bottleMorphTarget->_trans._scale.set( 0.01 );
  
  // YES THIS IS THE REAL OBJECT, NOT THE TARGET. 
  // IT SENDS THE MORPH VERTICES TO THE SHADER, NOT TO THE DRAW LIST TO BE DRAWN!
  bottle->bufferMorphOnly();
  
  // Generic OpenGL setup: Enable the depth _buffer and set a nice background color.
  glEnable( GL_DEPTH_TEST );
  glClearColor( 0.3, 0.5, 0.9, 1.0 );
  
}

/**
 * Cleans up our scene graph.
 */
void cleanup( void ) {
  //Engine::instance()->rootScene()->DestroyObject();
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

/**
 * Display/Render the entire screen.
 */
void display( void ) {
  static Cameras *camList = Engine::instance()->cams();
  
  // Clear the _buffer.
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  
  // Tell camList to draw using our 'draw' rendering function.
  camList->view( draw );
  
  // Swap to the next _buffer.
  glutSwapBuffers();
  
}

/**
 * Compute time since last idle, update camera positions, redisplay.
 * Apply new animations.
 */
void idle( void ) {
  
  static Cameras *camList = Engine::instance()->cams();
  static Scene *rootScene = Engine::instance()->rootScene();
  
  // Compute the time since last idle().
  // This is a global, stateful operation.
  tick.tock();
  
  // Animation variables.
  static double timer = 0.0;
  if ( (timer += 0.005) > 360.0 ) timer = 0.0;
  float percent = (sin( timer ) + 1) / 2;
  
  // Update the morph percentage.
  (*rootScene)["bottle"]->morphPercentage( percent );
  
  if ( DEBUG_MOTION )
    fprintf( stderr, "Time since last idle: %lu\n", tick.delta() );
  
  // Move all cameras: Apply velocity and acceleration adjustments.
  // If no cameras are currently moving, this will do nothing ;)
  camList->idleMotion();
  
  // Inform GLUT we'd like to render a new frame.
  glutPostRedisplay();
  
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
  
  // OS X suppresses events after mouse warp.  This resets the suppression 
  // interval to 0 so that events will not be suppressed. This also found
  // at http://stackoverflow.com/questions/728049/
  // glutpassivemotionfunc-and-glutwarpmousepointer
#ifdef __APPLE__
  CGSetLocalEventsSuppressionInterval( 0.0 );
#endif
  VooDoo::InitRelativePaths(argc, argv);
  
  glutInit( &argc, argv );
  glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
  glutInitWindowSize( 0, 0 );
  glutCreateWindow( "Linear Interpolation Morphing Demo" );
  glutFullScreen();
  glutSetCursor( GLUT_CURSOR_NONE );
  
  GLEW_INIT();
  init();
  
  /* Register our Callbacks */
  glutDisplayFunc( display );
  glutKeyboardFunc( keyboard );
  glutKeyboardUpFunc( keylift );
  glutSpecialFunc( keyboard_ctrl );
  glutMouseFunc( mouse );
  glutMotionFunc( mouseroll );
  glutPassiveMotionFunc( mouselook );
  glutIdleFunc( idle );
  glutReshapeFunc( resizeEvent );
  
  /* PULL THE TRIGGER */
  glutMainLoop();
  return EXIT_SUCCESS;
  
}
