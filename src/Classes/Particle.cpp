
/**
 * @file Particle.cpp
 * @author Nick Ver Voort, nicholas_vervoort@student.uml.edu
 * @date 2013-03-15
 * @brief FIXME: Needs documentation from NVV/CC
 */
#include <string>
#include <stdexcept>
#include <vector>
#include "globals.h"
#include "mat.hpp"
#include "model.hpp"
#include "Object.hpp"
#include "Particle.hpp"
#include "platform.h"
#include "Timer.hpp"
#include "vec.hpp"

using Angel::vec4;
using Angel::mat4;

// Constructor(s)
Particle::Particle( vec4 position, float scale, float lifetime ) :
  _pos( position ), _scale( scale ), _vel(vec3(0.0, 0.0, 0.0)),
  _respawnFlag( false )
{
  setMaxLifetime(lifetime), setColor(vec4(1.0,0.0,1.0,1.0));
}

Particle::Particle( vec4 position, float lifetime ) :
    _pos( position ), _vel(vec3(0.0, 0.0, 0.0)), _respawnFlag( false )
{
  setMaxLifetime(lifetime), setColor(vec4(1.0,0.0,1.0,1.0));
}

// Destructor(s)
Particle::~Particle() {
  
}

// Update position and other properties
void Particle::updateSelf( void ) {

  // use the timer so the particle movement will look the same for everyone
  //float scale = tick.scale() ;

  _pos.x += _vel.x ;//* scale ;
  _pos.y += _vel.y ;//* scale ;
  _pos.z += _vel.z ;//* scale ;

  this->_lifetime -= 0.05 ;//* scale;
  //increase alive time (not lifeSPAN)
  //if ( _lifetime <= 0.0 ) _pos.x = _pos.y = _pos.z = 0.0 ;

}


void Particle::changeVel( vec3 newVel ) {
  _vel += newVel;
}

float Particle::getLifetime( void ) {
  return _lifetime;
}

void Particle::setAlpha( float newAlpha ) {
  _color.w = newAlpha;
}

float Particle::getAlpha( void ) {
  return _color.w;
}

void Particle::setColor( vec4 newColor ) {
  _color = newColor;
}

void Particle::setLifetime( float newLifetime ) {
  _lifetime = newLifetime;
}

void Particle::setMaxLifetime( float newMaxLife ) {
	_maxLifetime = _lifetime = newMaxLife;
}

void Particle::setPos( vec4 newPos ) {
  _pos = newPos;
}

void Particle::setScale( vec3 newScale ) {
  _scale = newScale;
}

void Particle::setVel( vec3 newVel ) {
  _vel = newVel;
}

void Particle::setParticleRespawnFlag( bool flag )
{
    _respawnFlag = flag ;
}

bool Particle::getRespawnFlag()
{
    return _respawnFlag;
}

vec4
Particle::getPosition( void )
{
  return _pos;
}

float
Particle::getMaxLifetime()
{
	return _maxLifetime;
}

vec4
Particle::getColor( void ) {
  return _color ;
}
