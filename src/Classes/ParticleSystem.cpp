/**
 * @file ParticleSystem.cpp
 * @date 2013-03-29
 * @authors Nick VerVoort, Chris Compton, Nicholas St.Pierre
 * @brief ParticleSystem implementation.
 * @details FIXME: Needs documentation from CC/NVV/NSP
 */

#include <cmath>
#include <string>
#include <cstdlib>
#include <stdexcept>
#include <vector>
#include <ctime>
#include "globals.h"
#include "mat.hpp"
#include "model.hpp"
#include "Object.hpp"
#include "Particle.hpp"
#include "ParticleSystem.hpp"
#include "platform.h"
#include "Timer.hpp"
#include "vec.hpp"

#include "ParticleFieldFunctions.hpp"
#include "ColorFunctions.hpp"

using namespace Angel;

#ifndef MIN_EMITTER_RADIUS
#define MIN_EMITTER_RADIUS 1.0/2048.0
#endif


// Constructor(s)
ParticleSystem::ParticleSystem( int particleAmt, const std::string &name,
				GLuint shader ) :
  Object( name, shader ),
  _numParticles( particleAmt ), 
  _minLife( 0.1 ),
  _maxLife( 1 ), 
  _pauseTheSystem( false ), 
  _enableTheSystem( false ),
  _slaughterHeight( 0.0 ),
  _fillSpeedLimit( 5 ), 
  _emitterRadius( 0.0 ),
  _emitterShape(PS_NONE),
  _systemShape(PS_NONE),
  _funcParams( new FlameParameters() ),
  _uvf( new UserVectorField() ),
  _vecFieldFunc( NULL ), 
  _colorFunc(ColorFunctions::standard),
  _numToAddEachFrame(12),
  _cachedVisibleCount(0)
{
  this->drawMode( GL_POINTS );
}


ParticleSystem::~ParticleSystem( void ) {
  for ( size_t i = 0; i < _particles.size(); i++ ) {
    delete _particles[i];
  }
  _particles.clear();

  delete _funcParams;
  delete _uvf;
}

vec4
ParticleSystem::getRandomCircularSpawnPoint(void)
{

	vec4 ret;

	float randomTheta ;
	float jitteredRadius ;


	randomTheta = rangeRandom( 0.0, 2.0*M_PI ); // radians
	jitteredRadius = rangeRandom(this->_emitterRadius/100, this->_emitterRadius);

	ret.x = cos(randomTheta)*(jitteredRadius);
	ret.y = 0.0 ;
	ret.z = sin(randomTheta)*(jitteredRadius);
	ret.w = 1.0 ;


	return ret;

}



vec4
ParticleSystem::getRandomHemisphericalSpawnPoint(void)
{
    return getRandomSphericalSpawnPointHelper(2);
}

vec4
ParticleSystem::getRandomSphericalSpawnPoint(void)
{
    return getRandomSphericalSpawnPointHelper(1);
}


vec4
ParticleSystem::getRandomSphericalSpawnPointHelper(float in)
{

	vec4 ret;

	// Generate a random position on a half-circle of radius from particle system
	float randomTheta ;
	float randomPhi ;
	float jitteredRadius ;


	randomTheta = rangeRandom( 0.0, 2.0 * M_PI ); //if radians
	randomPhi   = rangeRandom( 0.0,  in * M_PI );

	jitteredRadius = rangeRandom(this->_emitterRadius/100, this->_emitterRadius);

	ret.x = sin(randomPhi) * cos(randomTheta) * (jitteredRadius);
	ret.y = cos(randomPhi) *                    (jitteredRadius);
	ret.z = sin(randomPhi) * sin(randomTheta) * (jitteredRadius);
	ret.w = 1.0 ;


	return ret;


}


void
ParticleSystem::respawnParticle(Particle &p)
{

    vec4  spawnPosition = vec4(0.0,0.0,0.0,1.0);

    if ( this->_emitterRadius >= MIN_EMITTER_RADIUS ) 
      spawnPosition =  this->_emitterLoc.ctm() * this->getSpawnPos( this->getEmitterShape() );

    p.setPos(spawnPosition);

    if ( p.getRespawnFlag() == true )
    {
        p.setMaxLifetime( generateLifespan() );
        p.setLifetime( p.getMaxLifetime() );
        p.setParticleRespawnFlag( false );
    }
    else
    {
        p.setLifetime( p.getMaxLifetime() ) ;
    }
}

vec4
ParticleSystem::getRandomLinearSpawnPoint(void)
{

  float rad, nrad, thickness;

  thickness = 0.05;

  nrad = -(rad = this->_emitterRadius);

  return vec4( rangeRandom( nrad, rad ), 
	       rangeRandom( -thickness, thickness ), 
	       0.0,
	       1.0  );

}


