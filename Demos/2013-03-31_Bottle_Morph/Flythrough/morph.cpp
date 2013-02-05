/**
   @file morph.cpp
   @author Nicholas St.Pierre
   @authors John Huston, Nicholas VerVoort, Chris Compton
   @date 2012-12-06
   @brief This is a derivative of our main project file, fly.cpp.
   @details This is a tech demo for morphing two objects back and forth.
   This is mostly based on ed angel's code from his book.
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

#include "OpenGL.h"

// Type Aliases
using Angel::vec3;
using Angel::vec4;
typedef Angel::vec4 color4;
typedef Angel::vec4 point4;

// Wii Connectivity 
#ifdef WII
#include <wiicpp.h>  // WiiC++ API
#include "WiiUtil.h" // Wii Controller Handler Util
CWii Wii;
bool usingWii = false;
#endif
////

Screen myScreen( 800, 600 );
Scene theScene;
GLuint gShader;
bool fixed_yaw = true;

void init() {

  
  // Load shaders and use the resulting shader program. 
  gShader = Angel::InitShader( "shaders/vmorph.glsl", "shaders/fmorph.glsl" );
  theScene.SetShader( gShader );
  myScreen.camList.SetShader( gShader );
  myScreen.camList.AddCamera( "Camera1" );
  myScreen.camList.Next();


  // Make ya models here

  Object *bottle = theScene.AddObject( "bottle" );
  loadModelFromFile( bottle, "../models/bottle-a.obj" ); // loadModelFromFile( bottle_a, "../models/bottle-a.obj" );
  bottle->trans.scale.Set( 0.01 );
  bottle->Buffer();

  // Objects has-a pointer to an object which is their "morph target."
  // they are created and buffered as follows:

  bottle->genMorphTarget( gShader ) ; // this makes a new object and links it to the source object. it returns the addr of the new obj..
  Object *bottleMorphTarget = bottle->getMorphTargetPtr() ; // we can get the addr of the morph object like this, also.
    loadModelFromFile( bottleMorphTarget, "../models/bottle-b.obj" ); // with this model, we can use all the preexisting Object class functionality
  //loadModelFromFile( bottle->getMorphTargetPtr(), "../models/bottle-b.obj" ); // with this model, we can use all the preexisting Object class functionality
  bottleMorphTarget->trans.scale.Set( 0.01 );
  bottle->BufferMorphOnly(); // YES THIS IS THE REAL OBJECT, NOT THE TARGET. IT SENDS THE MORPH VERTICES TO THE SHADER, NOT TO THE DRAW LIST TO BE DRAWN!

  /*
  Object *heavy     = theScene.AddObject( "heavy" ) ;
  loadModelFromFile( heavy, "../models/heavyT.obj" );
  heavy->trans.scale.Set(2.0);
  heavy->Buffer();


  heavy->genMorphTarget( gShader ) ;
  loadModelFromFile( heavy->getMorphTargetPtr(), "../models/medicT.obj" );
  heavy->getMorphTargetPtr()->trans.scale.Set(2.0);
  heavy->BufferMorphOnly();
  */


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
  // myScreen.camList.Draw(); /* Draw camera-attached objects */
}

void display( void ) {
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  // lights.sendAll();

  // Tell camList to draw using our displayViewport rendering function.
  myScreen.camList.View( displayViewport );
  glutSwapBuffers();
}

void keylift( unsigned char key, int x, int y ) {

  if (myScreen.camList.NumCameras() < 1) return;

  Camera &cam = *(myScreen.camList.Active());

  switch( key ) {
  case 'w':
    cam.Stop( Camera::Forward );
    break;
  case 's':
    cam.Stop( Camera::Backward );
    break;
  case 'a':
    cam.Stop( Camera::Left );
    break;
  case 'd':
    cam.Stop( Camera::Right );
    break;
  case 'q':
    cam.Stop( Camera::Up );
    break;
  case 'e':
    cam.Stop( Camera::Down );
    break;
  }
}


