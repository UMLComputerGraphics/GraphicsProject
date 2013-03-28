#include <cmath>
#include <string>
#include <cstdlib>
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

using namespace Angel;

// Constructor(s)
ParticleSystem::ParticleSystem( int particleAmt, const std::string &name,
                                GLuint shader ) :
    Object( name, shader ), numParticles( particleAmt ), minLife( 0.1 ),
    maxLife( 1 ) {
  
}

ParticleSystem::~ParticleSystem( void ) {
  for ( int i = 0; i < particles.size(); i++ ) {
    free( particles[i] );
  }
  particles.clear();
}

void ParticleSystem::addParticle( void ) {
  
  int numParticles = getNumParticles();
  int numToAdd = numParticles - particles.size();
  
  if ( numToAdd > 0 )
    for ( int i = 0; i < numToAdd; i++ ) {
      Particle *p = new Particle( vec4( 0, 0, 0, 1 ), 1,
                                  rangeRandom( minLife, maxLife ) );
      particles.push_back( p );
    }
}

// Getters and Setters
vec4 ParticleSystem::getColor( void ) {
  return color;
}

float ParticleSystem::getMaxLife( void ) {
  return maxLife;
}

float ParticleSystem::getMinLife( void ) {
  return minLife;
}

int ParticleSystem::getNumParticles( void ) {
  return numParticles;
}

void ParticleSystem::setColor( vec4 newColor ) {
  color = newColor;
}

void ParticleSystem::setLifespan( float minLifespan, float maxLifespan ) {
  minLife = minLifespan;
  maxLife = maxLifespan;
}

void ParticleSystem::setNumParticles( int newNumParticles ) {
  numParticles = newNumParticles;
}

// Other functions

void ParticleSystem::update() {
}

// Private Functions

float ParticleSystem::rangeRandom( float min, float max ) {
  float diff = max - min;
  
  return fmod( (float) random(), diff ) + min;
}
