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
 @brief The Particle class, many of which will be handled simultaneously by a
 ParticleSystem.
 **/

class Particle {
  
public:
  
  Particle( vec4 position, float scale, float lifeSpan );
  ~Particle( void );

  void changeVel( vec3 newVel );

  float getLifetime( void );

  void setAlpha( float newAlpha );
  void setColor( vec4 newColor );
  void setLifetime( float newLifetime );
  void setPos( vec4 newPos );
  void setScale( vec3 newScale );
  //void setSpin( float newSpin );
  void setVel( vec3 newVel );
  void setTexFile( string newFilename );
  vec4 getPosition( void );

  void updateSelf( void );

private:
  vec4 _pos;
  vec3 _scale;
  vec3 _vel;
  vec4 _color;
  float _lifetime;
  //  float spin;
  
  string _texFilename;
  
};

#endif
