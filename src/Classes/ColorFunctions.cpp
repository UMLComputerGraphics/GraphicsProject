#include "ColorFunctions.hpp"
#include "vec.hpp"
#include "mat.hpp"

#include <cmath>

using Angel::vec2;
using Angel::vec3;
using Angel::vec4;

vec4
ColorFunctions::flame( float lifePct, vec4 posIn ) {
  float r, g, b, w;
  
  if( lifePct >= 0.7 ) {
    r = 0.4;
    g = 0.6;
    b = 0.8;
    w = 1 - lifePct;
  } else {
    r = lifePct + 0.25;
    g = lifePct/2;
    b = 0.0;
    w = 1.0;
  }
  
  //return 2*normalize(vec4(r, g, b, w ));

  return vec4(2*normalize(vec3(r, g, b)), w);
}
