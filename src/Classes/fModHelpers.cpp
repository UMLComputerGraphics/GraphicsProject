/**

   helper functions for use with fMod
   nstpierr@cs.uml.edu

 */


#include "fModHelpers.hpp"
#include "vec.hpp"


#include "../fMod/include/wincompat.h"
#include "../fMod/include/fmod.hpp"
#include "../fMod/include/fmod_errors.h"

using Angel::vec3; 
using Angel::vec4; 


/**
   this function accepts a vec4, discards the w value and returns a matching FMOD_VECTOR          
   @param v a vec4 to convert
   @return a corresponding FMOD_VECTOR
*/
FMOD_VECTOR fModHelpers::convert_vec4_to_FMOD_VECTOR( vec4 v ){

  FMOD_VECTOR ret = { v.x, v.y, v.z } ;
  return ret ;

}

void fModHelpers::play3dSound( FMOD_VECTOR pos, FMOD_VECTOR vel, FMOD::Channel **a_channel, FMOD::Sound *a_sound, FMOD::System  *fSystem ) {

  /*    FMOD_VECTOR pos = { -10.0f * DISTANCEFACTOR, 0.0f, 0.0f };
	FMOD_VECTOR vel = {  0.0f, 0.0f, 0.0f }; 
  */

  FMOD_RESULT result ;

  

  result = fSystem->playSound(FMOD_CHANNEL_FREE, a_sound, true, a_channel);
  ERRCHECK(result);
  result = (*a_channel)->set3DAttributes(&pos, &vel);
  ERRCHECK(result);
  result = (*a_channel)->setPaused(false);
  ERRCHECK(result);

}

/**
   overloaded versions for your convenience: converting from vec4 to FMOD_VECTOR is gross
*/
void fModHelpers::play3dSound( vec4 pos, vec4 vel, FMOD::Channel **a_channel, FMOD::Sound *a_sound, FMOD::System  *fSystem ) {

  FMOD_VECTOR f_pos = convert_vec4_to_FMOD_VECTOR( pos );
  FMOD_VECTOR f_vel = convert_vec4_to_FMOD_VECTOR( vel );

  play3dSound(f_pos, f_vel, a_channel, a_sound, fSystem);

}

void fModHelpers::play3dSound( vec4 pos, vec3 vel, FMOD::Channel **a_channel, FMOD::Sound *a_sound, FMOD::System  *fSystem ) {

  vec4 vel4 = vec4(vel.x, vel.y, vel.z, 1.0 );

  play3dSound(pos, vel4, a_channel, a_sound, fSystem);

}

/**
   very necessary user-defined error catch function for fMod
*/
void fModHelpers::ERRCHECK(FMOD_RESULT result)
{
  if (result != FMOD_OK)
    {
      fprintf(stderr,"FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
      // exit(-1);
    }
}
