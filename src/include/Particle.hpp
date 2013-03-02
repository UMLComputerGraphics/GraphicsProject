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
   @authors Nick Ver Voort, Chris Compton
            nicholas_vervoort@student.uml.edu, christopher_compton@student.uml.edu
   @since 23 Feb 2013

   @brief todo

**/
class Particle {

public:

  Particle( vec4 position, float scale  );
  ~Particle( void ) ;




  void changeVel( vec3 newVel );

  void setAlpha( float newAlpha );  // changes the alpha level of color
  void setColor( vec4 newColor );
  void setLifespan( float newLifespan );
  void setScale( vec3 newScale );
  void setVel( vec3 newVel );
  void setTexFile( string newFilename );

 private:
  vec4 mPos;
  vec3 mScale;
  vec3 mVel;
  
  vec4  color;
  float lifespan;
  //  float spin;

  string texFilename;

};

#endif
