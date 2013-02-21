/**
   @file terrain.cpp
   @authors John Huston, Nicholas StPierre, Chris Compton
   @date 2012-12-06
   @brief This is a derivative of our main project file, fly.cpp.
   @details This is a tech demo for terrain generation using an udpated
   engine derived from fly.cpp, which was mostly based on Ed Angel's code
   from his book.
**/

// System Headers
#include <cmath>
#include <cstdio>
#include <sstream>
#include <cstdlib>
#include <time.h>
// Multi-platform support and OpenGL headers
#include "platform.h"
#include "globals.h"
// Ed Angel's Math Classes
#include "vec.hpp"
#include "mat.hpp"
// Utilities and Classes
#include "model.hpp"
#include "Camera.hpp"
#include "InitShader.hpp"
#include "Cameras.hpp"
#include "Screen.hpp"
#include "Object.hpp"
#include "Timer.hpp"
#include "Scene.hpp"

// Nick's Beta Lighting
//#include "LightSource.hpp
//#include "Lights.hpp"

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

// Application Globals
Screen myScreen( 800, 600 );
Scene theScene;
GLuint gShader;
bool fixed_yaw = true;

// #lights
//Lights lights(false); // Bool controls the lighting mode. False indicates the simpler, faster one.

// Textures
// Obtained from www.goodtextures.com
const char* terrainTex[] = {
  "../Textures/GoodTextures_0013423.jpg",  // Dirt/Mud
  "../Textures/GoodTextures_0013779.jpg",  // Sand
  "../Textures/GrassGreenTexture0002.jpg", // Grass (who'da thunk?)
  "../Textures/GoodTextures_0013418.jpg",  // Rock
  "../Textures/GoodTextures_0013291.jpg"   // Snow
};

/**
   randomize_terrain is called to regenerate the terrain in this application.
   It assumes the terrain is named "terrain" and is located as a direct
   child of the main scene graph.

   @return void.
**/
void randomize_terrain() {

  // Final project demo used the equivalent of terrain_size = 10.
  const size_t terrain_size = 8;
  float H = fmod((float)random(), 20 * terrain_size) + 5 * terrain_size;
  
  srand(time(NULL));

  Object *Terrain = theScene["terrain"];
  double magnitude = landGen( Terrain, terrain_size, H );
  Terrain->Buffer();
  GLint handle = glGetUniformLocation( gShader, "terrainMag" );
  if (handle != -1) glUniform1f( handle, magnitude );

}


