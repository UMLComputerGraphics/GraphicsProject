#include "ColorFunctions.hpp"
#include "vec.hpp"
#include "mat.hpp"

#include <cmath>

using Angel::vec2;
using Angel::vec3;
using Angel::vec4;

vec4
ColorFunctions::flame( float lifePct, vec4 posIn ) {
  return vec4(1.0, 0.8, 0.0, lifePct );
}
