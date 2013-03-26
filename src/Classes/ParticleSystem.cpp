#include <cmath>
#include <string>
#include <SOIL.h>
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
ParticleSystem::ParticleSystem( int particleAmt, const std::string &name, GLuint shader)
  : Object( name, shader ), numParticles(particleAmt), minLife(0.1), maxLife(1)
{
  positions = new vec4[numParticles];

  addParticles();
}

ParticleSystem::~ParticleSystem( void )
{
	for(int i=0;i<particles.size();i++)
	{
		free(particles[i]);
	}
    particles.clear();
}

void
ParticleSystem::addParticles( void )
{
  
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
vec4
ParticleSystem::getColor( void )
{
  return color;
}

float
ParticleSystem::getMaxLife( void )
{
  return maxLife;
}

float
ParticleSystem::getMinLife( void )
{
  return minLife;
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
ParticleSystem::setLifespan( float minLifespan, float maxLifespan )
{
  minLife = minLifespan ;
  maxLife = maxLifespan ;
}

void
ParticleSystem::setNumParticles( int newNumParticles )
{
  numParticles = newNumParticles;
}

void
ParticleSystem::Buffer()
{
  glBindVertexArray(vao);

  glBindBuffer(GL_ARRAY_BUFFER, buffer[VERTICES]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Angel::vec4) * numParticles,
  			positions, GL_STATIC_DRAW);

  glBindVertexArray(0);
}

void
ParticleSystem::Draw()
{
  glBindVertexArray(vao);

  GLint currShader;
  glGetIntegerv(GL_CURRENT_PROGRAM, &currShader);
  if( (GLuint)currShader != Shader()) {
    Camera *activeCamera = Engine::instance()->cams()->active();
    glUseProgram( Shader() );
    activeCamera->Shader( Shader() );
    activeCamera->view();
  }

  send( Object::CamPos );
  send( Object::ObjectCTM  ) ;
  glDrawArrays( GL_POINTS, 0, numParticles );

  glBindVertexArray(0);
  Scene::Draw();
}

// Other functions

void
ParticleSystem::update(){

  

}

// Private Functions

float
ParticleSystem::rangeRandom( float min, float max )
{
  float diff = max - min;
  
  return  fmod( (float)random(), diff) + min; 
}
