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

#ifndef MIN_EMITTER_RADIUS
#define MIN_EMITTER_RADIUS 1.0/512.0
#endif

// Constructor(s)
ParticleSystem::ParticleSystem( int particleAmt, const std::string &name,
                                GLuint shader ) :
    Object( name, shader ), numParticles( particleAmt ), minLife( 0.1 ),
    maxLife( 1 ) 
{
   this->drawMode(GL_POINTS) ;
   this->fillSystemWithParticles();

   this->pauseTheSystem = false;
}

ParticleSystem::~ParticleSystem( void ) {
  for ( size_t i = 0; i < particles.size(); i++ ) {
    free( particles[i] );
  }
  particles.clear();
}

Particle*
ParticleSystem::newRandomParticle(void)
{
    vec4  spawnPosition ;

    if ( this->_emitterRadius > MIN_EMITTER_RADIUS ) {

        spawnPosition.x = spawnPosition.y = spawnPosition.z = 0.0 ;
	spawnPosition.w = 1.0 ;

    } else {
	
      // Generate a random position on a circle of radius radius (passed as arg)
      float randomTheta ;

      // I LOVE THIS FUNCTION SO MUCH
      randomTheta = rangeRandom( 0.0, 360.0 ); // if degrees
      //randomTheta = rangeRandom( 0.0, 2.0*M_PI ); //if radians

      spawnPosition.x = cos(randomTheta)*(this->_emitterRadius);
      spawnPosition.y = 0.0 ;
      spawnPosition.z = sin(randomTheta)*(this->_emitterRadius);
      spawnPosition.w = 1.0 ; // ??? I don't know if this matters
    } 

    //generate a particle on the random position (if radius was big enough)
    Particle *p = new Particle( spawnPosition,
				1,
				rangeRandom(this->minLife, this->maxLife) );

    float tempXV, tempYV, tempZV ;
    tempXV = rangeRandom( -0.001f, 0.001f );
    tempYV = rangeRandom( -0.001f, 0.001f );
    tempZV = rangeRandom( -0.001f, 0.001f );
    p->setVel( vec3( tempXV, tempYV, tempZV ) );


  return p ;
}


void
ParticleSystem::addOneParticleAtOrigin( void ) {

      Particle *p = new Particle(vec4(0.0, 0.0, 0.0, 1.0), 1, rangeRandom(minLife, maxLife));
      
      float tempXV, tempYV, tempZV;
      tempXV = rangeRandom( -0.001f, 0.001f );
      tempYV = rangeRandom( -0.001f, 0.001f );
      tempZV = rangeRandom( -0.001f, 0.001f );
      p->setVel( vec3( tempXV, tempYV, tempZV ) );

        // some extra padding, push_back can throw if something catastrophic happpens
       try
       {
	   this->particles.push_back(p);
       }
       catch (...)
       {
	   std::cerr << "SEVERE: attempt to add a particle to particles vector failed" 
		     << std::endl;
       }

      //_vertices.push_back(p->getPosition());

}


void
ParticleSystem::addSomeParticles( int numToAdd ) {

      for ( int i = 0 ; i < numToAdd ; i++ )
        this->addParticle();

}


void
ParticleSystem::addParticle()
{

    Particle *p;

    p = newRandomParticle() ;

    // some extra padding, push_back can throw if something catastrophic happpens
    try
    {
	this->particles.push_back(p);
    }
    catch (...)
    {
        std::cerr << "SEVERE: attempt to add a particle to particles vector failed" 
		  << std::endl;
    }

}

void
ParticleSystem::fillSystemWithParticles( void ) {

    int numParticles = getNumParticles();
    int numToAdd = numParticles - particles.size();
  
    //std::cout << "Adding " << numToAdd << " particles" << std::endl;

    for ( int i = 0 ; i < numToAdd ; i++ )
    {
	this->addParticle();
    }

}

// Getters and Setters
vec4
ParticleSystem::getColor( void ) {
  return color;
}

float
ParticleSystem::getMaxLife( void ) {
  return maxLife;
}

float
ParticleSystem::getMinLife( void ) {
  return minLife;
}

int
ParticleSystem::getNumParticles( void ) {
  return numParticles;
}

void
ParticleSystem::setColor( vec4 newColor ) {
  color = newColor;
}

void
ParticleSystem::setLifespan( float minLifespan, float maxLifespan ) {
  minLife = minLifespan;
  maxLife = maxLifespan;
}

void
ParticleSystem::setNumParticles( int newNumParticles ) {
  numParticles = newNumParticles;
}

void
ParticleSystem::buffer( void )
{
    glBindVertexArray(_vao);

    glBindBuffer(GL_ARRAY_BUFFER, _buffer[VERTICES]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Angel::vec4) * _vertices.size(), &(_vertices[0]), GL_DYNAMIC_DRAW);

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
ParticleSystem::draw( void )
{

    //std::cerr << "invoking ps draw" << std::endl;

    // we should consider moving the update() call to the idle() loop
    update();
    buffer();

    glBindVertexArray(_vao);
    // if it isn't already loaded, switch in the appropriate shader. 
    // TODO make this bit of code a (private) function in Object?
    //GLint currShader;
    //glGetIntegerv(GL_CURRENT_PROGRAM, &currShader);
    //if( (GLuint)currShader != shader()) {
      Camera *activeCamera = Engine::instance()->cams()->active();
      glUseProgram( shader() );
      activeCamera->shader( shader() );
      activeCamera->view();
      //}


    send( Object::IS_TEXTURED );
    send( Object::OBJECT_CTM );
    send( Object::MORPH_PCT );
    send( Object::TEX_SAMPLER );

    // send uniform information to the shader.
    //send( Object::camPos ); 
    send( Object::OBJECT_CTM  ) ;

    //glDrawArrays( GL_POINTS, 0, numParticles );
    glDrawArrays( GL_POINTS, 0, _vertices.size() );

    glBindVertexArray(0);
    Scene::draw();

}

//Update the particles in our system >>> AND ALSO UPDATE OUR DRAW BUFFER
void
ParticleSystem::update() {

    if ( this->pauseTheSystem ) {
      return;
    }

    _vertices.clear();

    vector<ParticleP>::iterator i;
    
    for( i = particles.begin(); i != particles.end(); ++i) {

      // call the update function on each particle
      (*i)->updateSelf();

      _vertices.push_back((*i)->getPosition());

      if( (*i)->getLifetime() <= 0.0 ) {
	(*i)->setPos( position() );
	(*i)->setLifetime( rangeRandom(getMinLife(), getMaxLife() ));
	float tempXV, tempYV, tempZV ;
	tempXV = rangeRandom( -0.001f, 0.001f );
	tempYV = rangeRandom( -0.001f, 0.001f );
	tempZV = rangeRandom( -0.001f, 0.001f );
	(*i)->setVel( vec3( tempXV, tempYV, tempZV ) );


      }
      
    }
  
    // should we really tie the buffering to the update() call??
    //buffer();

}

//
// Private Functions
//



/**
   THIS SHOULD BE MOVED TO UTIL.CPP!
   @param max first value greater than min that is too great to be returned
   @param min minimum possible value to be returned
   @return a float between min (inclusive) and max (exclusive)
 */
float 
ParticleSystem::rangeRandom( float min, float max ) {
  float diff = max - min;
  
  return fmod( (float) random(), diff ) + min;
}
