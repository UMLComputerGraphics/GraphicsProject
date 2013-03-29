/**
 * @file Particle.hpp
 * @author FIXME
 * @authors FIXME
 * @date 2013-03-15
 * @brief FIXME: Needs documentation from NVV/CC.
 */

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
 @authors Nick Ver Voort, Chris Compton
 nicholas_vervoort@student.uml.edu, christopher_compton@student.uml.edu
 @brief TODO: You know you've been bad.
 **/

class Particle {
  
public:
  
  Particle( vec4 position, float scale, float lifeSpan );
  ~Particle( void );

  void changeVel( vec3 newVel );

  float getLifespan( void );

  void setAlpha( float newAlpha );  // changes the alpha level of color
  void setColor( vec4 newColor );
  void setLifespan( float newLifespan );
  void setPos( vec4 newPos );
  void setScale( vec3 newScale );
  void setSpin( float newSpin );
  void setVel( vec3 newVel );
  void setTexFile( string newFilename );
  vec4 getPosition( void );

private:
  vec4 mPos;
  vec3 mScale;
  vec3 mVel;
  vec4 color;
  float lifespan;
  //  float spin;
  
  string texFilename;
  
};

#endif