/**
   init will initialize this particular flythrough,
   by creating and instantiating a shader, a camera,
   and a number of initial objects.

   @return void.
**/
void init() {

  
  // Load shaders. Give the Shader handle to the Scene Graph and the Camera List.
  gShader = Angel::InitShader( "shaders/vterrain.glsl", "shaders/fterrain.glsl" );
  theScene.SetShader( gShader );
  myScreen.camList.SetShader( gShader );

  // Cameras must be added after setting a shader.
  myScreen.camList.AddCamera( "Camera1" );
  myScreen.camList.Next();

  // initialize lights
  //lights.init_lights(gShader);

  /*
    NOTE:
    ==========================
    Please do not give the Terrain children.
    The terrain generation animation applies directly to the terrain,
    which of course propegates the animation to all child objects.

    You'll pancake any child objects.
  */

  // Let's create some objects.
  Object *terrain = theScene.AddObject( "terrain" );
  terrain->Texture( terrainTex );
  terrain->Mode( GL_TRIANGLE_STRIP );
  randomize_terrain(); // This call depends upon "terrain" existing within theScene.
  
  Object *pyramid = theScene.AddObject( "pyramid" );

/*
  Object *box   = theScene.AddObject( "box" ) ;
  loadModelFromFile( box, "../models/box-a.obj");
  box->trans.offset.SetY(100.0);
  box->trans.offset.SetX(100.0);
  box->Buffer();
*/

  Sierpinski_Pyramid( pyramid,
		      vec4(  0,      1,  0, 1 ),
		      vec4( -1, -0.999,  1, 1 ),
		      vec4(  1, -0.999,  1, 1 ),
		      vec4(  0, -0.999, -1, 1 ),
		      4 );
  pyramid->Buffer();

  Object *cube_base = theScene.AddObject( "basecube" );
  colorcube( cube_base, 1.0 );
  cube_base->Buffer();
  
  Object *moon_cube = pyramid->AddObject( "moon" );
  colorcube( moon_cube, 0.5 );
  moon_cube->Buffer();

  // These models came from VALVE,
  // From their game "Team Fortress 2."
  // The model processing was done in Blender.
  Object *heavy = theScene.AddObject( "heavy" );
  loadModelFromFile( heavy, "../models/heavyT.obj" );
  heavy->Buffer();
  heavy->trans.scale.Set( 0.10 );
  heavy->trans.offset.Set( 0, 2, 0 );

  // Valve's TF2 Medic
  Object *medic = heavy->AddObject( "medic" );
  loadModelFromFile( medic, "../models/medicT.obj" );
  medic->trans.offset.Set( 0, 20, 0 );
  medic->Buffer();

  // Valve's TF2 Spy
  Object *spy = medic->AddObject( "spy" );
  loadModelFromFile( spy, "../models/spyT.obj" );
  spy->trans.offset.Set( 0, 20, 0 );
  spy->Buffer();

  Object *ball = theScene.AddObject( "ball" );
  sphere( ball );
  ball->trans.scale.Set( 1000 );
  ball->Buffer();
  
  /*
  Object *sun = theScene.AddObject ("sun");
  //loadModelFromFile( sun, "../models/heavyT.obj" );
  sphere(sun);
  sun->trans.offset.SetX(500.0);
  sun->trans.scale.Set(16.0) ;
  sun->Buffer();
  */

  /*
  Object *actualMoon = theScene.AddObject ("actualMoon");
  //loadModelFromFile( actualMoon, "../models/spyT.obj" );
  sphere(actualMoon);
  actualMoon->trans.offset.SetX(-500.0);
  actualMoon->trans.scale.Set(12.0);
  actualMoon->Buffer();
  */

  //lights.addLightSource( LightSource( point4(0.0, 30.0, 0.0, 1.0), 				      color4(1.0, 1.0, 0.0, 1.0)));
  //lights.addLightSource( LightSource( point4(0.0, -1.0, 0.0, 1.0), 				      color4(1.0, 0.0, 1.0, 1.0)));
  //lights.addLightSource( LightSource( point4(10.0, 10.0, 10.0, 1.0), 				      color4(0.0, 1.0, 1.0, 1.0)));

  // The water gets generated last -- In order for our fake transparency to work.
  Object *agua = theScene.AddObject( "agua" );
  makeAgua( terrain, agua );
  agua->Buffer();
  agua->Mode( GL_TRIANGLES );

  glEnable( GL_DEPTH_TEST );
  glClearColor( 0.3, 0.5, 0.9, 1.0 );

  //Attach a model to the Camera.
  Object *cam = myScreen.camList.Active();
  loadModelFromFile( cam, "../models/rainbow_dashT.obj" );
  // http://kp-shadowsquirrel.deviantart.com/		
  //   art/Pony-Model-Download-Center-215266264
  cam->Buffer();
  cam->Mode( GL_TRIANGLES );
  cam->trans.scale.Set( 0.05 );
  cam->trans.PreRotation.RotateY( 180 );
  cam->Propegate(); 

  // Add the Propegate method to the Scene Graph directly, instead of this:
  // Note: Terrain doesn't/shouldn't have children ...
  terrain->Propegate();


}

  // Nick: Stop littering the global namespace everywhere in the mainfile,
  // Put them in the functions as statics or put them at the top of the file.
  float timeOfDay = 0.0; // If we are going for day and night

/**
   cleanup is a routine to call at exit time that will free up the
   resources the application is using.
   While not critical, it does aid in using debuggers to not have
   any memory leaks at exit time.
   
   @return void.
**/
void cleanup( void ) {

  theScene.DestroyObject();

}

/** 
    displayViewport is responsible for drawing a single viewport.

    @return void.
**/
void displayViewport( void ) {  
  theScene.Draw();         // Draw free-floating objects
  myScreen.camList.Draw(); // Draw camera-attached objects
}

/**
   display is responsible for drawing an entire screen.

   @return void.
**/
void display( void ) {
  // Clear the screen and begin rendering.
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  //lights.sendAll();
  // Tell camList to draw using our displayViewport rendering function.
  myScreen.camList.View( displayViewport );

  // Utilize multi-buffering.
  glutSwapBuffers();
}


