/**
   @file morph.cpp
   @author Nicholas St.Pierre
   @date 2013-1-26
   @brief This is the old morph demo with added keybinds to control morphing.
   @details This is a tech demo for morphing two objects back and forth.
   It uses the FModEX API to play sounds.
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

// FMOD includes for audio output
#include "../fMod/include/wincompat.h"
#include "../fMod/include/fmod.hpp"
#include "../fMod/include/fmod_errors.h"

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


/* use an enum to control which way we are morphing */
/* 'A' represents the 'first' model and 'B' represents the morph target*/

typedef enum { DONE,
	       DING,
	       TO_A_SMOOTH, 
	       TO_B_SMOOTH,
	       TO_A_LINEAR,
	       TO_B_LINEAR } morph_status ;

morph_status a_morph_status = DONE ;
bool pause_morph = false ;

/* end morphing globals blob */


/* begin fMod globals blob */

FMOD::System     *system ;
FMOD::Sound      *sound1, *sound2, *sound3 ;
FMOD::Channel    *channel = 0 ;
FMOD_RESULT       result ;

/* end fMod globals blob */

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

bool doQuake = false;


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

  case 'l': doQuake = !doQuake ; // toggle the quake effect @todo the quake effect


    // Morphing
  case 'p': // toggle pause
    pause_morph = !pause_morph;
    break ;


  case 'i': // to A - Smooth
    if ( a_morph_status == TO_A_LINEAR || a_morph_status == TO_B_LINEAR ) {
      ;
    }

    a_morph_status = TO_A_SMOOTH ;
    break ;


  case 'o': // to B - Smooth
    if ( a_morph_status == TO_A_LINEAR || a_morph_status == TO_B_LINEAR ) {
      ;
    }

    a_morph_status = TO_B_SMOOTH ;
    break ;


  case 'j': // to A - Linear
    if ( a_morph_status == TO_A_SMOOTH || a_morph_status == TO_B_SMOOTH ) {
      ;
    }

    a_morph_status = TO_A_LINEAR ;
    break ;


  case 'k': // to B - Linear
    if  ( a_morph_status == TO_A_SMOOTH || a_morph_status == TO_B_SMOOTH) {
      ;
    }

    a_morph_status = TO_B_LINEAR ;
    break ;


  }
}

