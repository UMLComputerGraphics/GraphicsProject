/**
   @file ds.cpp
   @author John Huston
   @authors ...
   @date 2013-02-20
   @brief Dual-shader demo.
   @details Work in progress!
   Based loosely on Ed Angel's tutorials.
**/

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
#include "Grinstein.hpp"
/* Utilities and Common */
#include "model.hpp"
#include "InitShader.hpp"
#include "glut_callbacks.h"

// Initialization: load and compile shaders, initialize camera(s), load models.
void init() {
  GLuint shader[2];
  Cameras *camlist = Grinstein::GetCameras();
  Scene *rootscene = Grinstein::GetScene();

  shader[0] = Angel::InitShader( "shaders/vred.glsl", "shaders/fragment.glsl" );
  shader[1] = Angel::InitShader( "shaders/vblu.glsl", "shaders/fragment.glsl" );
  
  std::cerr << "shader[0]: " << shader[0] << " shader[1]: " << shader[1] << "\n";
  std::cerr << "theScene.GetShader: " << Grinstein::GetScene()->GetShader() << "\n";

  camlist->SetShader( shader[0] );
  camlist->AddCamera( "Camera1" );
  camlist->Next();
  camlist->Active()->changePerspective( Camera::IDENTITY );

  // Fail.
  try {
    rootscene->AddObject( "Failure Object" );
  } catch ( std::exception &e ) {
    std::cerr << "Failed to add the object. Surprise.\n";
  }

  // Set without a default.
  Object *A = rootscene->AddObject( "Object A (RED)", shader[0] );

  // Test setting, then overriding, the default.
  rootscene->SetShader( shader[0] ); 
  Object *B = rootscene->AddObject( "Object B (BLU)", shader[1] );

  // Draw two squares:
  triangle(A, vec4(-1,0,0,1), vec4(0,0,0,1), vec4(-1,1,0,1), 0);
  triangle(A, vec4(0,0,0,1), vec4(-1,1,0,1), vec4(0,1,0,1), 0);
  A->Buffer();

  triangle(B, vec4(0,-1,0,1), vec4(1,-1,0,1), vec4(0,0,0,1), 0);
  triangle(B, vec4(1,-1,0,1), vec4(0,0,0,1), vec4(1,0,0,1), 0);
  B->Buffer();

  glEnable( GL_DEPTH_TEST );
  glClearColor( 0, 0, 0, 1.0 );
}

void cleanup( void ) {
  Grinstein::GetScene()->DestroyObject();
}

//--------------------------------------------------------------------

// Implementation of drawing the display with regards to a single viewport.
void draw( void ) {
  static Scene *theScene = Grinstein::GetScene();
  static Cameras *camList = Grinstein::GetCameras();

  theScene->Draw();
  camList->Draw();
}

// Display/Render the entire screen.
void display( void ) {
  static Cameras *camList = Grinstein::GetCameras();

  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  // Tell camList to draw using our 'draw' rendering function.
  camList->View( draw );

  glutSwapBuffers();
}

void idle( void ) {

  static Cameras *camList = Grinstein::GetCameras();

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
  //glutKeyboardFunc( keyboard );
  //glutKeyboardUpFunc( keylift );
  //glutSpecialFunc( keyboard_ctrl );
  //glutMouseFunc( mouse );
  //glutMotionFunc( mouseroll );
  //glutPassiveMotionFunc( mouselook );
  glutIdleFunc( idle );
  glutReshapeFunc( resizeEvent );

  /* PULL THE TRIGGER */
  glutMainLoop();
  return EXIT_SUCCESS;

}