/**
   keylift is registered as a GLUT callback for when a user releases a depressed key.
   
   @param key The key that was lifted.
   @param x The x coordinate of the mouse at the time the key was released.
   @param y The y coordinate of the mouse at the time the key was released.

   @return void.
**/
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


// This global switch controls if we generate terrain or not.
bool switchingTerrain = false ;

/**
   TerrainGenerationAnimation is an animation callback that will:
   (A) If triggered, begin to shrink the object into a flat plane,
   (B) Order a re-generation of the terrain data
   (C) will grow the object back into its new, final shape.

   @param obj A reference to an object's transformation state.

   @return void.
**/
void TerrainGenerationAnimation( TransCache &obj ) {

  typedef enum {
    SHRINKING,
    TRANSITIONING,
    GROWING,
    DONE
  } AnimationState;
  static AnimationState terrainState = DONE;
  static float CurrentScale = 0.0;


  if (switchingTerrain) {
    terrainState = SHRINKING;
    switchingTerrain = false;
    CurrentScale = 0.0;
  }

  switch ( terrainState ) {

  case SHRINKING:
    obj.scale.Set(1.0,
		  ((1.0+cos(CurrentScale*DegreesToRadians))/2.0),
		  1.0);
    
    CurrentScale += 1.0 * Tick.Scale() ;

    if ( CurrentScale >= 180.0 ) {
      terrainState = TRANSITIONING ;
    }
    break;

  case TRANSITIONING:
    // Multithread this for graphics 2 project
    randomize_terrain();
    terrainState = GROWING;
    break;

  case GROWING:
    obj.scale.Set(1.0,
		  ((1.0+cos(CurrentScale*DegreesToRadians))/2.0),
		  1.0);

    CurrentScale += 1.0 * Tick.Scale() ;

    if ( CurrentScale >= 360.0 ) terrainState = DONE ;
    break;

  case DONE:
    return;

  default: 
    return;
  }
}

/*
bool pushSun = false ;
void moveSun(TransCache &sun){

  switch ( sunState ) {

  case SHRINKING:

    sun.orbit.RotateZ( timeOfDay*0.1 );
    timeOfDay += 0.001 * Tick.Scale();
    if ( timeOfDay > 90.0 ) sunState = TRANSITIONING ;

    break;

  case TRANSITIONING:

    if ( pushSun ) {
      sunState = GROWING ;
      pushSun = false ;
    }

    return;

  case GROWING:

    sun.orbit.RotateZ( timeOfDay*0.1 ) ;
    timeOfDay += 0.001 * Tick.Scale( ) ;

    if ( timeOfDay > 360.0 ) sunState = DONE ;

    break;

  case DONE:

    if ( pushSun ) {
      sunState = SHRINKING ;
      pushSun = false ;
    }

    return;

  }


}
*/

/**
   keyboard is a callback registered with GLUT.
   It handles keyboard input.

   @param key The key pressed by the user.
   @param x The X coordinate of the mouse when the key was pressed.
   @param y The Y coordinate of the mouse when the key was pressed.
   
   @return void.
**/
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
    
  case 'l':
    // Trigger the Terrain Generation animation.
    switchingTerrain = true;
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

  //case 't': lights.addLightSource( randomLight() /*! not defined !*/ ) ; break;
  //case 't': pushSun = true; break;
  }
}


/**
   keyboard_ctrl is registered as a GLUT callback.
   It is responsible for catching when special keys are pressed.

   @param key The key pressed.
   @param x The X coordinate of the mouse when the key was pressed.
   @param y The Y coordinate of the mouse when the key was pressed.

   @return void.
**/
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


/**
   wiilook is an analog of mouselook, for wii remote controls.
   It takes a reference to a Camera, and two vec3s,
   and uses the information to adjust the Camera's rotation.

   @param WiiCamera The camera to adjust the rotation of.
   @param NewTheta  The X,Y,Z angles of the Wii Remote.
   @param MovementRates The X, Y, Z angular velocities of the Wii Remote.

   @return Void.
**/
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


