/**
 @file Particle.hpp
 @author Nick Ver Voort, nicholas_vervoort@student.uml.edu
 @since 23 Feb 2013
 @authors Nick Ver Voort, Chris Compton, Nicholas St.Pierre
 nicholas_vervoort@student.uml.edu, christopher_compton@student.uml.edu, nstpierr@cs.uml.edu
 @brief The Particle class
 **/

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


class Particle {
  
public:

  Particle( vec4 position, float lifetime );
  Particle( vec4 position, float scale, float lifetime );
  ~Particle( void );

  void changeVel( vec3 newVel ); // ???

  // setters
  void  setAlpha( float newAlpha );
  void  setColor( vec4 newColor );
  void  setLifetime( float newLifetime );
  void  setMaxLifetime( float newMaxLife );
  void  setPos( vec4 newPos );
  void  setScale( vec3 newScale );
  void  setVel( vec3 newVel );
  void  setParticleRespawnFlag( bool );

  // getters
  vec4  getPosition( void );
  float getMaxLifetime(void);
  float getLifetime( void );
  float getAlpha( void ) ;
  vec4 getColor( void );
  bool getRespawnFlag( void );
  //void setSpin( float newSpin );

  void updateSelf( void );

private:
  vec4   _pos;
  vec3   _scale;
  vec3   _vel;
  vec4   _color;
  float  _lifetime;
  float  _maxLifetime;
  bool   _respawnFlag;
  //  float spin;

};

#endif
