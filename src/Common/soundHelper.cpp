/**
   @file soundHelper.cpp
   @author nick
   @brief helper methods and include for use with fMod
**/


#include "soundHelper.hpp"

#include "vec.hpp"

#include "Util.hpp"
#include "Camera.hpp"
#include "Timer.hpp"

//const float DISTANCEFACTOR = 100.0f;// Units per meter.  I.e feet would = 3.28. cm would = 100.

using namespace Angel;
using namespace FMOD;

const float soundHelper::DISTANCEFACTOR = 6.56f;// Units per meter.  I.e feet would = 3.28. cm would = 100.
FMOD_RESULT soundHelper::result;

/**
 this function accepts a vec4, discards the w value and returns a matching FMOD_VECTOR
 @param v a vec4 to convert
 @return a corresponding FMOD_VECTOR
 */
FMOD_VECTOR soundHelper::convert_vec4_to_FMOD_VECTOR( vec4 v ) {

  FMOD_VECTOR ret = { v.x, v.y, v.z };
  return ret;

}


void soundHelper::add3dSound( const char* filename, FMOD::System* theSystem,  
			      FMOD::Sound** theSound, bool loopOn)
{

  printf("FMODILE = %s\n", filename);

  soundHelper::result = theSystem->createSound( Util::getRelativePath(filename).c_str(), FMOD_3D, 0, theSound );
  ERRCHECK( );

  soundHelper::result = (*theSound)->set3DMinMaxDistance( 0.5f * DISTANCEFACTOR,
					     5000.0f * DISTANCEFACTOR );
  ERRCHECK( );

  soundHelper::result = (loopOn) ? (*theSound)->setMode( FMOD_LOOP_NORMAL ) 
                    : (*theSound)->setMode( FMOD_LOOP_OFF );
  ERRCHECK( );

}

void soundHelper::fModInit( FMOD::System **fSystem ) {

  FMOD_SPEAKERMODE speakermode;
  FMOD_CAPS caps;
  char name[256];
  
  soundHelper::result = FMOD::System_Create( fSystem );
  ERRCHECK( );
  
  soundHelper::result = (*fSystem)->getDriverCaps( 0, &caps, 0, &speakermode );
  ERRCHECK( );
  
  soundHelper::result = (*fSystem)->setSpeakerMode( speakermode ); /* Set the user selected speaker mode. */
  ERRCHECK( );
  
  // copied from the example code, not sure what it does yet.

  if ( caps & FMOD_CAPS_HARDWARE_EMULATED )
  /* The user has the 'Acceleration' slider set to off!  This is really bad for latency!. */
  { /* You might want to warn the user about this. */
    soundHelper::result = (*fSystem)->setDSPBufferSize( 1024, 10 );
    ERRCHECK( );
  }

  soundHelper::result = (*fSystem)->getDriverInfo( 0, name, 256, 0 );
  ERRCHECK( );

  if ( strstr( name, "SigmaTel" ) ) /* Sigmatel sound devices crackle for some reason if the format is PCM 16bit.
				       PCM floating point output seems to solve it. */
  {
    soundHelper::result = (*fSystem)->setSoftwareFormat( 48000, FMOD_SOUND_FORMAT_PCMFLOAT, 0,
                                         0, FMOD_DSP_RESAMPLER_LINEAR );
    ERRCHECK( );
  }
  
  soundHelper::result = (*fSystem)->init( 100, FMOD_INIT_NORMAL, 0 );
  if ( soundHelper::result == FMOD_ERR_OUTPUT_CREATEBUFFER ) /* Ok, the speaker mode selected isn't supported by this soundcard.
						   Switch it back to stereo... */
  {
    soundHelper::result = (*fSystem)->setSpeakerMode( FMOD_SPEAKERMODE_STEREO );
    ERRCHECK();
    
    soundHelper::result = (*fSystem)->init( 100, FMOD_INIT_NORMAL, 0 );/* ... and re-init. */
    ERRCHECK();
  }
  
  //  Set the distance units. (meters/feet etc).
  soundHelper::result = (*fSystem)->set3DSettings( 1.0, DISTANCEFACTOR, 1.0f );
  ERRCHECK();

}

void soundHelper::play3dSound( FMOD_VECTOR pos, 
			       FMOD_VECTOR vel, 
			       FMOD::System *fSystem,
			       FMOD::Channel **a_channel,
			       FMOD::Sound *a_sound ) {

  soundHelper::result = fSystem->playSound( FMOD_CHANNEL_FREE, a_sound, true, a_channel );
  ERRCHECK();
  soundHelper::result = (*a_channel)->set3DAttributes( &pos, &vel );
  ERRCHECK();
  soundHelper::result = (*a_channel)->setPaused( false );
  ERRCHECK();

}

/**
 overloaded version for your convenience.
 */
void soundHelper::play3dSound( vec4 pos, vec4 vel, 
			       FMOD::System *fSystem,
			       FMOD::Channel **a_channel,
			       FMOD::Sound *a_sound ) {

  FMOD_VECTOR f_pos = convert_vec4_to_FMOD_VECTOR( pos );
  FMOD_VECTOR f_vel = convert_vec4_to_FMOD_VECTOR( vel );

  play3dSound( f_pos, f_vel, fSystem, a_channel, a_sound );

}

/**
 very necessary user-defined error catch function for fMod
 */
void soundHelper::ERRCHECK( ) {
  if ( soundHelper::result != FMOD_OK ) {
    gprint( PRINT_ERROR, "FMOD error! (%d) %s\n", soundHelper::result,
            FMOD_ErrorString( soundHelper::result ) );
  }
}



void soundHelper::updateListener(Camera* cam, FMOD::System *fSystem )
{
  updateListener(cam->pos(), cam->dPos(), cam->forward(), cam->up(), fSystem );
}


void soundHelper::updateListener(vec4 pos,     vec3 vel, 
				 vec4 forward, vec4 up,
				 FMOD::System *fSystem )
{

  FMOD_VECTOR FVforward = convert_vec4_to_FMOD_VECTOR( forward );
  FMOD_VECTOR FVup = convert_vec4_to_FMOD_VECTOR( -up );// = { 0.0f, 1.0f, 0.0f } ;
  
  FMOD_VECTOR FVvel = convert_vec4_to_FMOD_VECTOR( vel );
  FMOD_VECTOR FVlistenerpos = convert_vec4_to_FMOD_VECTOR( pos );


  // ********* NOTE ******* READ NEXT COMMENT!!!!!
  // vel = how far we moved last FRAME (m/f), then time compensate it to SECONDS (m/s).
  
  /*
   #ifdef _RT
   framesToSeconds = (float)(timeSinceLastFrame / SecToNSec);
   #else
   framesToSeconds = (float)(timeSinceLastFrame / SecTouSec);
   #endif
   */

  float framesToSeconds = tick.scale() / 1250;

  FVvel.x *= framesToSeconds;
  FVvel.y *= framesToSeconds;
  FVvel.z *= framesToSeconds;
  
  gprint( PRINT_VERBOSE, "we appear to be operating at "
          "%f frames per second \n",
          framesToSeconds );

  fSystem->set3DListenerAttributes( 0, &FVlistenerpos, 
				    &FVvel, &FVforward,
				    &FVup );
  ERRCHECK();
  
  fSystem->update();
  
}