void keyboard( unsigned char key, int x, int y ) {

  // Hacky, for the wii reset, below.
  Camera *camptr = dynamic_cast< Camera* >( myScreen.camList["AutoCamera2"] );

  switch( key ) {

  case 033: // Escape Key	  
    cleanup();
    glutLeaveMainLoop();
    break;

  case ';': // Print Info
    fprintf( stderr, "Active Object: %s\n",
	     theScene.Active()->Name().c_str() );
    break;

  case '~':
#ifdef WII
    CalibrateGyro( Wii );
    if (camptr) camptr->resetRotation();
#endif
    break;
    
    
  case '+':
    std::stringstream camName;
    camName << "AutoCamera" << myScreen.camList.NumCameras() + 1;
    myScreen.camList.AddCamera( camName.str() );
    break;
  }

  if (myScreen.camList.NumCameras() < 1) return;

  /* A shorthand variable with local scope that refers to "The Active Camera." */
  Camera &cam = *(myScreen.camList.Active());
  
  switch( key ) {
  case '-':
    myScreen.camList.PopCamera();
    break;
  case ';':
    fprintf( stderr, "Camera Position: (%f,%f,%f)\n", cam.X(), cam.Y(), cam.Z() );
    break;
    
  case 'w':
    cam.Move( Camera::Forward );
    break;
  case 's':
    cam.Move( Camera::Backward );
    break;
  case 'a':
    cam.Move( Camera::Left );
    break;
  case 'd':
    cam.Move( Camera::Right );
    break;
  case 'q':
    cam.Move( Camera::Up );
    break;
  case 'e':
    cam.Move( Camera::Down );
    break;
    
    //Perspectives
  case 'z': cam.changePerspective( Camera::PERSPECTIVE ); break;
  case 'x': cam.changePerspective( Camera::ORTHO ); break;
  case 'c': cam.changePerspective( Camera::ORTHO2D ); break;
  case 'v': cam.changePerspective( Camera::FRUSTUM ); break;
  case 'b': cam.changePerspective( Camera::IDENTITY ); break;
    //  case '[': theScene["bottle"]->setMorphPercentage(0.0); break;
    //  case ']': theScene["bottle"]->setMorphPercentage(1.0); break;

  }
}

void keyboard_ctrl( int key, int x, int y ) {

  switch (key) {
    //Cycle between Active Objects ...
  case GLUT_KEY_LEFT:
    theScene.Prev();
    break;
  case GLUT_KEY_RIGHT:
    theScene.Next();
    break;

    //Change the Draw Mode ...
  case GLUT_KEY_F1:
    theScene.Active()->Mode( GL_POINTS );
    break;
  case GLUT_KEY_F2:
    theScene.Active()->Mode( GL_LINE_STRIP );
    break;
  case GLUT_KEY_F3:
    theScene.Active()->Mode( GL_TRIANGLE_STRIP );
    break;
  case GLUT_KEY_F4:
    theScene.Active()->Mode( GL_TRIANGLES );
    break;
  }

  // If there are no Cameras, don't muck around with this section.
  if (myScreen.camList.NumCameras() < 1) return;

  switch( key ) {
  case GLUT_KEY_PAGE_UP:
    myScreen.camList.Prev();
    break;

  case GLUT_KEY_PAGE_DOWN:
    myScreen.camList.Next();
    break;
  }
}

void mouse( int button, int state, int x, int y ) {

  if (myScreen.camList.NumCameras() < 1) return;

  if ( state == GLUT_DOWN ) {
    switch( button ) {
    case 3: myScreen.camList.Active()->dFOV( 1 ); break;
    case 4: myScreen.camList.Active()->dFOV( -1 ); break;
    }
  }

}


void mouseroll( int x, int y ) {

  if ((x != myScreen.MidpointX()) || (y != myScreen.MidpointY())) {
    if (myScreen.camList.NumCameras() > 0)
      myScreen.camList.Active()->roll( x - myScreen.MidpointX() );
    glutWarpPointer( myScreen.MidpointX(), myScreen.MidpointY() );
  }

}


void wiilook( Camera &WiiCamera, const Angel::vec3 &NewTheta,
	      const Angel::vec3 &MovementRates ) {

  static Angel::vec3 OldTheta; /* Defaults to 0,0,0 */  
  // Rotation Order: Y-X-Z looks the best, I think.
  //WiiCamera.yaw( NewTheta.y - OldTheta.y );
  //WiiCamera.pitch( OldTheta.x - NewTheta.x );
  //WiiCamera.roll( NewTheta.z - OldTheta.z );

  float yaw = -MovementRates.y / 20;
  float pitch = -MovementRates.x / 20;
  float roll = MovementRates.z / 20;

  if (abs(yaw) >= 0.1)
    WiiCamera.yaw( -MovementRates.y / 20, fixed_yaw );
  if (abs(pitch) >= 0.1)
    WiiCamera.pitch( -MovementRates.x / 20 );
  if (abs(roll) >= 0.1)
    WiiCamera.roll( MovementRates.z / 20 );

  OldTheta = NewTheta;

}

