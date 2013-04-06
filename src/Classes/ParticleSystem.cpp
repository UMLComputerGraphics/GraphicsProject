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
    Object( name, shader ), numParticles( particleAmt ), minLife( 0.1 ),
    maxLife( 1 ) {

  addParticles();
}

ParticleSystem::~ParticleSystem( void ) {
  for ( int i = 0; i < particles.size(); i++ ) {
    free( particles[i] );
  }
  particles.clear();
}

void ParticleSystem::addParticles( void ) {
  
  int numParticles = getNumParticles();
  int numToAdd = numParticles - particles.size();
  
  std::cout << "Adding " << numToAdd << " particles" << std::endl;

  if ( numToAdd > 0 )
    for ( int i = 0 ; i < numToAdd ; i++ )
    {
      Particle *p = new Particle(vec4(0.0, 0.0, 0.0, 1.0), 1, rangeRandom(minLife, maxLife));
      
      float tempXV, tempYV, tempZV;
      tempXV = rangeRandom( -0.001f, 0.001f );
      tempYV = rangeRandom( -0.001f, 0.001f );
      tempZV = rangeRandom( -0.001f, 0.001f );
      p->setVel( vec3( tempXV, tempYV, tempZV ) );

      particles.push_back(p);

      //_vertices.push_back(p->getPosition());
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
ParticleSystem::buffer()
{
  glBindVertexArray(_vao);

  glBindBuffer(GL_ARRAY_BUFFER, _buffer[VERTICES]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Angel::vec4) * _vertices.size(), 
	       &(_vertices[0]), GL_DYNAMIC_DRAW);

  // Copy/Pasted this from Object::Buffer()
  // Who knows if we will be texturing the spots?
  if ( _texUVs.size() == 0 && _isTextured == false ) {
    _texUVs.push_back( Angel::vec2( -1, -1 ) );
  } else if ( _texUVs.size() > 1 ) {
    /* Yes, this workaround prevents us from having
     textured objects with only one point.
     Oops. */
    _isTextured = true;
  }

  glBindVertexArray(0);
}

void
ParticleSystem::draw()
{

  // bind the vao and 
  glBindVertexArray(_vao);

  // if it isn't already loaded, switch in the appropriate shader. 
  // TODO make this bit of code a (private) function in Object?
  GLint currShader;
  glGetIntegerv(GL_CURRENT_PROGRAM, &currShader);
  if( (GLuint)currShader != shader()) {
    Camera *activeCamera = Engine::instance()->cams()->active();
    glUseProgram( shader() );
    activeCamera->shader( shader() );
    activeCamera->view();
  }

  // XXX THIS DISAPPEARED -- FIX IT?

  // >> listen to the woman; calm down, just calm down 
  // ~Abe Lincoln


  // send uniform information to the shader.
  //send( Object::camPos ); 
  send( Object::OBJECT_CTM  ) ;
  glDrawArrays( GL_POINTS, 0, numParticles );


  /*   // move all the positions into the _vertices buffer inherited from Object
  this->_vertices.clear();
  for ( int i = 0 ; i < this->getNumParticles() ; i++ ) {
	 this-> _vertices.push_back(p->getPosition());
	 //if ( i > getNumParticles() ) cerr << "" << std::endl;
  }*/
  //Get the particles moved/rebuffered for the next draw call
  update();

  // FIXME the particle rebuffering should be /separate/ from the position updating

  glBindVertexArray(0);
  Scene::draw();

}

//Update the particles in our system >>> AND ALSO UPDATE OUR DRAW BUFFER
void ParticleSystem::update() {

  _vertices.clear();

  vector<ParticleP>::iterator i;

  for(i = particles.begin(); i != particles.end(); ++i) {

    (*i)->updateSelf();
    _vertices.push_back((*i)->getPosition());

  }
  //buffer();

}

// Private Functions

float ParticleSystem::rangeRandom( float min, float max ) {
  float diff = max - min;
  
  return fmod( (float) random(), diff ) + min;
}
