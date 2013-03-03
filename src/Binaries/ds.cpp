/**
   @file ds.cpp
   @author John Huston
   @authors John Huston, Greg Giannone
   @date 2013-02-20
   @brief Dual-shader demo.
   @details Work in progress!
   Based loosely on Ed Angel's tutorials.
**/
#include <SOIL.h>
/* Multi-platform support and OpenGL headers. */
#include "globals.h"
#include "platform.h"
/* Engine Classes */
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

// Initialization: load and compile shaders, initialize camera(s), load models.
void init() {

  GLuint shader[3];
  Cameras *camList = Engine::Instance()->Cams();
  Scene *rootScene = Engine::Instance()->RootScene();

  shader[0] = Angel::InitShader( "shaders/vred.glsl", "shaders/fragment.glsl" );
  shader[1] = Angel::InitShader( "shaders/vblu.glsl", "shaders/fragment.glsl" );
  shader[2] = Angel::InitShader( "shaders/vtex.glsl", "shaders/ftex.glsl" );

  //camList->Shader( shader[0] );
  camList->AddCamera( "Camera1" );
  camList->Next();
  camList->Active()->changePerspective( Camera::IDENTITY );

  // Adding objects without a default shader:
  Object *A = rootScene->AddObject( "Object A (RED)", shader[0] );

  // Setting a default and adding objects without:
  rootScene->Shader( shader[1] ); 
  Object *B = rootScene->AddObject( "Object B (BLU)" );

  // Third Object, over-ride default shader.
  Object *C = rootScene->AddObject( "Object C (TEX)", shader[1] );

  // Draw two squares:
  triangle(A, vec4(-1,0,0,1), vec4(0,0,0,1), vec4(-1,1,0,1), 0);
  triangle(A, vec4(0,0,0,1), vec4(-1,1,0,1), vec4(0,1,0,1), 0);
  A->Buffer();

  triangle(B, vec4(0,-1,0,1), vec4(1,-1,0,1), vec4(0,0,0,1), 0);
  triangle(B, vec4(1,-1,0,1), vec4(0,0,0,1), vec4(1,0,0,1), 0);
  B->Buffer();

  triangle(C, vec4(-1,-1,0,1), vec4(0,-1,0,1), vec4(-1,0,0,1), 0);
  C->texcoords.push_back( vec2(0,0) );
  C->texcoords.push_back( vec2(1,1) );
  C->texcoords.push_back( vec2(0,1) );
  C->Buffer();

  //GLint tex2ddirt = txload_w( "../Textures/GrassGreenTexture0002.jpg" );

  glEnable( GL_DEPTH_TEST );
  glClearColor( 0, 0, 0, 1.0 );
}

void cleanup( void ) {
  Engine::Instance()->RootScene()->DestroyObject();
}

//--------------------------------------------------------------------

// Implementation of drawing the display with regards to a single viewport.
void draw( void ) {
  static Scene *theScene = Engine::Instance()->RootScene();
  static Cameras *camList = Engine::Instance()->Cams();


  theScene->Draw();
  camList->Draw();
}

// Display/Render the entire screen.
void display( void ) {
  static Cameras *camList = Engine::Instance()->Cams();

  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  // Tell camList to draw using our 'draw' rendering function.
  camList->View( draw );

  glutSwapBuffers();
}

void idle( void ) {

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
  //glutMouseFunc( mouse );
  //glutMotionFunc( mouseroll );
  //glutPassiveMotionFunc( mouselook );
  glutIdleFunc( idle );
  glutReshapeFunc( resizeEvent );

  /* PULL THE TRIGGER */
  glutMainLoop();
  return EXIT_SUCCESS;

}
