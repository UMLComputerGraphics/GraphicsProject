#ifndef __FMOD_HELPERS_HPP
#define __FMOD_HELPERS_HPP

#include "vec.hpp"

#include "../fMod/include/wincompat.h"
#include "../fMod/include/fmod.hpp"
#include "../fMod/include/fmod_errors.h"

using Angel::vec3; 
using Angel::vec4; 

namespace fModHelpers {

  
  /**
     this function accepts a vec4, discards the w value and returns a matching FMOD_VECTOR          
     @param v a vec4 to convert
     @return a corresponding FMOD_VECTOR
  */
  FMOD_VECTOR convert_vec4_to_FMOD_VECTOR( vec4 v );

  
  void play3dSound( FMOD_VECTOR pos, FMOD_VECTOR vel,
		    FMOD::Channel **a_channel, FMOD::Sound *a_sound, FMOD::System *fSystem  ) ;

  /**
     overloaded versions for your convenience. 
  */
  void play3dSound( vec4 pos, vec4 vel, 
		    FMOD::Channel **a_channel, FMOD::Sound *a_sound, FMOD::System *fSystem  ) ;
  
  void play3dSound( vec4 pos, vec3 vel,
		    FMOD::Channel **a_channel, FMOD::Sound *a_sound, FMOD::System *fSystem  ) ;
  

  /**
     very necessary user-defined error catch function for fMod
  */
  void ERRCHECK(FMOD_RESULT result);


}

#endif
