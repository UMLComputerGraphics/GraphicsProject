#ifndef __PARTICLE_FIELD_FUNCTIONS
#define __PARTICLE_FIELD_FUNCTIONS

#include "vec.hpp"

using Angel::vec3;
using Angel::vec4;

namespace ParticleFieldFunctions
{

  Angel::vec3 tornado(Angel::vec4);
  Angel::vec3 flame(Angel::vec4);
  Angel::vec3 flameold(Angel::vec4);
  Angel::vec3 fixed(Angel::vec4);
  Angel::vec3 up(Angel::vec4);
  Angel::vec3 userSupplied( Angel::vec4 );

}

#endif
