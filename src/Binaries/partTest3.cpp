/**
   @file dumb.cpp
   @author Nicholas St.Pierre
   @authors John Huston, Nicholas VerVoort, Chris Compton
   @date 2013-02-23
   @details This is a tech demo for a particle system
*/

#include "Engine.hpp"
#include "ParticleSystem.hpp"
#include "ParticleFieldFunctions.hpp"
/* Utilities and Common */
#include "glut_callbacks.h"
#include "ObjLoader.hpp"
#include "InitShader.hpp"
#include "model.hpp"
#include "mat.hpp"
/* System Headers */
#include <cmath>
#include <cstdio>
#include <sstream>
#include <cstdlib>
#include <time.h>

/* fmod includes */
/* john don't look at how ugly this is */
#include "../fMod/include/wincompat.h"
#include "../fMod/include/fmod.hpp"
#include "../fMod/include/fmod_errors.h"

// variable used to initialize the particle system
// If there is an argv[1], we will use it to initialize the particle system.
int numberOfParticles = 99999 ;


const float DISTANCEFACTOR = 100.0f;          // Units per meter.  I.e feet would = 3.28. cm would = 100.


// Type Aliases
using   Angel::vec3;
using   Angel::vec4;
typedef Angel::vec4 color4;
typedef Angel::vec4 point4;

// Global objects for magical camera success
Screen myScreen( 800, 600 );
bool fixed_yaw = false;


/* begin fMod globals blob */

FMOD::System     *fSystem ;
FMOD::Sound      *ding1, *ding2, *heavy, *bg ;
FMOD::Channel    *bottleChannel=0, *randomChannel=0, *bgChannel=0;
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
     NOT RELATIVE TO THE EXECUTABLE'S PATH.  */
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

  result = fSystem->createSound("../sounds/BN-ForeverEndless.wav", FMOD_SOFTWARE, 0, &bg);
  ERRCHECK(result);
  result = bg->setMode(FMOD_LOOP_ON);
  ERRCHECK(result);

  {
    result = fSystem->playSound(FMOD_CHANNEL_FREE, bg, true, &bgChannel);
    ERRCHECK(result);
    result = bgChannel->setPaused(false);
    ERRCHECK(result);
  }


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
void init() 
{

  GLuint  particleSystemShader, bottleShader;
  Screen *primScreen = Engine::instance()->mainScreen();
  Scene  *rootScene  = Engine::instance()->rootScene();

  /*
  bottleShader         = Angel::InitShader("shaders/vmorph.glsl",
					   "shaders/fmorph.glsl");
  */

  particleSystemShader = Angel::InitShader("shaders/vParticle2.glsl",
					   "shaders/fParticle2.glsl",
					   "shaders/gParticle.glsl", //"shaders/gParticle.glsl",
					   GL_POINTS,
					   GL_TRIANGLE_STRIP,
					   12 );

  rootScene->shader(particleSystemShader);
  primScreen->_camList.shader(particleSystemShader);

  primScreen->_camList.addCamera( "Camera1" );
  primScreen->_camList.next();

  /*
  {
    Object *bottle = rootScene->addObject( "bottle" );
    ObjLoader::loadModelFromFile( bottle, "../models/bottle-a.obj" );
    bottle->_trans._scale.set( 0.01 );
    bottle->buffer();
  }
  */

  {
    ParticleSystem *particleSystem = new ParticleSystem( numberOfParticles, 
							 "ParticleSystem1b", 
							 particleSystemShader );

    particleSystem->setLifespan(4.0, 8.0);
    particleSystem->setVectorField( ParticleFieldFunctions::flame ) ;
    particleSystem->setEmitterRadius( 0.02 ) ;
    particleSystem->drawMode( GL_TRIANGLES ) ; // NEED THIS IF WE USE A GEOMETRY SHADER!
    rootScene->insertObject( particleSystem );
    //particleSystem->setSlaughterHeight(0.2455);
    //particleSystem->_trans._displacement.set(0.0, 0.25, 0.0);
    //particleSystem->setEmitterRadius( 0.1 );
    particleSystem->propagate();
    //particleSystem->fillSystemWithParticles();
    particleSystem->buffer();
  }

  // Generic OpenGL setup: Enable the depth buffer and set a nice background color.
  glEnable( GL_DEPTH_TEST );
  glClearColor( 0.0, 0.0, 0.0, 1.0 );

  // also need this to render visible points
  glPointSize( 1.1 );
}

