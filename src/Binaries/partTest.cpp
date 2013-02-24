/**
   @file partTest.cpp
   @author Nicholas St.Pierre
   @authors John Huston, Nicholas VerVoort, Chris Compton
   @date 2013-02-23
   @brief This is a derivative of our main project file, fly.cpp.
   @details This is a tech demo for a particle system
*/

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
#include "glut_callbacks.h"
#include "Engine.hpp"

#include "LightSource.hpp"
#include "Lights.hpp"

#include "Particle.hpp"

// Type Aliases
using Angel::vec3;
using Angel::vec4;
typedef Angel::vec4 color4;
typedef Angel::vec4 point4;

// Global objects for magical camera success
Screen myScreen( 800, 600 );
bool fixed_yaw = true;

// Initialization: load and compile shaders, initialize camera(s), load models.
void init() 
{

  GLuint gShader;
  Cameras *camList = Engine::Instance()->Cams();
  Scene *rootScene = Engine::Instance()->RootScene();
  
  // Load shaders and use the resulting shader program. 
  gShader = Angel::InitShader( "shaders/vmorph.glsl", "shaders/fmorph.glsl" );

  camList->SetShader( gShader );
  camList->AddCamera( "Camera1" );
  camList->Next();
  camList->Active()->changePerspective( Camera::IDENTITY );

  Particle particle = Particle( vec4(0,0,0,1), vec3(0,0,0), vec3(0,0,0), 1.0,
				 vec4( 1,1,1,1), 1.0, 0, "someString", gShader );
  //  theScene.InsertObject( "particle", &particle );

  // Generic OpenGL setup: Enable the depth buffer and set a nice background color.
  glEnable( GL_DEPTH_TEST );
  glClearColor( 0.3, 0.5, 0.9, 1.0 );

}

void cleanup( void ) 
{
  Engine::Instance()->RootScene()->DestroyObject();
}

//--------------------------------------------------------------------

void draw( void )
{
  static Scene *theScene  = Engine::Instance()->RootScene();
  static Cameras *camList = Engine::Instance()->Cams();

  theScene->Draw();
  camList->Draw();
}

// GLUT display callback. Effectively calls displayViewport per-each Camera.
void display( void ) 
{
  static Cameras *camList = Engine::Instance()->Cams();
  
  // Clear the buffer.
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  // Tell camList to draw using our 'draw' rendering function.
  camList->View( draw );

  // Swap to the next buffer.
  glutSwapBuffers();

}

void idle( void ) 
{
  static Cameras *camList = Engine::Instance()->Cams();

  // Compute the time since last idle().
  Tick.Tock();

  // Move all camera(s).
  camList->IdleMotion();
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
  glutCreateWindow( "Particle Test" );
  //  glutFullScreen();
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
