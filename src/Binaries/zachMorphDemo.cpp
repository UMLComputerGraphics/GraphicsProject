/**
 @file zachMorphDemo.cpp
 @author Zach Maybury
 @authors Nicholas St.Pierre, John Huston, Nicholas VerVoort, Chris Compton
 @date 2013-03-28
 @brief Zach's Bottle Morphing Demo.
 @details This is a tech demo for morphing two objects back and forth.
 The main driver code is based on Ed Angel's code from his book.
 **/

#include "globals.h"
/* System Headers */
#include <cmath>
#include <cstdio>
#include <sstream>
#include <cstdlib>
#include <time.h>
/* Multi-platform support and OpenGL headers. */
#include "platform.h"
#include "Camera.hpp"
#include "Cameras.hpp"
#include "Screen.hpp"
#include "Object.hpp"
#include "Timer.hpp"
#include "Scene.hpp"
#include "Engine.hpp"
/* Utilities and Common */
#include "model.hpp"
#include "InitShader.hpp"
#include "glut_callbacks.h"
#include "ObjLoader.hpp"

// Initialization: load and compile shaders, initialize camera(s), load models.
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
  ObjLoader::loadModelFromFile( bottle, "../models/bottle-b.obj" );
  
  // Objects has-a pointer to an object which is their "morph target."
  // they are created and buffered as follows:
  
  bottle->genMorphTarget( gShader );// this makes a new object and links it to the source object. it returns the addr of the new obj..
  Object *bottleMorphTarget = bottle->morphTarget();// we can get the addr of the morph object like this, also.
  ObjLoader::loadModelFromFile( bottleMorphTarget, "../models/bottle-a.obj" );// with this model, we can use all the preexisting Object class functionality
  //loadModelFromFile( bottle->getMorphTargetPtr(), "../models/bottle-b.obj" ); // with this model, we can use all the preexisting Object class functionality
  
  printf( "Number Vertices: %d\n", bottle->numberOfPoints() );
  printf( "Number Vertices: %d\n", bottleMorphTarget->numberOfPoints() );
  printf( "Colors: %lu\n", bottle->_colors.size() );
  printf( "Normals: %lu\n", bottle->_normals.size() );
  printf( "Indices: %lu\n", bottle->_indices.size() );
  //makeModelsSameSize(bottle, bottleMorphTarget);
  
  printf( "Number Vertices: %d\n", bottle->numberOfPoints() );
  printf( "Number Vertices: %d\n", bottleMorphTarget->numberOfPoints() );
  
  bottle->_trans._scale.set( 0.01 );
  bottleMorphTarget->_trans._scale.set( 0.01 );
  bottle->buffer();
  bottle->bufferMorphOnly();// YES THIS IS THE REAL OBJECT, NOT THE TARGET. IT SENDS THE MORPH VERTICES TO THE SHADER, NOT TO THE DRAW LIST TO BE DRAWN!
  
  // Generic OpenGL setup: Enable the depth _buffer and set a nice background color.
  glEnable( GL_DEPTH_TEST );
  glClearColor( 0.3, 0.5, 0.9, 1.0 );
  
}

void cleanup( void ) {
  //Engine::instance()->rootScene()->DestroyObject();
}

//--------------------------------------------------------------------

// Implementation of drawing the display with regards to a single viewport.
void draw( void ) {
  static Scene *theScene = Engine::instance()->rootScene();
  static Cameras *camList = Engine::instance()->cams();
  
  theScene->draw();
  camList->draw();
}

// GLUT display callback. Effectively calls displayViewport per-each Camera.
void display( void ) {
  static Cameras *camList = Engine::instance()->cams();
  
  // Clear the _buffer.
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  
  // Tell camList to draw using our 'draw' rendering function.
  camList->view( draw );
  
  // Swap to the next _buffer.
  glutSwapBuffers();
  
}

void simpleRotateAnim( TransCache &obj ) {
  
  obj._rotation.rotateY( tick.scale() * 1.5 );
  obj._offset.set( 1.5, 0, 0 );
  obj._orbit.rotateY( tick.scale() * -1.0 );
  
}

void idle( void ) {
  
  static Cameras *camList = Engine::instance()->cams();
  static Scene *rootScene = Engine::instance()->rootScene();
  tick.tock();
  
  //zach m  - in order to eventually allow for user specified equations,
  //          we need to think of equations between 0 and 1.
  //          The current line "percent = timer" would be the line where
  //          we would handle a user specified equation in the form of
  //          y = x, where y is the percent and x is the timer. We may
  //          want to specify that the y should equal 1 when x equals 1,
  //          otherwise we can get a percent greater than 100.
  static double timer = 0.0;
  static bool trigger = false;
  if ( trigger == false ) {
    if ( (timer += 0.01) > 1 ) {
      timer = 1.0;
      trigger = true;
    }
  } else {
    if ( (timer -= 0.01) < 0 ) {
      timer = 0.0;
      trigger = false;
    }
  }
  float percent = timer;
  
  //if ( (timer += 0.05 ) > 360.0 ) timer = 0.0 ;
  //float percent = ( sin(timer) + 1 ) / 2 ;
  (*rootScene)["bottle"]->morphPercentage( percent );
  
  if ( DEBUG_MOTION )
    fprintf( stderr, "Time since last idle: %lu\n", tick.delta() );
  
  // Move all cameras: Apply velocity and acceleration adjustments.
  // If no cameras are currently moving, this will do nothing ;)
  camList->idleMotion();
  
  // Inform GLUT we'd like to render a new frame.
  glutPostRedisplay();
  
}

int main( int argc, char **argv ) {
  
  // OS X suppresses events after mouse warp.  This resets the suppression 
  // interval to 0 so that events will not be suppressed. This also found
  // at http://stackoverflow.com/questions/728049/
  // glutpassivemotionfunc-and-glutwarpmousepointer
#ifdef __APPLE__
  CGSetLocalEventsSuppressionInterval( 0.0 );
#endif
  Angel::InitInitShader( argv[0] );
  
  glutInit( &argc, argv );
  glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
  //glutInitWindowSize( myScreen.Width(), myScreen.Height() );
  glutCreateWindow( "Zach's Morphing Demo" );
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
  
  if ( DEBUG ) {
    fprintf( stderr, "GL_VENDOR: %s\n", glGetString( GL_VENDOR ) );
    fprintf( stderr, "GL_RENDERER: %s\n", glGetString( GL_RENDERER ) );
    fprintf( stderr, "GL_VERSION: %s\n", glGetString( GL_VERSION ) );
    fprintf( stderr, "GL_SHADING_LANGUAGE_VERSION: %s\n",
             glGetString( GL_SHADING_LANGUAGE_VERSION ) );
  }
  
  /* PULL THE TRIGGER */
  glutMainLoop();
  return EXIT_SUCCESS;
  
}