void cleanup( void ) 
{
  Engine::instance()->rootScene()->delObject();
}

//----------------------------------------------------------------------------------------

void draw( void )
{
  static Scene *theScene  = Engine::instance()->rootScene();
  static Cameras *camList = Engine::instance()->cams();

  theScene->draw();
  camList->draw();
}

// GLUT display callback. Effectively calls displayViewport per-each Camera.
void display( void ) 
{
  static Cameras *camList = Engine::instance()->cams();
  
  // Clear the buffer.
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  // Tell camList to draw using our 'draw' rendering function.
  camList->view( draw );

  // Swap to the next buffer.
  glutSwapBuffers();

}

void idle( void ) 
{
  static Cameras *camList = Engine::instance()->cams();

  // Compute the time since last idle().
  tick.tock();

  updateListener();  // update the 3d sound stuff

  // Move all camera(s).
  camList->idleMotion();
  glutPostRedisplay();
}


void updateListener( void ) {

  Camera &cam = *(Engine::instance()->mainScreen()->_camList.active());// *(myScreen.camList.Active());

  // It looks like an FMOD_VECTOR is very, very similar to an Angel::vec3

  //static float t = 0;
  //static FMOD_VECTOR lastpos   = { 0.0f, 0.0f, 0.0f } ;
  vec4 forwardVector;
  FMOD_VECTOR forward   = convert_vec4_to_FMOD_VECTOR( cam.forward() ) ;
  FMOD_VECTOR up        = convert_vec4_to_FMOD_VECTOR( cam.up() ) ; // = { 0.0f, 1.0f, 0.0f } ;
  //unsigned long timeSinceLastFrame = tick.Delta();
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

  framesToSeconds = 60 * tick.scale();
  //framesToSeconds = tick.Rate() * tick.scale();

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

  if ( argc == 2 )
    numberOfParticles = atoi(argv[1]);
  else
    std::cerr << "****************************************************************" << std::endl
	      << "* PLEASE RUN THIS PROGRAM WITH A NUMBER AS ITS FIRST ARGUMENT! *" << std::endl
	      << "****************************************************************" << std::endl;

  // OS X suppresses events after mouse warp.  This resets the suppression 
  // interval to 0 so that events will not be suppressed. This also found
  // at http://stackoverflow.com/questions/728049/
  // glutpassivemotionfunc-and-glutwarpmousepointer
#ifdef __APPLE__
  CGSetLocalEventsSuppressionInterval( 0.0 );
#endif
  Util::InitRelativePaths(argc, argv);

  glutInit( &argc, argv );
  glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
  glutInitWindowSize( myScreen.width(), myScreen.height() );
  glutCreateWindow( "Particle Test" );
  glutFullScreen();
  glutSetCursor( GLUT_CURSOR_NONE );

  fModInit();
  GLEW_INIT();
  init();

  /* Register our Callbacks */
  glutDisplayFunc( display );
  glutKeyboardFunc( engineKeyboard );
  glutKeyboardUpFunc( engineKeylift );
  glutSpecialFunc( engineSpecialKeyboard );
  glutMouseFunc( engineMouse );
  glutMotionFunc( engineMouseMotion );
  glutPassiveMotionFunc( EngineMousePassive );
  glutIdleFunc( idle );
  glutReshapeFunc( engineResize );

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
