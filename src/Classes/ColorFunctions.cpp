#include "ColorFunctions.hpp"
#include "vec.hpp"
#include "mat.hpp"

#include <cmath>

using Angel::vec2;
using Angel::vec3;
using Angel::vec4;

vec4 ColorFunctions::standard( float lifePct, vec4 posIn ) {
  return vec4( 1.0, 1.0, 1.0, 1.0 );
}

vec4 ColorFunctions::flame( float lifePct, vec4 posIn ) {
  float r, g, b, w;
  
  if( lifePct >= 0.7 ) {
    r = 0.4;
    g = 0.6;
    b = 0.8;
    w = (1 - lifePct)/2;
  } else {
    r = lifePct + 0.3;
    g = lifePct/2 + 0.05;
    b = 0.05;
    w = lifePct + 0.2;
  }
  
  //return 2*normalize(vec4(r, g, b, w ));

  return vec4(2*normalize(vec3(r, g, b)), w);
}

vec4 ColorFunctions::flameSt( float lifePct, vec4 posIn ) {
  float r, g, b, w;
  
  if( lifePct >= 0.82 ) {
    r = 1-lifePct+0.3;
    g = 1-lifePct+0.5;
    b = 0.8;

  } else {
    r = 1/(1.1+lifePct) ;//+ 0.25;
    g = 1/(1.8+lifePct) ;//+ 0.15;
    b = 1/(1.5+lifePct) ;//+ 0.20;

  }

  w = 0.1+(2*lifePct*(1-lifePct));

  return vec4(r, g, b, w);

}
