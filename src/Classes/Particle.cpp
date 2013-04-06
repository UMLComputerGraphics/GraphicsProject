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
Particle::Particle( vec4 position, float scale, float lifeSpan ) :
  _pos( position ), _scale( scale ), _vel(vec3(0.0, 0.0, 0.0)), 
  _lifespan( lifeSpan ) {
  
}

// Destructor(s)
Particle::~Particle() {
  
}

// Update position and other properties
void Particle::updateSelf( void ) {

  // use the timer so the particle movement will look the same for everyone
  float scale = tick.scale() ;

  _pos.x += _vel.x * scale ;
  _pos.y += _vel.y * scale ;
  _pos.z += _vel.z * scale ;

  //add more stuff

  //increase alive time (not lifeSPAN)
  //if life is over (alive time >= lifeSPAN) then respawn particle.

}

// Setters (Will we need getters? Probably not)
void Particle::changeVel( vec3 newVel ) {
  _vel += newVel;
}

float Particle::getLifespan( void ) {
  return _lifespan;
}

void Particle::setAlpha( float newAlpha ) {
  _color.w = newAlpha;
}

void Particle::setColor( vec4 newColor ) {
  _color = newColor;
}

void Particle::setLifespan( float newLifespan ) {
  _lifespan = newLifespan;
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

void Particle::setTexFile( string newFilename ) {
  _texFilename = newFilename;
}

vec4
Particle::getPosition( void )
{
  return _pos;
}
