#ifndef __PARTICLE_COLOR_FUNCTIONS
#define __PARTICLE_COLOR_FUNCTIONS

#include "vec.hpp"

using Angel::vec3;
using Angel::vec4;

namespace ColorFunctions
{

  Angel::vec4 standard(float, Angel::vec4);
  Angel::vec4 flame(float, Angel::vec4);
  Angel::vec4 flameSt(float, Angel::vec4);

  Angel::vec4 aurora(float, Angel::vec4);
  Angel::vec4 HSV( float, Angel::vec4 );
  Angel::vec4 rainbow(float, Angel::vec4);
  Angel::vec4 tropical(float, Angel::vec4);
  Angel::vec4 galaxy(float, Angel::vec4);
}

#endif