vec4 negateY(vec4 in)
{
  in.y = -in.y;
  return in;
}

//typedef enum{ PS_NONE, PS_SPHERE, PS_HEMI_D, PS_HEMI_U, PS_CUBE }PS_SHAPE; getSystemShape();

vec4
ParticleSystem::getSpawnPos(PS_SHAPE s)
{

    switch(s)
    {

    case PS_NONE:
      return vec4(0.0,0.0,0.0,1.0);

    case PS_CIRCLE:
      return getRandomCircularSpawnPoint();

    case PS_LINE:
      return getRandomLinearSpawnPoint();

    case PS_SPHERE:
      return getRandomSphericalSpawnPoint();

    case PS_HEMI_D:
      return getRandomHemisphericalSpawnPoint();

    case PS_HEMI_U:
      return negateY( getRandomHemisphericalSpawnPoint());

    case PS_CUBE:
      return getRandomCubeSpawnPoint();


    default:
      throw std::logic_error("da faq are you doing? stop passing garbage to the system");

    }

    return vec4(0.0,0.0,0.0,1.0); // does nothing except satisfy compiler

}


PS_SHAPE
ParticleSystem::getEmitterShape() const 
{
        return this->_emitterShape ;
}

PS_SHAPE
ParticleSystem::getSystemShape() const 
{
        return this->_systemShape ;
}

void
ParticleSystem::setEmitterShape(PS_SHAPE in)
{
        this->_emitterShape = in ;
}

void
ParticleSystem::setSystemShape(PS_SHAPE in)
{
        this->_systemShape = in;
}

//RADIUS IS DISTANCE FROM CENTER TO CORNER OF CUBE
vec4
ParticleSystem::getRandomCubeSpawnPoint()
{
        //square gen method
        float tempXV, tempYV, tempZV, rad, nrad ;

	nrad = -(rad = this->_emitterRadius);

	tempXV = rangeRandom( nrad, rad );
	tempYV = rangeRandom( nrad, rad );
	tempZV = rangeRandom( nrad, rad );

	return vec4( tempXV, tempYV, tempZV, 1.0 ) ;
}

Particle*
ParticleSystem::newRandomParticle(void)
{
	vec4  spawnPosition = vec4(0.0,0.0,0.0,1.0);

	if ( this->_emitterRadius >= MIN_EMITTER_RADIUS ) {
	    spawnPosition =  this->_emitterLoc.ctm() * this->getSpawnPos( this->getEmitterShape() );
	}

	// Generate a particle on the random position (if radius was big enough)
	Particle *p = new Particle( spawnPosition, 1, generateLifespan() );

	// TODO initial particle velocities based on which direction the emitter is pointing 

	return p ;
}

// don't use this
void
ParticleSystem::addOneParticleAtOrigin( void ) {

	Particle *p = new Particle(vec4(0.0, 0.0, 0.0, 1.0), 1, generateLifespan());

	float tempXV, tempYV, tempZV;
	tempXV = rangeRandom( -0.001f, 0.001f );
	tempYV = rangeRandom( -0.001f, 0.001f );
	tempZV = rangeRandom( -0.001f, 0.001f );
	p->setVel( vec3( tempXV, tempYV, tempZV ) );

	// some extra padding, push_back can throw if something catastrophic happpens
	try
	{
		this->_particles.push_back(p);
	}
	catch (...)
	{
		std::cerr << "SEVERE: attempt to add a particle to particles vector failed"
				<< std::endl;
	}

}

void
ParticleSystem::setSlaughterHeight(float f){
	this->_slaughterHeight = f ;
}

void
ParticleSystem::updateNumParticles( int numParticles ) {
	if( numParticles >= 0 ){
		for ( int i = 0 ; i < numParticles ; i++ )
			this->addParticle();
	}
	else if ( numParticles < 0 ) {
		for ( int i = numParticles  ; i < 0 ; i++ )
			this->removeParticle();
	}
}

void
ParticleSystem::addParticle()
{

	Particle *p;

	p = newRandomParticle() ;


	try
	{
		this->_particles.push_back(p);
	}
	catch (...)
	{
		std::cerr << "SEVERE: attempt to add a particle to particles vector failed"
			  << std::endl;
	}
}

void
ParticleSystem::removeParticle()
{
	try
	{
		if( this->_particles.size() > 0 )
		{
			this->_particles.pop_back();
		}
	}
	catch (...)
	{
		std::cerr << "SEVERE: attempt to remove a particle to particles vector failed"
				<< std::endl;
	}
}

