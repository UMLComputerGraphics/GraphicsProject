#ifndef __PARTICLE_H
#define __PARTICLE_H

#include <string>
#include "mat.hpp"
#include "vec.hpp"
#include "Object.hpp"

using Angel::vec2;
using Angel::vec3;
using Angel::vec4;
using Angel::mat4;
using std::string;

/**
   @author Nick Ver Voort, nicholas_vervoort@student.uml.edu
   @since 23 Feb 2013

   @brief todo

**/
class Particle : public Object {

public:

  Particle( vec4 initPos, vec3 initScale, vec3 initVel, float initAlpha, 
	    vec4 initColor, float initLifespan, float initSpin, 
	    string initTex, GLuint gShader );
  virtual ~Particle( void ) ;

  /* Apply all changes to object's transformation */
  void update( );

  void setPos( vec4 newPos ) ;
  void setScale( vec3 newScale ) ;
  void setVel( vec3 newVel );

  void setAlpha( float newAlpha ) ;
  void setColor( vec4 newColor ) ;
  void setLifespan( float newLifespan ) ;
  void setSpin( float newSpin );

  void setTexFile( string newFilename );

 private:
  vec4 mPos;
  vec3 mScale;
  vec3 mVel;
  
  float alpha;
  vec4 blendColor;
  float lifespan;
  float spin;

  string texFilename;

};

#endif
