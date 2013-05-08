#include "ColorFunctions.hpp"
#include "vec.hpp"
#include "mat.hpp"

#include "ParticleSystem.hpp"

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

vec4 ColorFunctions::aurora( float lifePct, vec4 posIn ) {
  float r, g, b, w;
  
  r = sin(1-lifePct*4);
  g = cos(1-posIn.y);
  b = 0.5;
  w = sin(lifePct*4);

  return vec4(r, g, b, w);
}

float scaleOverRange( float min, float max, float pct ) {
  return pct / (max - min);
}

vec4 ColorFunctions::HSV( float lifePct, vec4 posIn ) {
  
  float hue = lifePct;

  static const float S = 1.0;
  static const float V = 1.0;
  static const float M = 0.0;
  float H, F, N, I;
  int i;
  float &K = F;

  H = (hue * 6.0);
  I = floor( H );
  F = (H - I);
  N = (1.0 - F);
  
  i = (int)I;
  
  if (i == 0) return vec4( V, K, M, 1 );
  if (i == 1) return vec4( N, V, M, 1 );
  if (i == 2) return vec4( M, V, K, 1 );
  if (i == 3) return vec4( M, N, V, 1 );
  if (i == 4) return vec4( K, M, V, 1 );

  return vec4( V, M, N, 1 );
  
}

vec4 ColorFunctions::rainbow( float lifePct, vec4 posIn ) {
  float r, g, b, w;

  //D'oh, this is needed to avoid backwards rainbows
  float pct = 1 - lifePct;
  
  r = 1.0;
  g = 0.0;
  b = 0.0;
  w = 1.0;

  if( pct > 0 && pct < 0.1 ) {
    r = 1.0;
    g = scaleOverRange( 0, 0.1, pct );
  }
  if( pct > 0.1 && pct < 0.4 ) {
    g = 1.0;
  }
  if( pct > 0.2 && pct < 0.3 ) {
    r = -scaleOverRange( 0.2, 0.3, pct );
  }
  if( pct < 0.25 ) {
    b = 0.0;
  }
  if( pct > 0.25 && pct < 0.4 ) {
    b = scaleOverRange( 0.25, 0.4, pct );
  }
  if( pct > 0.25 && pct < 0.6 ) {
    r = 0.0;
  }
  if( pct > 0.4 && pct < 0.6 ) {
    g = -scaleOverRange( 0.4, 0.6, pct );
  }
  if( pct > 0.4 && pct < 0.8 ) {
    b = 1.0;
  }
  if( pct > 0.6 && pct < 0.8 ) {
    r = scaleOverRange( 0.6, 0.7, pct );
  }
  if( pct > 0.8 && pct < 1.0 ) {
    b = -scaleOverRange( 0.8, 1.0, pct );
    r = 1.0;
  }

  //fade out gracefully
  if( pct > 0.9 ) {
    w = -scaleOverRange( 0.9, 1.0, pct );
  }
  
  return vec4(r, g, b, w);
}

vec4 ColorFunctions::tropical( float lifePct, vec4 posIn ) {
  float r, g, b, w;

  r = 0.5;
  g = sin(posIn.x);
  b = sin(posIn.z);

  //fade out gracefully
  if( lifePct > 0.9 ) {
    w = -scaleOverRange( 0.9, 1.0, lifePct );
  }

  return vec4(r, g, b, w);
}

vec4 ColorFunctions::galaxy( float lifePct, vec4 posIn ) {
  float r, g, b, w;

  if( lifePct > 0.9 ) {
    r = 1.0;
    g = 0.8;
    b = 0.7;
  }  else {
    r = lifePct * 1.25;
    b = 0.7 + (0.3 * (1-lifePct));
    g = 0.7;
  }
 
  //fade out gracefully
  if( lifePct > 0.8 ) {
    w = -scaleOverRange( 0.8, 1.0, lifePct );
  }

  return vec4(r, g, b, w);
}