/**
   resizeEvent is registered as a glut callback for when
   the screen is resized. It instructs the screen object
   of the new size, which informs all of the children cameras
   to recompute their aspect ratios, viewport positions, and so on.

   We also warp the pointer to the center of the screen, for
   compatibility with mouselook( void ).

   @param width The new width of the window.
   @param height The new height of the window.

   @return void.
**/
void resizeEvent( int width, int height ) {

  // Resize all of our cameras and recompute aspect ratios.
  myScreen.Size( width, height );

  // Warp cursor to center so that we don't have a big mouse jump.
  glutWarpPointer( myScreen.MidpointX(), myScreen.MidpointY() );

}


void simpleRotateY( TransCache &obj ) {
  obj.rotation.RotateY( Tick.Scale() * 1.5 ); 
}
void simpleRotateAnim( TransCache &obj ) {
  obj.rotation.RotateY( Tick.Scale() * 1.5 );
  obj.offset.Set( 1.5, 0, 0 );
  obj.orbit.RotateY( Tick.Scale() * -1.0 );
}

void animationTest( TransCache &obj ) {
  double timeScale = Tick.Scale();
  double theta = timeScale * 0.1;
  if (0) fprintf( stderr, "Timescale: %f\n", timeScale );

  //Object increasingly grows. 
  /* Note that, Scaling Adjustment, unlike Rotation and Translation,
     is a multiplicative adjustment, which means that
     we can't just multiply by our time scaling factor,
     we have to take pow( scaleFactor, timeScale ) instead.
     This is, of course, incredibly inefficient. */
  // obj.scale.Adjust( pow( 1.001, timeScale ) );

  //Object rotates in-place.
  obj.rotation.RotateX( theta );
  obj.rotation.RotateY( theta );
  obj.rotation.RotateZ( theta );

  obj.offset.Set( 5, 0, 0 );

  //Object increasingly moves away from origin, x += 0.01
  //obj.offset.Delta( timeScale * 0.01, 0, 0 );

  //Object orbits about the origin
  obj.orbit.RotateX( timeScale * 0.2 );
  obj.orbit.RotateY( timeScale * 0.2 );
  obj.orbit.RotateZ( timeScale * 0.2 );

  // Object moves its focal orbit-point, x = 5.
  //obj.displacement.Set( 5, 0, 0 ); 
}
/// hackity hack hack hackey doo!
float heightScale = 0.0;
float ticker = 0.0;

//###
//void sunOrbit(TransCache &sun ) {

//}

/*
void moonOrbit(TransCache &moon) {
  moon.orbit.RotateZ( timeOfDay*0.1 );
}
*/

void idle( void ) {

  Tick.Tock();

  //  if ( timeOfDay > 360.0 ) timeOfDay = 0.0;
  glClearColor( 
	       0.4*(sin(timeOfDay*DegreesToRadians)*sin(timeOfDay*DegreesToRadians)),
	       0.4,
	       0.45*(1.0+cos(timeOfDay*DegreesToRadians)),
	       1.0 );
  //  glClearColor( 0.0,0.0,0.1,1.0);

  if (DEBUG_MOTION) 
    fprintf( stderr, "Time since last idle: %lu\n", Tick.Delta() );


  Object &Terrain = *(theScene["terrain"]);
  Object &Pyramid = *(theScene["pyramid"]);
  Pyramid.Animation( animationTest );
  Pyramid["moon"]->Animation( simpleRotateAnim );

  Object &Heavy = *(theScene["heavy"]);
  Object &Medic = *(Heavy["medic"]);
  Object &Spy   = *(Medic["spy"]);
  Heavy.Animation( simpleRotateAnim );
  Medic.Animation( simpleRotateY );
  Spy.Animation( simpleRotateY );

  Terrain.Animation( TerrainGenerationAnimation );

  //Sun.Animation( moveSun ) ;// sunOrbit  );
  //Moon.Animation( moonOrbit );

  // Move light absolutely, ie to the point given
  //lights.moveLight( 0, Sun.GetPosition() ) ;

  // Move light absolutely, ie to the point given
  //lights.moveLight( 0, Sun.GetPosition() ) ;

  // lights hack
  //glUniform4fv( glGetUniformLocation(gShader, "sunHeight"), 1,
	//       Sun.GetPosition() );


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
    landGen( theScene["terrain"], 12, 40.0 );
    theScene["terrain"]->Buffer();
    break;
  case 1:
    if (fixed_yaw) fixed_yaw = false;
    else fixed_yaw = true;
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
  glutCreateWindow( "Terrain" );
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
