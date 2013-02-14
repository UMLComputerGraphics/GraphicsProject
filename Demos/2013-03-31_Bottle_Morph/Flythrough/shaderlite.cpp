/**
   @file shaderlite.cpp
   @author Greg Giannone
   @authors John Huston, Nicholas VerVoort, Chris Compton
   @date 2013-2-13
   @brief An attempt at having different shaders for different objects
   @details This is a demo this the objective of loading two different models
   and having each one have a different shader associated with it
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
/* Ed Angel's Math Classes */
#include "vec.hpp"
#include "mat.hpp"
/* Utilities and Classes */
#include "model.hpp"
#include "Camera.hpp"
#include "InitShader.hpp"
#include "Cameras.hpp"
#include "Screen.hpp"
#include "Object.hpp"
#include "Timer.hpp"
#include "Scene.hpp"

#include "LightSource.hpp"
#include "Lights.hpp"


// Type Aliases
using Angel::vec3;
using Angel::vec4;
typedef Angel::vec4 color4;
typedef Angel::vec4 point4;

// Global objects for magical camera success
Screen myScreen( 800, 600 );
Scene theScene;
bool fixed_yaw = true;

// Initialization: load and compile shaders, initialize camera(s), load models.
void init() {
  
  GLuint gShader, gHeavy;

  // Load shaders and use the resulting shader program. 
  gHeavy = Angel::InitShader( "shaders/vtest.glsl", "shaders/ftest.glsl" );
  gShader = Angel::InitShader("shaders/vterrain.glsl", "shaders/fterrain.glsl");

  std::cout << "gHeavy is: " << gHeavy << std::endl;
  std::cout << "gShader is: " << gShader << std::endl;

  theScene.SetShader( gShader );
  myScreen.camList.SetShader( gShader );
  myScreen.camList.AddCamera( "Camera1");
  myScreen.camList.Next();

  Object *heavy = theScene.AddObject("heavy", gHeavy);
  Object *medic = theScene.AddObject("medic", gShader);
  Object *spy = theScene.AddObject("spy", gShader);
  loadModelFromFile( heavy, "../models/heavyT.obj" );
  loadModelFromFile( medic, "../models/medicT.obj" );
  loadModelFromFile(spy, "../models/spyT.obj");
  heavy->trans.scale.Set(0.10);
  heavy->trans.offset.Set(1, 0, -2);
  medic->trans.scale.Set(0.10);
  medic->trans.offset.Set(-1, 0, -2);
  spy->trans.scale.Set(0.10);
  spy->trans.offset.Set(0, 0, -2);
  heavy->Buffer();
  medic->Buffer();
  spy->Buffer();
  heavy->Propegate();
  medic->Propegate();
  spy->Propegate();

  glEnable( GL_DEPTH_TEST );
  glClearColor( 0.3, 0.5, 0.9, 1.0 );

}

void cleanup( void ) {
  theScene.DestroyObject();
}

//--------------------------------------------------------------------

/** A function that takes no arguments.
    Is responsible for drawing a SINGLE VIEWPORT. **/
void displayViewport( void ) {  
  theScene.Draw(); /* Draw free-floating objects */
  myScreen.camList.Draw(); /* Draw camera-attached objects */
}

void display( void ) {
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  // Tell camList to draw using our displayViewport rendering function.
  myScreen.camList.View( displayViewport );
  glutSwapBuffers();
}

// Sorry about this, but it's better if you don't know.
#include "keybindings.inc"

void resizeEvent( int width, int height ) {

  /* Handles resizing the child cameras as well. */
  myScreen.Size( width, height );
  glutWarpPointer( myScreen.MidpointX(), myScreen.MidpointY() );

}


void idle( void ) {

  // Compute the time since last idle().
  Tick.Tock();
  static double timer = 0.0 ;
  if ( (timer += 0.005 ) > 360.0 ) timer = 0.0 ;
  float percent = ( sin(timer) + 1 ) / 2 ;

  if (DEBUG_MOTION) 
    fprintf( stderr, "Time since last idle: %lu\n", Tick.Delta() );

  // Move all camera(s).
  myScreen.camList.IdleMotion();
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

  glutInit( &argc, argv );
  glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
  glutInitWindowSize( myScreen.Width(), myScreen.Height() );
  glutCreateWindow( "Multiple Shader Demo" );
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