void mouselook( int x, int y ) {

  if ((x != myScreen.MidpointX()) || (y != myScreen.MidpointY())) {
    const double dx = ((double)x - myScreen.MidpointX());
    const double dy = ((double)y - myScreen.MidpointY());
    
    if (myScreen.camList.NumCameras() > 0) {
      myScreen.camList.Active()->pitch( dy );
      myScreen.camList.Active()->yaw( dx, fixed_yaw );
    }    

    glutWarpPointer( myScreen.MidpointX(), myScreen.MidpointY() );
  }
  
}


void resizeEvent( int width, int height ) {

  /* Handles resizing the child cameras as well. */
  myScreen.Size( width, height );
  glutWarpPointer( myScreen.MidpointX(), myScreen.MidpointY() );

}


void simpleRotateAnim( TransCache &obj ) {

  obj.rotation.RotateY( Tick.Scale() * 1.5 );
  obj.offset.Set( 1.5, 0, 0 );
  obj.orbit.RotateY( Tick.Scale() * -1.0 );

}



void idle( void ) {

  Tick.Tock();

  static double timer = 0.0 ;

  if ( (timer += 0.005 ) > 360.0 ) timer = 0.0 ;

  float percent = ( sin(timer) + 1 ) / 2 ;

  theScene["bottle"]->setMorphPercentage(percent);
  //  theScene["heavy"]->setMorphPercentage(percent);


  if (DEBUG_MOTION) 
    fprintf( stderr, "Time since last idle: %lu\n", Tick.Delta() );



#ifdef WII
  if (usingWii) {
    static const unsigned NumPolls = 20;
    Camera *camptr = dynamic_cast< Camera* >( myScreen.camList["AutoCamera2"] );
    Angel::vec3 theta_diff;
    Angel::vec3 accel_mag;

    // Take many samples for two reasons:
    // (1) Without this, we can't poll often enough and Wii Input "lags".
    // (2) Average/Sample to "smooth" the data.
    for (size_t i = 0; i < NumPolls; ++i) {
      pollWii( Wii );
      if (PollResults.Reset_Camera && camptr != NULL) camptr->resetRotation();
      theta_diff += PollResults.wr_thetas;
      accel_mag += PollResults.bb_magnitudes;
    }

    //Angel::vec3 normal_accel = accel_mag / NumPolls;
    //fprintf( stderr, "bbXL: (%f,%f,%f)\n", normal_accel.x, normal_accel.y, normal_accel.z );

    if (camptr) {
      camptr->Accel( (accel_mag / NumPolls) * 2.0 );
      wiilook( *camptr, theta_diff / NumPolls, PollResults.wr_rates );
    }

  }
#endif
  
  // Move all camera(s).
  myScreen.camList.IdleMotion();
  glutPostRedisplay();

}


//--------------------------------------------------------------------

void menufunc( int value ) {

  switch (value) {
  case 0:
    break;
  case 1:
    break;
  }

}

int main( int argc, char **argv ) {

#ifdef WII
  if (!(usingWii = initWii( Wii ))) {
    std::cerr << "Not using Wii controls for this runthrough.\n";
  }
#endif

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
  glutCreateWindow( "Change This" );
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

  if (DEBUG) {
    fprintf( stderr, "GL_VENDOR: %s\n", glGetString( GL_VENDOR ));
    fprintf( stderr, "GL_RENDERER: %s\n", glGetString( GL_RENDERER ));
    fprintf( stderr, "GL_VERSION: %s\n", glGetString( GL_VERSION ));
    fprintf( stderr, "GL_SHADING_LANGUAGE_VERSION: %s\n", 
	     glGetString( GL_SHADING_LANGUAGE_VERSION ));
  }

  int menu = glutCreateMenu( menufunc );
  glutSetMenu( menu );
  glutAddMenuEntry( "Randomize Terrain", 0 );
  glutAddMenuEntry( "Toggle Free Rotation", 1 );
  glutAttachMenu( GLUT_RIGHT_BUTTON );

  /* PULL THE TRIGGER */
  glutMainLoop();
  return EXIT_SUCCESS;

}
