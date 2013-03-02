#include <string>
#include <SOIL.h>
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
Particle::Particle( vec4 position, float scale )
{

}

// Destructor(s)
Particle::~Particle()
{

}

// Setters (Will we need getters? Probably not)

void
Particle::setAlpha( float newAlpha )
{
  color.w = newAlpha;
}

void
Particle::setColor( vec4 newColor )
{
  color = newColor;
}

void 
Particle::setLifespan( float newLifespan )
{
  lifespan = newLifespan;
}

void
Particle::setPos( vec4 newPos )
{
  mPos = newPos;
}

void
Particle::setScale( vec3 newScale )
{
  mScale = newScale;
}

void
Particle::setVel( vec3 newVel )
{
  mVel = newVel;
}

void
Particle::setSpin( float newSpin )
{
  spin = newSpin;
}

void
Particle::setTexFile( string newFilename )
{
  texFilename = newFilename;
}

// Other Methods

void
Particle::update()
{
  // What do you do? Call things that move object in the Transcache(?) class.
  // Also will handle billboarding and spinning.
  // ALL THE PARTICLE MOVEMENTS
}