void
ParticleSystem::fillSystemWithParticles( void ) {

	int numParticles = getNumParticles();

	while( numParticles % 3 ) numParticles++;

	int numToAdd = numParticles - _particles.size();


	//std::cout << "Adding " << numToAdd << " particles" << std::endl;

	for ( int i = 0 ; i < numToAdd ; i++ )
	{
		this->addParticle();
	}

}

// Getters and Setters
float
ParticleSystem::getMaxLife( void ) const
{
	return _maxLife;
}

float
ParticleSystem::getMinLife( void ) const
{
	return _minLife;
}

int
ParticleSystem::getNumParticles( void ) const
{
	return _numParticles;
}

int
ParticleSystem::getNumParticlesActual( void ) const {
        return _vertices.size();
}

int
ParticleSystem::getNumParticlesVisible( void ) const
{
  return _cachedVisibleCount;
}
/*    int count = 0;
    for( vector<ParticleP>::const_iterator i = _particles.begin() ;
         i != _particles.end() ; ++i )
        if( (*i)->getAlpha() > 0.1 ) count++;

    return count;
    }*/

void
ParticleSystem::setLifespan( float minLifespan, float maxLifespan ) {
	_minLife = minLifespan;
	_maxLife = maxLifespan;
}

void
ParticleSystem::setNumParticles( int newNumParticles ) {
	_numParticles = newNumParticles;
}

void  ParticleSystem::buffer( GLenum usage ) {

	// Note: The default for ParticleSystem is GL_DYNAMIC_DRAW,
	// While the default for Object is GL_STATIC_DRAW.
	// This method exists to override that default.
	Object::buffer( usage );

}

void ParticleSystem::draw( bool doDraw ) {

        //move/spawn/remove/color all particles
	if ( ! this->_pauseTheSystem ) 
	  this->update();

	// if we don't have any particles, bail
	if ( _vertices.size() <= 0 ) return;

	// particleSytems must re-buffer every frame
	if ( ! this->_pauseTheSystem ) 
	  this->buffer();

	// This binds the vertex array and sets the active shader,
	// And sends all of the Object's uniforms.
	Object::drawPrep();

	// use additive blending (makes the flame better!)
	// TODO make the blend mode an attribute of the particle system
	glBlendFunc( GL_SRC_ALPHA, GL_ONE );

	// We can handle this ourselves, because we're pretty.
	glDrawArrays( _drawMode, 0, _vertices.size() );

	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	// Unbind our vertex, then draw children if any.
	glBindVertexArray(0);
	Scene::draw();


}

void
ParticleSystem::setEmitterRadius( float r )
{
	this->_emitterRadius = r ;
}


void hideParticle(Particle *p) {  p->setAlpha(0.0); }


void ParticleSystem::setNumToAddEachFrame(unsigned in){ _numToAddEachFrame = in ; }


//Update the particles in our system >>> AND ALSO UPDATE OUR DRAW BUFFER
void
ParticleSystem::update() {

	// used to stagger the creation of particles
	static unsigned currFillFrame=0;

	const int averageFrameLifetime = 10 * (_minLife+_maxLife) ;

	int numRespawnsThisFrame, visibleCount=0 ;

	// The answer lies in one of these two functions... BUT WHICH ONE??
	numRespawnsThisFrame = this->getNumParticles()/averageFrameLifetime ;
	//numRespawnsThisFrame = this->getNumParticlesActual()/(averageFrameLifetime) ;

	this->_emitterLoc.calcCTM();

	// if we need more particles...
	if ( _particles.size() < (unsigned int) this->_numParticles ){

		if ( currFillFrame == _fillSpeedLimit ) {
			updateNumParticles( _numToAddEachFrame );
			currFillFrame = 0;
		}
		currFillFrame++;
	}

	// if we need fewer particles...
	else if ( ( _particles.size() > (unsigned int) this->_numParticles ) &&
		  ( (unsigned)abs(this->_numParticles - (int)_particles.size() ) >= _numToAddEachFrame ) 
		  //^^ condition to make sure the system doesn't bang back and forth between values
		  ){

		if ( currFillFrame == _fillSpeedLimit ) {
			updateNumParticles( -1 * _numToAddEachFrame );
			currFillFrame = 0;
		}
		currFillFrame++;
	}

	_vertices.clear();
	_colors.clear();

	vector<ParticleP>::iterator i;
	//float maxHeight = this->_slaughterHeight ;

	for( i = _particles.begin() ; i != _particles.end() ; ++i) 
	{

		float percentLifeRemaining = (*i)->getLifetime()/(*i)->getMaxLifetime();

		//check to see if the particle is dead BEFORE we change it's members
		if( ((*i)->getLifetime() <= 0.0)
		/*|| ((*i)->getPosition().y >= maxHeight)*/ ) 
		{
		  
		  if( numRespawnsThisFrame )
		  {
		          respawnParticle(**i)  ;
			  numRespawnsThisFrame--;


			  if ( this->_vecFieldFunc == NULL ) {

			    /* We "need" a default particle velocity behavior, and don't have one yet. */

			    // sphere generating method
			    float row   = rangeRandom( 0.001f, 0.004f ); // equivalent to magnitude
			    float phi   = rangeRandom( 0.0f, 2 * M_PI );
			    float theta = rangeRandom( 0.0f, 2 * M_PI );
			    (*i)->setVel( vec3( row*sin(phi)*cos(theta),
						row*sin(phi)*sin(theta),
						row*cos(phi) ));

			  }

		  }
		  else
		  {	
		          hideParticle(*i);
			  continue; // if the particle is dead, leave it dead.
		  }

		}


		// call the update function on each particle
		(*i)->updateSelf();

		// apply the color function, if we have one
		if ( this->_colorFunc != NULL ) {
		  (*i)->setColor( (*_colorFunc)(percentLifeRemaining, (*i)->getPosition() ) );
		}

		// apply the vector field function, if we have one
		if ( this->_vecFieldFunc != NULL ) {
			(*i)->setVel( (*_vecFieldFunc)((*i)->getPosition(), this->getFuncParams()) ) ;
		}
		

		if( (*i)->getAlpha() > 0.1 ) visibleCount++;
		
		_vertices.push_back((*i)->getPosition());
		_colors.push_back((*i)->getColor());

	}

	//MUTEX_LOCK(IMAGINARY);
	_cachedVisibleCount = visibleCount;
	//MUTEX_UNLOCK(IMAGINARY);
}