void keyboard_ctrl( int key, int x, int y ) {

  // lazy try-catch to prevent unhandled Active() exceptions
  // this should only be around the theScene.Active() calls,
  // but I've got bigger fish to fry at the moment.

  try {

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
  catch(exception& e ) 
  {
    fprintf( stderr, "Exception caught in keyboard_ctrl():\n%s\n", e.what() );
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


/**

   drives the morphing of one and only one object
   @returns a float between 0 and 1 which represents the morph percentage

 */
float driveTheMorph(void){

  // yes, these oughta be static. they hold the morph state when pausing and time when switiching between moprhs.

  static double timer  = 0.0 ;  // This is a timer variable, used to control the morph transition.

  static morph_status previous_morph_status = DONE ; // this holds the previous morph status, 
                                                     // which is needed when switching between linear and smooth.

  static float percent = 0.0 ;  // Will send this value to the shader as the morph percentage:
                                // It varies from 0.0 to 1.0 where 1.0 is 100% and 0.0 is 0%

  if ( !pause_morph ) {

    switch ( a_morph_status ) {

      // timer goes from zero to one eighty ( half of a sine period )

    case DONE:
      break ;

    case DING:
      a_morph_status = DONE ;
      break;

    case TO_A_LINEAR:


      if ( (timer -= 0.05 ) <= 0.0 ) {
	// play a glass 'ding' sound before going to the DONE state:
	result = system->playSound(FMOD_CHANNEL_FREE, sound1, 0, &channel1);
	ERRCHECK(result);
	timer   = 0.0 ;
	percent = 0.0 ;
	a_morph_status = previous_morph_status = DONE ;
      }
      else percent = (timer/180.0) * Tick.Scale();
      break;

      
    case TO_B_LINEAR:

      if ( (timer += 0.05 ) >= 180.0 ) {
	result = system->playSound(FMOD_CHANNEL_FREE, sound2, 0, &channel2);
	ERRCHECK(result);
	timer   = 180.0 ;
	percent =   1.0 ;
	a_morph_status = previous_morph_status = DONE ;
	
      }
      else percent = (timer/180.0) * Tick.Scale();
      break;


    case TO_A_SMOOTH:

      if ( (timer -= 0.05 ) <= 0.0 ) {
	result = system->playSound(FMOD_CHANNEL_FREE, sound1, 0, &channel1);
	ERRCHECK(result);
	timer   = 0.0 ;
	percent = 0.0 ;
	a_morph_status = previous_morph_status = DONE ;
	
      }
      else percent = ((1.0-cos(timer*DegreesToRadians))/2.0) * Tick.Scale();

      break ;

      
    case TO_B_SMOOTH:

      if ( (timer += 0.05 ) >= 180.0 ) {
	result = system->playSound(FMOD_CHANNEL_FREE, sound2, 0, &channel2);
	ERRCHECK(result);
	timer   = 180.0 ;
	percent =   1.0 ;
	a_morph_status = previous_morph_status = DONE ;
      }
      else percent = ((1.0-cos(timer*DegreesToRadians))/2.0) * Tick.Scale();

      break ;

    }
  }

  return percent ;

}

/**
   this function accepts a vec4, discards the w value and returns a matching FMOD_VECTOR
   @param v a vec4 to convert
   @return a corresponding FMOD_VECTOR
 */
FMOD_VECTOR convert_vec4_to_FMOD_VECTOR( vec4& v ){

  FMOD_VECTOR ret = { v.x, v.y, v.z } ;
  return ret ;

}

/**  
     update the 3d sound stuff 
     @return Void.
*/
void updateListener( void ) {

    Camera &cam = *(myScreen.camList.Active());

  // ==========================================================================
  // UPDATE THE LISTENER
  // ==========================================================================

  // It looks like an FMOD_VECTOR is very, very similar to an Angel::vec3

  //static float t = 0;
    static FMOD_VECTOR lastpos   = { 0.0f, 0.0f, 0.0f } ;
    const  FMOD_VECTOR forward   = { 0.0f, 0.0f, 1.0f } ;
    const  FMOD_VECTOR up        = { 0.0f, 1.0f, 0.0f } ;
    FMOD_VECTOR vel;

    FMOD_VECTOR listenerpos = convert_vec4_to_FMOD_VECTOR( cam.pos() ) ;

    // ********* NOTE ******* READ NEXT COMMENT!!!!!
    // vel = how far we moved last FRAME (m/f), then time compensate it to SECONDS (m/s).

    /*
    vel.x = (listenerpos.x - lastpos.x) * (1000 / INTERFACE_UPDATETIME);
    vel.y = (listenerpos.y - lastpos.y) * (1000 / INTERFACE_UPDATETIME);
    vel.z = (listenerpos.z - lastpos.z) * (1000 / INTERFACE_UPDATETIME);
    */

    // store pos for next time
    lastpos = listenerpos;

    result = system->set3DListenerAttributes(0, &listenerpos, &vel, &forward, &up);
    ERRCHECK(result);

    //t += (30 * (1.0f / (float)INTERFACE_UPDATETIME));    // t is just a time value .. it increments in 30m/s steps in this example

////////////////////////////////////////////////////////////////////////////////

  }

  system->update();


  Sleep(INTERFACE_UPDATETIME - 1);



}

void idle( void ) {

  Tick.Tock();

  float percent = driveTheMorph();
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
  
#ifdef USE_FMOD

  updateListener();  // update the 3d sound stuff

  result = system->set3DListenerAttributes(0, &listenerpos, &vel, &forward, &up);
  ERRCHECK(result);


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


/**

   error checking function for FMod


 */
#ifdef USE_FMOD

  const int   INTERFACE_UPDATETIME = 50;      // 50ms update for interface
  const float DISTANCEFACTOR = 1.0f;          // Units per meter.  I.e feet would = 3.28.  centimeters would = 100.

/**
   very necessary user-defined error checking function
 */
void ERRCHECK(FMOD_RESULT result)
{
    if (result != FMOD_OK)
    {
      fprintf(stderr,"FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
        // exit(-1);
    }
}



/**
   init the fmod engine and setup sounds for 3d playback and/or register callbacks.
   this code was adapted from an example provided by the fmod team
 */
void fModInit(void){

  FMOD_RESULT result ; // used to hold the result value

  result = FMOD::System_Create(&system);
  ERRCHECK(result);

  result = system->getDriverCaps(0, &caps, 0, &speakermode);
  ERRCHECK(result);

  result = system->setSpeakerMode(speakermode);       /* Set the user selected speaker mode. */
  ERRCHECK(result);

  // copied from the example code, not sure what it does yet.
  if (caps & FMOD_CAPS_HARDWARE_EMULATED)
    /* The user has the 'Acceleration' slider set to off!  This is really bad for latency!. */
    {                                                   /* You might want to warn the user about this. */
      result = system->setDSPBufferSize(1024, 10);
      ERRCHECK(result);
    }

  result = system->getDriverInfo(0, name, 256, 0);
  ERRCHECK(result);

  if (strstr(name, "SigmaTel"))   /* Sigmatel sound devices crackle for some reason if the format is PCM 16bit.
				     PCM floating point output seems to solve it. */
    {
      result = system->setSoftwareFormat(48000, FMOD_SOUND_FORMAT_PCMFLOAT, 0,0, FMOD_DSP_RESAMPLER_LINEAR);
      ERRCHECK(result);
    }

  result = system->init(100, FMOD_INIT_NORMAL, 0);
  if (result == FMOD_ERR_OUTPUT_CREATEBUFFER)         /* Ok, the speaker mode selected isn't supported by this soundcard.  
							 Switch it back to stereo... */
    {
      result = system->setSpeakerMode(FMOD_SPEAKERMODE_STEREO);
      ERRCHECK(result);
                
      result = system->init(100, FMOD_INIT_NORMAL, 0);/* ... and re-init. */
      ERRCHECK(result);
    }


  //  Set the distance units. (meters/feet etc).

  result = system->set3DSettings(1.0, DISTANCEFACTOR, 1.0f);
  ERRCHECK(result);

  // load some sounds
  result = system->createSound("../sounds/ding1.wav", FMOD_3D, 0, &sound1);
  ERRCHECK(result);
  result = sound1->set3DMinMaxDistance(0.5f * DISTANCEFACTOR, 5000.0f * DISTANCEFACTOR);
  ERRCHECK(result);
  result = sound1->setMode(FMOD_LOOP_OFF);
  ERRCHECK(result);


  result = system->createSound("../sounds/ding2.wav", FMOD_3D, 0, &sound2);
  ERRCHECK(result);
  result = sound1->set3DMinMaxDistance(0.5f * DISTANCEFACTOR, 5000.0f * DISTANCEFACTOR);
  ERRCHECK(result);
  result = sound1->setMode(FMOD_LOOP_OFF);
  ERRCHECK(result);

  result = system->createSound("../sounds/heavy_yell9.wav", FMOD_3D, 0, &sound3);
  ERRCHECK(result);
  result = sound1->set3DMinMaxDistance(0.5f * DISTANCEFACTOR, 5000.0f * DISTANCEFACTOR);
  ERRCHECK(result);
  result = sound1->setMode(FMOD_LOOP_OFF);
  ERRCHECK(result);

  /*
        Play sounds at certain positions
  */
  {
    FMOD_VECTOR pos = { -10.0f * DISTANCEFACTOR, 0.0f, 0.0f };
    FMOD_VECTOR vel = {  0.0f, 0.0f, 0.0f };

    result = system->playSound(FMOD_CHANNEL_FREE, sound1, true, &channel1);
    ERRCHECK(result);
    result = channel1->set3DAttributes(&pos, &vel);
    ERRCHECK(result);
    result = channel1->setPaused(false);
    ERRCHECK(result);
  }

  {
    FMOD_VECTOR pos = { 15.0f * DISTANCEFACTOR, 0.0f, 0.0f };
    FMOD_VECTOR vel = { 0.0f, 0.0f, 0.0f };

    result = system->playSound(FMOD_CHANNEL_FREE, sound2, true, &channel2);
    ERRCHECK(result);
    result = channel2->set3DAttributes(&pos, &vel);
    ERRCHECK(result);
    result = channel2->setPaused(false);
    ERRCHECK(result);
  }

  {
    FMOD_VECTOR pos = { 45.0f * DISTANCEFACTOR, 0.0f, 0.0f };
    FMOD_VECTOR vel = { 0.0f, 0.0f, 0.0f };

    result = system->playSound(FMOD_CHANNEL_FREE, sound3, true, &channel3);
    ERRCHECK(result);
    result = channel3->set3DAttributes(&pos, &vel);
    ERRCHECK(result);
    result = channel3->setPaused(false);
    ERRCHECK(result);
  }


}

#endif

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

  // fMod is the audio library


  glutInit( &argc, argv );
  glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
  glutInitWindowSize( myScreen.Width(), myScreen.Height() );
  glutCreateWindow( "Change This" );
  glutFullScreen();
  glutSetCursor( GLUT_CURSOR_NONE );

  GLEW_INIT();
  init();

#ifdef USE_FMOD
  fModInit(); // Init the fmod sound engine
#endif

  /* Register our Callbacks */
  glutDisplayFunc( display );
  glutKeyboardFunc( keyboard ); // Use the local one! The generic one doesn't have morph controls.
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


  // Fmod cleanup
  // Unsure if this will ever be executed but...
  ERRCHECK(sound1->release());
  ERRCHECK(sound2->release());
  ERRCHECK(sound3->release());

  ERRCHECK(system->close()  );
  ERRCHECK(system->release());


  return EXIT_SUCCESS;

}
