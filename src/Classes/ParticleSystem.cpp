#include <string>
#include <SOIL.h>
#include <stdexcept>
#include <vector>
#include "globals.h"
#include "mat.hpp"
#include "model.hpp"
#include "Object.hpp"
#include "Particle.hpp"
#include "ParticleSystem.hpp"
#include "platform.h"
#include "Timer.hpp"
#include "vec.hpp"


using Angel::vec4;
using Angel::mat4;

// Constructor(s)
ParticleSystem::ParticleSystem( const std::string &name, GLuint shader)
  :Object( name, shader )
{
  
}

ParticleSystem::~ParticleSystem( void )
{

}

void
ParticleSystem::addParticle( void )
{
  
  int numParticles = getNumParticles();
  int numToAdd = numParticles - paricles.size();

  if ( numToAdd > 0 )
    for ( int i = 0 ; i < numToAdd ; i++ )
      particles.push_back( new Particle::Particle() );
}

// Getters and Setters
vec4
ParticleSystem::getColor( void )
{
  return color;
}

float
ParticleSystem::getLifespan( void )
{
  return lifespan;
}

int
ParticleSystem::getMaxAngle( void )
{
  return maxAngle;
}

int
ParticleSystem::getMinAngle( void )
{
  return minAngle;
}

int
ParticleSystem::getNumParticles( void )
{
  return numParticles;
}

void
ParticleSystem::setColor( vec4 newColor )
{
  color = newColor;
}

void
ParticleSystem::setLifespan( float newLifespan )
{
  lifespan = newLifespan;
}

void
ParticleSystem::setMaxAngle( int newMaxAngle )
{
  maxAngle = newMaxAngle;
}

void
ParticleSystem::setMinAngle( int newMinAngle )
{
  minAngle = newMinAngle;
}

void
ParticleSystem::setNumParticles( int newNumParticles )
{
  numParticles = newNumParticles;
}

// Other functions

void
ParticleSystem::candleFlame(){}

void
ParticleSystem::smokeSystem(){}

void
ParticleSystem::percipitation(){}

void
ParticleSystem::update(){}
