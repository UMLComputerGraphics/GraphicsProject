/**
   @file morphkeysound.cpp
   @author Nicholas St.Pierre
   @authors John Huston, Nicholas VerVoort, Chris Compton
   @date 2012-12-06
   @brief This is a derivative of our main project file, fly.cpp.
   @details This is a tech demo for morphing two objects back and forth,
   with keybinds, and sound.
   This is mostly based on ed angel's code from his book.
**/



/* 
   TO DO's. (JUMP ON IN KIDDOS)
   @todos: - move all this into a singleton, similar to our Engine object
           - make a fMod_helper.h and move all the fmod functions there. (maybe use a namespace?)
           - integrate fmod into the engine???????????????
	   - make a heavy orbit the bottle and have it make noise
*/


#ifndef USE_FMOD
#define USE_FMOD
#endif

const float DISTANCEFACTOR = 100.0f;          // Units per meter.  I.e feet would = 3.28. cm would = 100.


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

/* fmod includes */
#include "../fMod/include/wincompat.h"
#include "../fMod/include/fmod.hpp"
#include "../fMod/include/fmod_errors.h"


/* use an enum to control which way we are morphing */
/* 'A' represents the 'first' model and 'B' represents the morph target*/

typedef enum { DONE,
               TO_A_SMOOTH,
               TO_B_SMOOTH,
               TO_A_LINEAR,
               TO_B_LINEAR } morph_status ;

morph_status a_morph_status = DONE ;
bool pause_morph = false ;

/* end morphing globals blob */


/* begin fMod globals blob */


FMOD::System     *fSystem ;
FMOD::Sound      *ding1, *ding2, *heavy ;
FMOD::Channel    *bottleChannel=0, *randomChannel=0;
FMOD_RESULT       result ; // doesn't need to be a global... @todo: make local where appropriate
void updateListener( void ) ;
void ERRCHECK(FMOD_RESULT result);
void fModInit(void);

/**
   this function accepts a vec4, discards the w value and returns a matching FMOD_VECTOR
   @param v a vec4 to convert
   @return a corresponding FMOD_VECTOR
*/
FMOD_VECTOR convert_vec4_to_FMOD_VECTOR( vec4 v ){

  FMOD_VECTOR ret = { v.x, v.y, v.z } ;
  return ret ;

}

/**
     update the 3d sound stuff, initialize fModEx
     @return Void.
*/

