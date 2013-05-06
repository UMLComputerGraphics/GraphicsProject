/**
   @file soundHelper.cpp
   @author nick
   @brief helper methods and includes for use with fMod
**/

#ifndef __SOUNDHELPER_HPP
#define __SOUNDHELPER_HPP

#include "wincompat.h"
#include "fmod.hpp"
#include "fmod_errors.h"

#include "Camera.hpp"
#include "vec.hpp"


using namespace Angel ;
using namespace FMOD ;

namespace soundHelper
{

  extern const float DISTANCEFACTOR ;// Units per meter.  I.e feet would = 3.28. cm would = 100.
  extern FMOD_RESULT result;

  void fModInit( FMOD::System** );
  void add3dSound( const char*, FMOD::System*, FMOD::Sound**, bool);
  void play3dSound( FMOD_VECTOR pos, FMOD_VECTOR vel, FMOD::System*, FMOD::Channel**, FMOD::Sound*);

  /**
     overloaded version for MY convenience.
  */
  void play3dSound( vec4 pos, vec4 vel, FMOD::System*, FMOD::Channel**, FMOD::Sound*);
  
  /**
     very necessary user-defined error catch function for fMod
  */
  void ERRCHECK( ) ;


  /**
     this function accepts a vec4, discards the w value and returns a matching FMOD_VECTOR
     @param v a vec4 to convert
     @return a corresponding FMOD_VECTOR
  */
  FMOD_VECTOR convert_vec4_to_FMOD_VECTOR( vec4 ) ;

  void updateListener(Camera*, FMOD::System *);
  void updateListener(vec4, vec3, vec4, vec4, FMOD::System *);




}

#endif
