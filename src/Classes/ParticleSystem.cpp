/**
 * @file ParticleSystem.cpp
 * @date 2013-03-29
 * @authors Nick VerVoort, Chris Compton
 * @brief ParticleSystem implementation.
 * @details FIXME: Needs documentation from CC/NVV
 */

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
  Object( name, shader, false ), numParticles( particleAmt ), maxLife( 1 ), minLife( 0.1 ) {

  positions = new vec4[numParticles];

  addParticles();
}

ParticleSystem::~ParticleSystem( void ) {
  for ( size_t i = 0; i < particles.size(); i++ ) {
    free( particles[i] );
  }
  particles.clear();
}

void ParticleSystem::addParticles( void ) {
  
  int numParticles = getNumParticles();
  int numToAdd = numParticles - particles.size();
  
  if ( numToAdd > 0 )
    for ( int i = 0 ; i < numToAdd ; i++ )
    {
      Particle *p = new Particle(vec4(0.0, 0.0, 0.0, 1.0), 1, rangeRandom(minLife, maxLife));
      particles.push_back(p);

	 positions[i] = p->getPosition();
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

void
ParticleSystem::Buffer()
{
  glBindVertexArray(_vao);

  glBindBuffer(GL_ARRAY_BUFFER, _buffer[VERTICES]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Angel::vec4) * numParticles,
  			positions, GL_STATIC_DRAW);

  glBindVertexArray(0);
}

void
ParticleSystem::Draw()
{
  glBindVertexArray(_vao);

  GLint currShader;
  glGetIntegerv(GL_CURRENT_PROGRAM, &currShader);
  if( (GLuint)currShader != shader()) {
    Camera *activeCamera = Engine::instance()->cams()->active();
    glUseProgram( shader() );
    activeCamera->shader( shader() );
    activeCamera->view();
  }

  // XXX THIS DISAPPEARED -- FIX IT?
  //send( Object::camPos ); 
  send( Object::OBJECT_CTM  ) ;
  glDrawArrays( GL_POINTS, 0, numParticles );

  glBindVertexArray(0);
  Scene::draw();
}

// Other functions

void ParticleSystem::update() {
}

// Private Functions

float ParticleSystem::rangeRandom( float min, float max ) {
  float diff = max - min;
  
  return fmod( (float) random(), diff ) + min;
}