void fModInit(void){

  FMOD_SPEAKERMODE speakermode;
  FMOD_CAPS        caps;
  char             name[256];

  FMOD_RESULT result ; // used to hold the result value
  result = FMOD::System_Create(&fSystem);
  ERRCHECK(result);

  result = fSystem->getDriverCaps(0, &caps, 0, &speakermode);
  ERRCHECK(result);

  result = fSystem->setSpeakerMode(speakermode);       /* Set the user selected speaker mode. */
  ERRCHECK(result);

  // copied from the example code, not sure what it does yet.

  if (caps & FMOD_CAPS_HARDWARE_EMULATED)
    /* The user has the 'Acceleration' slider set to off!  This is really bad for latency!. */
    {                                                   /* You might want to warn the user about this. */
      result = fSystem->setDSPBufferSize(1024, 10);
      ERRCHECK(result);
    }

  result = fSystem->getDriverInfo(0, name, 256, 0);
  ERRCHECK(result);

  if (strstr(name, "SigmaTel"))   /* Sigmatel sound devices crackle for some reason if the format is PCM 16bit.
                                     PCM floating point output seems to solve it. */
    {
      result = fSystem->setSoftwareFormat(48000, FMOD_SOUND_FORMAT_PCMFLOAT, 0,0, FMOD_DSP_RESAMPLER_LINEAR);
      ERRCHECK(result);
    }

  result = fSystem->init(100, FMOD_INIT_NORMAL, 0);
  if (result == FMOD_ERR_OUTPUT_CREATEBUFFER)         /* Ok, the speaker mode selected isn't supported by this soundcard.
                                                         Switch it back to stereo... */
    {
      result = fSystem->setSpeakerMode(FMOD_SPEAKERMODE_STEREO);
      ERRCHECK(result);

      result = fSystem->init(100, FMOD_INIT_NORMAL, 0);/* ... and re-init. */
      ERRCHECK(result);
    }


 //  Set the distance units. (meters/feet etc).
  result = fSystem->set3DSettings(1.0, DISTANCEFACTOR, 1.0f);
  ERRCHECK(result);

  // load some sounds

  /* IF YOU GET A FILE NOT FOUND PROBLEM, 
     KNOW THAT THESE ARE RELATIVE TO THE DIRECTORY YOU ARE EXECUTING FROM,
     NOT RELATIVE TO THE EXECUTABLE'S PATH.
  */


  result = fSystem->createSound("../sounds/ding1.wav", FMOD_3D, 0, &ding1);
  ERRCHECK(result);
  result = ding1->set3DMinMaxDistance(0.5f * DISTANCEFACTOR, 5000.0f * DISTANCEFACTOR);
  ERRCHECK(result);
  result = ding1->setMode(FMOD_LOOP_OFF);
  ERRCHECK(result);


  result = fSystem->createSound("../sounds/ding2.wav", FMOD_3D, 0, &ding2);
  ERRCHECK(result);
  result = ding2->set3DMinMaxDistance(0.5f * DISTANCEFACTOR, 5000.0f * DISTANCEFACTOR);
  ERRCHECK(result);
  result = ding2->setMode(FMOD_LOOP_OFF);
  ERRCHECK(result);

  result = fSystem->createSound("../sounds/heavy_yell9.wav", FMOD_3D, 0, &heavy);
  ERRCHECK(result);
  result = heavy->set3DMinMaxDistance(0.5f * DISTANCEFACTOR, 5000.0f * DISTANCEFACTOR);
  ERRCHECK(result);
  result = heavy->setMode(FMOD_LOOP_OFF);
  ERRCHECK(result);

  /*
        don't init the channels.
  */
  /*
  {
    FMOD_VECTOR pos = { -10.0f * DISTANCEFACTOR, 0.0f, 0.0f };
    FMOD_VECTOR vel = {  0.0f, 0.0f, 0.0f };

    result = fSystem->playSound(FMOD_CHANNEL_FREE, ding1, true, &channel1);
    ERRCHECK(result);
    result = channel1->set3DAttributes(&pos, &vel);
    ERRCHECK(result);
    result = channel1->setPaused(false);
    ERRCHECK(result);
  }

  {
    FMOD_VECTOR pos = { 15.0f * DISTANCEFACTOR, 0.0f, 0.0f };
    FMOD_VECTOR vel = { 0.0f, 0.0f, 0.0f };

    result = fSystem->playSound(FMOD_CHANNEL_FREE, ding2, true, &channel2);
    ERRCHECK(result);
    result = channel2->set3DAttributes(&pos, &vel);
    ERRCHECK(result);
    result = channel2->setPaused(false);
    ERRCHECK(result);
  }

  {
    FMOD_VECTOR pos = { 45.0f * DISTANCEFACTOR, 0.0f, 0.0f };
    FMOD_VECTOR vel = { 0.0f, 0.0f, 0.0f };

    result = fSystem->playSound(FMOD_CHANNEL_FREE, sound3, true, &channel3);
    ERRCHECK(result);
    result = channel3->set3DAttributes(&pos, &vel);
    ERRCHECK(result);
    result = channel3->setPaused(false);
    ERRCHECK(result);
  }
  */


}


void play3dSound( FMOD_VECTOR pos, FMOD_VECTOR vel, FMOD::Channel **a_channel, FMOD::Sound *a_sound ) {

/*    FMOD_VECTOR pos = { -10.0f * DISTANCEFACTOR, 0.0f, 0.0f };
      FMOD_VECTOR vel = {  0.0f, 0.0f, 0.0f }; */

  result = fSystem->playSound(FMOD_CHANNEL_FREE, a_sound, true, a_channel);
  ERRCHECK(result);
  result = (*a_channel)->set3DAttributes(&pos, &vel);
  ERRCHECK(result);
  result = (*a_channel)->setPaused(false);
  ERRCHECK(result);

}

/**

   overloaded version for your convenience.

 */
void play3dSound( vec4 pos, vec4 vel, FMOD::Channel **a_channel, FMOD::Sound *a_sound ) {

  FMOD_VECTOR f_pos = convert_vec4_to_FMOD_VECTOR( pos );
  FMOD_VECTOR f_vel = convert_vec4_to_FMOD_VECTOR( vel );

  play3dSound(f_pos, f_vel, a_channel, a_sound);

}

