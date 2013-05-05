#ifndef __PARTICLE_FIELD_FUNCTIONS
#define __PARTICLE_FIELD_FUNCTIONS

#include "UserVectorField.hpp"
#include "vec.hpp"

using Angel::vec3;
using Angel::vec4;

namespace ParticleFieldFunctions
{

  Angel::vec3 tornado(Angel::vec4);
  Angel::vec3 tornadoDefault( Angel::vec4 );

  Angel::vec3 flame(Angel::vec4, Angel::vec3, double, float, float);
  Angel::vec3 flameDefault( Angel::vec4 );

  Angel::vec3 flameold(Angel::vec4);
  Angel::vec3 flameoldDefault( Angel::vec4 );

  Angel::vec3 fixed(Angel::vec4);
  Angel::vec3 fixedDefault( Angel::vec4 );

  Angel::vec3 up(Angel::vec4);
  Angel::vec3 upDefault( Angel::vec4 );

  /**
     Avoid setting the functions IN the callback.
     The function strings shouldn't be input to the callback,
     That'll cause re-compilations on every call and that's not the right way to use the library.
  Angel::vec3 userSupplied( Angel::vec4, const std::string &f_x = "0x + 0y + 0z + 0",
                                                    const std::string &f_y = "0x + 0y + 0z + 0.01",
                                                    const std::string &f_z = "0x + 0y + 0z + 0");
  */
  Angel::vec3 userSupplied( Angel::vec4 );
  Angel::vec3 userSupplied( Angel::vec4 &pos, UserVectorField &uvf );
}

#endif