void ParticleSystem::setRespawnFlag( bool flag )
{
    vector<ParticleP>::iterator i;

    for( i = this->_particles.begin() ; i != this->_particles.end() ; ++i )
    {
        (*i)->setParticleRespawnFlag( flag );
    }
}

void ParticleSystem::setVectorField(vec3 (*vectorFieldFunc)(vec4, Parameters*) )
{
	this->_vecFieldFunc = vectorFieldFunc ;
}

void 
ParticleSystem::setColorFunc(vec4 (*ColorFunc)(float, vec4) )
{
	this->_colorFunc = ColorFunc ;
}

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

/**
 * ParticleSystem::generateLifespan()
 * Randomly generates a lifespan within a min and max range. Random die
 * roll allows for 1 out of N particles to live longer than maxLifeMinor.
 * @return a randomly generated "weighted" lifespan
 * FIXME: ???
 */
float
ParticleSystem::generateLifespan(){
	// maxLifeMinor set to 75% of possible life range.

	float maxLifeMinor = ((this->_maxLife - this->_minLife) * .3) + this->_minLife;
	int dieRoll = rand();
	float life = 0.0;

	// Keep rolling for random life until it is less than maxLifeMinor or
	// greater than maxLifeMinor and successful die roll.
	while (1){
		life = rangeRandom(this->_minLife, this->_maxLife);
		// 1 of every 10000 particles live longer than maxLifeMinor
		if( (dieRoll == 1 && life > maxLifeMinor) || ( life < maxLifeMinor ) ) {
			break;
		}
	}
	return life;
}


void ParticleSystem::pauseTheSystem(void)
{
	_pauseTheSystem = true;
}

void ParticleSystem::unpauseTheSystem(void)
{
	_pauseTheSystem = false;
}

void ParticleSystem::setEnableTheSystem( bool theBool )
{
    _enableTheSystem = theBool;
}

bool ParticleSystem::getEnableTheSystem( void )
{
    return _enableTheSystem;
}

void ParticleSystem::togglePause(void)
{
	_pauseTheSystem =  !_pauseTheSystem ;
}


void ParticleSystem::setPause(bool b)
{
	_pauseTheSystem = b ;
}

void ParticleSystem::setFuncParams(Parameters* theParameters)
{
	_funcParams = theParameters ;
}

Parameters* ParticleSystem::getFuncParams(void)
{
    return _funcParams;
}

UserVectorField *ParticleSystem::uvf() {
  return _uvf;
}

void ParticleSystem::switchUVF( UserVectorField *newUVF ) {
  UserVectorField *old = _uvf;
  _uvf = newUVF;
  delete old;
}

// Nada. Don't buffer particles to the raytracer,
// That's crazy-talk!
void ParticleSystem::sceneToRaytracer( RayTracer &rt ) { /* WOW, NOTHING.*/ }
void ParticleSystem::bufferToRaytracer( RayTracer &rt ) { /* WOW, NOTHING.*/ }