/**
   very necessary user-defined error catch function for fMod
*/
void ERRCHECK(FMOD_RESULT result)
{
  if (result != FMOD_OK)
    {
      fprintf(stderr,"FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
      // exit(-1);
    }
}

/* end fMod globals blob */

// Initialization: load and compile shaders, initialize camera(s), load models.
void init() {
  
  // Get handles to the Scene and the Screen.
  Scene *rootScene = Engine::instance()->rootScene();
  Screen *primScreen = Engine::instance()->mainScreen();

  // Load shaders and use the resulting shader program. 
  GLuint gShader = Angel::InitShader( "shaders/vmorph.glsl", "shaders/fmorph.glsl" );

  // Let the other objects know which shader to use by default.
  rootScene->Shader( gShader );
  primScreen->_camList.Shader( gShader );

  // We start with no cameras, by default. Add one and set it "active" by using Next().
  primScreen->_camList.addCamera( "Camera1" );
  primScreen->_camList.next();

  // Create an object and add it to the scene with the name "bottle".
  Object *bottle = rootScene->AddObject( "bottle" );

  // Use the object loader to actually fill out the vertices and-so-on of the bottle.
  loadModelFromFile( bottle, "../models/bottle-a.obj" );

  // Scale the bottle down!
  bottle->trans.scale.Set( 0.01 );

  // Buffer the object onto the GPU. This does not happen by default,
  // To allow you to make many changes and buffer only once,
  // or to buffer changes selectively.
  bottle->Buffer();

  // Object class has-a pointer to an object which is the morph target.
  // they are created and buffered as follows:

  // this makes a new object and links it to the source object. it returns the addr of the new obj..
  bottle->genMorphTarget( gShader ) ; 

  // we can get the addr of the morph object like this, also.
  Object *bottleMorphTarget = bottle->getMorphTargetPtr() ; 

  // with this model, we can use all the preexisting Object class functionality
  loadModelFromFile( bottleMorphTarget, "../models/bottle-b.obj" ); 
  bottleMorphTarget->trans.scale.Set( 0.01 );

  // YES THIS IS THE REAL OBJECT, NOT THE TARGET. 
  // IT SENDS THE MORPH VERTICES TO THE SHADER, NOT TO THE DRAW LIST TO BE DRAWN!
  bottle->BufferMorphOnly(); 

  // Generic OpenGL setup: Enable the depth buffer and set a nice background color.
  glEnable( GL_DEPTH_TEST );
  glClearColor( 0.3, 0.5, 0.9, 1.0 );

}

void cleanup( void ) {
  Engine::instance()->rootScene()->DestroyObject();
}

//--------------------------------------------------------------------

// Implementation of drawing the display with regards to a single viewport.
void draw( void ) {
  static Scene *theScene = Engine::instance()->rootScene();
  static Cameras *camList = Engine::instance()->cams();

  theScene->Draw();
  camList->Draw();
}

// GLUT display callback. Effectively calls displayViewport per-each Camera.
void display( void ) {
  static Cameras *camList = Engine::instance()->cams();

  // Clear the buffer.
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  // Tell camList to draw using our 'draw' rendering function.
  camList->view( draw );

  // Swap to the next buffer.
  glutSwapBuffers();

}


float driveTheMorph(void){

  // yes, these oughta be static. they hold the morph state when pausing and time when switiching between moprhs.

  static double timer  = 0.0 ;  // This is a timer variable, used to control the morph transition.

  static morph_status previous_morph_status = DONE ; // this holds the previous morph status,
                                                     // which is needed when switching between linear and smooth.

  static float percent = 0.0 ;  // Will send this value to the shader as the morph percentage:
                                // It varies from 0.0 to 1.0 where 1.0 is 100% and 0.0 is 0%

#ifndef DegreesToRadians
  const float DegreesToRadians = M_PI/180.0;
#endif

  if ( !pause_morph ) {

    switch ( a_morph_status ) {

      // timer goes from zero to one eighty ( half of a sine period )

    case DONE:
      break ;


    case TO_A_LINEAR:


      if ( (timer -= 0.05 ) <= 0.0 ) {
        timer   = 0.0 ;
        percent = 0.0 ;
        a_morph_status = previous_morph_status = DONE ;
        // play a glass 'ding' sound before going to the DONE state:
	play3dSound( vec4(0.0,0.0,0.0,1.0),
		     vec4(0.0,0.0,0.0,1.0),
		     &bottleChannel,
		     ding1 );
      }
      else percent = (timer/180.0) * Tick.Scale();
      break;


      /*

void play3dSound( vec4 pos, vec4 vel, FMOD::Channel *a_channel, FMOD::Sound *a_sound );


       */

    case TO_B_LINEAR:

      if ( (timer += 0.05 ) >= 180.0 ) {
        timer   = 180.0 ;
        percent =   1.0 ;
        a_morph_status = previous_morph_status = DONE ;
	play3dSound( vec4(0.0,0.0,0.0,1.0),
		     vec4(0.0,0.0,0.0,1.0),
		     &bottleChannel,
		     ding2 );
      }
      else percent = (timer/180.0) * Tick.Scale();
      break;

   case TO_A_SMOOTH:

      if ( (timer -= 0.05 ) <= 0.0 ) {
        timer   = 0.0 ;
        percent = 0.0 ;
        a_morph_status = previous_morph_status = DONE ;
	play3dSound( vec4(0.0,0.0,0.0,1.0),
		     vec4(0.0,0.0,0.0,1.0),
		     &bottleChannel,
		     ding1 );
      }
      else percent = ((1.0-cos(timer*DegreesToRadians))/2.0) * Tick.Scale();

      break ;


    case TO_B_SMOOTH:

      if ( (timer += 0.05 ) >= 180.0 ) {
        timer   = 180.0 ;
        percent =   1.0 ;
        a_morph_status = previous_morph_status = DONE ;
	play3dSound( vec4(0.0,0.0,0.0,1.0),
		     vec4(0.0,0.0,0.0,1.0),
		     &bottleChannel,
		     ding2 );
      }
      else percent = ((1.0-cos(timer*DegreesToRadians))/2.0) * Tick.Scale();

      break ;

    }
  }

  return percent ;

}


void idle( void ) {

  static Cameras *camList = Engine::instance()->cams();
  static Scene *rootScene = Engine::instance()->rootScene();

  // Compute the time since last idle().
  // This is a global, stateful operation.
  Tick.Tock();

  // Animation variables.
  float percent = driveTheMorph();
  (*rootScene)["bottle"]->setMorphPercentage(percent);
  //(*rootScene)["heavy"]->setMorphPercentage(percent);


  // Update the morph percentage.


  if (DEBUG_MOTION) 
    fprintf( stderr, "Time since last idle: %lu\n", Tick.Delta() );

  // Move all cameras: Apply velocity and acceleration adjustments.
  // If no cameras are currently moving, this will do nothing ;)
  camList->idleMotion();


#ifdef USE_FMOD
  updateListener();  // update the 3d sound stuff
#endif

  // Inform GLUT we'd like to render a new frame.
  glutPostRedisplay();

}


/**

   A hackey keyboard handling function.
   so uh, yeah. this is sort of a filter function. It will catch presses to
   the 'p', 'i', 'o', 'j', and 'k' keys, and let all others go be sent to the 
   keyboard() function defined in glut_callbacks.cpp

   @todo: write a more permanant solution, and put it in glut_callbacks.cpp

 */
void keyboard_morph(unsigned char key, int x, int y) {

  // Morphing
  switch( key ) {

  case 'p': // toggle pause
    pause_morph = !pause_morph;
    break ;


  case 'i': // to A - Smooth
    if ( a_morph_status == TO_A_LINEAR || a_morph_status == TO_B_LINEAR ) {
      ; // @todo: fix jumpiness when switching modes
    }
    
    a_morph_status = TO_A_SMOOTH ;
    break ;
    

  case 'o': // to B - Smooth
    if ( a_morph_status == TO_A_LINEAR || a_morph_status == TO_B_LINEAR ) {
      ; // @todo: fix jumpiness when switching modes
    }

    a_morph_status = TO_B_SMOOTH ;
    break ;


  case 'j': // to A - Linear
    if ( a_morph_status == TO_A_SMOOTH || a_morph_status == TO_B_SMOOTH ) {
      ; // @todo: fix jumpiness when switching modes    
    }

    a_morph_status = TO_A_LINEAR ;
    break ;


  case 'k': // to B - Linear
    if  ( a_morph_status == TO_A_SMOOTH || a_morph_status == TO_B_SMOOTH) {
      ; // @todo: fix jumpiness when switching modes
    }


  default:
    keyboard( key, x, y ) ;

  }

}


void updateListener( void ) {

  Camera &cam = *(Engine::instance()->mainScreen()->_camList.active());// *(myScreen.camList.Active());

  // It looks like an FMOD_VECTOR is very, very similar to an Angel::vec3

  //static float t = 0;
  //static FMOD_VECTOR lastpos   = { 0.0f, 0.0f, 0.0f } ;
  vec4 forwardVector;
  FMOD_VECTOR forward   = convert_vec4_to_FMOD_VECTOR( cam.forward() ) ;
  FMOD_VECTOR up        = convert_vec4_to_FMOD_VECTOR( cam.up() ) ; // = { 0.0f, 1.0f, 0.0f } ;
  //unsigned long timeSinceLastFrame = Tick.Delta();
  float framesToSeconds;

  FMOD_VECTOR vel;
  FMOD_VECTOR listenerpos;

  listenerpos = convert_vec4_to_FMOD_VECTOR( cam.pos() ) ;
  vel = convert_vec4_to_FMOD_VECTOR( cam.dPos() ) ;

  // ********* NOTE ******* READ NEXT COMMENT!!!!!
  // vel = how far we moved last FRAME (m/f), then time compensate it to SECONDS (m/s).

  /*
#ifdef _RT
    framesToSeconds = (float)(timeSinceLastFrame / SecToNSec);
#else
    framesToSeconds = (float)(timeSinceLastFrame / SecTouSec);
#endif
  */

  framesToSeconds = 60 * Tick.Scale();
  //framesToSeconds = Tick.Rate() * Tick.Scale();

  vel.x *= framesToSeconds ;
  vel.y *= framesToSeconds ;
  vel.z *= framesToSeconds ;

  if ( DEBUG ) {

    fprintf(stderr, "we appear to be operating at %f frames per second \n", framesToSeconds );

  }

  /*
    vel.x = (listenerpos.x - lastpos.x) * (1000 / INTERFACE_UPDATETIME);
    vel.y = (listenerpos.y - lastpos.y) * (1000 / INTERFACE_UPDATETIME);
    vel.z = (listenerpos.z - lastpos.z) * (1000 / INTERFACE_UPDATETIME);
  */
  // store pos for next time
  //lastpos = listenerpos;

  result = fSystem->set3DListenerAttributes(0, &listenerpos, &vel, &forward, &up);
  ERRCHECK(result);

  // don't know what the point of this variable was.
  //t += (30 * (1.0f / (float)INTERFACE_UPDATETIME));    // t is just a time value .. it increments in 30m/s steps in this example

  fSystem->update();

  //Sleep(INTERFACE_UPDATETIME - 1);

}



/*
FMOD::System     *fSystem ;
FMOD::Sound      *dingl, *ding2, *heavy ;
FMOD::Channel    *bottleChannel=0, *randomChannel=0;
FMOD_RESULT       result ; // doesn't need to be a global... @todo: make local where appropriate

 */
int main( int argc, char **argv ) {

  fprintf(stderr,"This file is executing from the following directory\nall relative paths are relative to THIS directory:\n");
  system("pwd");

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

  fModInit();
  GLEW_INIT();
  init();

  /* Register our Callbacks */
  glutDisplayFunc( display );
  glutKeyboardFunc( keyboard_morph );
  glutKeyboardUpFunc( keylift );
  glutSpecialFunc( keyboard_ctrl );
  glutMouseFunc( mouse );
  glutMotionFunc( mouseroll );
  glutPassiveMotionFunc( mouselook );
  glutIdleFunc( idle );
  glutReshapeFunc( resizeEvent );

  /* PULL THE TRIGGER */
  glutMainLoop();

  // Fmod cleanup
  // Unsure if this will ever be executed but...
  ERRCHECK(ding1->release());
  ERRCHECK(ding2->release());
  ERRCHECK(heavy->release());

  ERRCHECK(fSystem->close()  );
  ERRCHECK(fSystem->release());



  return EXIT_SUCCESS;

}