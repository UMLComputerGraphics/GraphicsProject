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
#define MIN_EMITTER_RADIUS 1.0/1024.0
#endif

#ifndef NUM_PARTICLES_TO_ADD_ON_UPDATE
#define NUM_PARTICLES_TO_ADD_ON_UPDATE 3
#endif

// Constructor(s)
ParticleSystem::ParticleSystem( int particleAmt, const std::string &name,
		GLuint shader ) :
    				Object( name, shader ), _numParticles( particleAmt ), _minLife( 0.1 ),
    				_maxLife( 1 ), _emitterRadius( 0.0 ), _pauseTheSystem( false ),
    				_slaughterHeight( 0.0 ), _updateRequired( false ),
    				_useGlobalParticleSpace( false ), _fillSpeedLimit( 5 ),
    				_vecFieldFunc( NULL ), _colorFunc( NULL ) {
	this->drawMode( GL_POINTS );
	this->_vecFieldFunc = NULL;
	setColorFunc( ColorFunctions::standard );
	this->setVectorField( ParticleFieldFunctions::up );
}

ParticleSystem::~ParticleSystem( void ) {
	for ( size_t i = 0; i < _particles.size(); i++ ) {
		free( _particles[i] );
	}
	_particles.clear();
}

vec4
ParticleSystem::getRandomCircularSpawnPoint(void)
{

	vec4 ret;

	float randomTheta ;
	float jitteredRadius ;

	//randomTheta = rangeRandom( 0.0, 360.0 ); // if degrees
	randomTheta = rangeRandom( 0.0, 2.0*M_PI ); //if radians
	jitteredRadius = rangeRandom(this->_emitterRadius/100, this->_emitterRadius);

	ret.x = cos(randomTheta)*(jitteredRadius);
	ret.y = 0.0 ;
	ret.z = sin(randomTheta)*(jitteredRadius);
	ret.w = 1.0 ; // ??? I don't know if this matters


	return ret;

}

vec4
ParticleSystem::getRandomHemisphericalSpawnPoint(void)
{

	vec4 ret;
	// Generate a random position on a circle of radius from particle system
	float randomTheta ;
	float randomPhi ;
	float jitteredRadius ;


	//randomTheta = rangeRandom( 0.0, 360.0 ); // if degrees
	randomTheta = rangeRandom( 0.0, 2.0 * M_PI ); //if radians
	randomPhi   = rangeRandom( 0.0,       M_PI );

	jitteredRadius = rangeRandom(this->_emitterRadius/100, this->_emitterRadius);

	// dont reindent this
	ret.x = sin(randomPhi) * cos(randomTheta) * (jitteredRadius);
	ret.y = cos(randomPhi) *                    (jitteredRadius);
	ret.z = sin(randomPhi) * sin(randomTheta) * (jitteredRadius);
	ret.w = 1.0 ; // ??? I don't know if this matters


	return ret;


}

/*
void  setAlpha( float newAlpha );
void  setColor( vec4 newColor );
void  setScale( vec3 newScale );
void  setVel( vec3 newVel );
 */
void
ParticleSystem::respawnParticle(Particle &p)
{

	{ // new position
		vec4  spawnPosition = vec4(0.0,0.0,0.0,1.0);

		if ( getParticleSpace() ) {
			spawnPosition = this->_trans.otm() * spawnPosition ;
		}

		if ( this->_emitterRadius >= MIN_EMITTER_RADIUS ) {
			spawnPosition += this->getRandomHemisphericalSpawnPoint();
		}
		p.setPos(spawnPosition);
	}

	p.setLifetime( p.getMaxLifetime() ) ;

	//    Particle *p = new Particle( spawnPosition, 1, generateLifespan() );

}

Particle*
ParticleSystem::newRandomParticle(void)
{

	vec4  spawnPosition = vec4(0.0,0.0,0.0,1.0);

	if ( getParticleSpace() ) {
		spawnPosition = this->_trans.otm() * spawnPosition ;
	}

	if ( this->_emitterRadius >= MIN_EMITTER_RADIUS ) {
		//spawnPosition += this->getRandomCircularSpawnPoint();
		spawnPosition += this->getRandomHemisphericalSpawnPoint();
	}

	// Generate a particle on the random position (if radius was big enough)
	Particle *p = new Particle( spawnPosition, 1, generateLifespan() );

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
//		this->setNumParticles( getNumParticles() + 1 );
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
	//		this->setNumParticles( getNumParticles() - 1 );
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
ParticleSystem::getMaxLife( void ) {
	return _maxLife;
}

float
ParticleSystem::getMinLife( void ) {
	return _minLife;
}

int
ParticleSystem::getNumParticles( void ) {
	return _numParticles;
}

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

void ParticleSystem::draw( void ) {
	static const TransCache newTrans = TransCache();

	// we should consider moving the update() call to the idle() loop
	update();

	// if we don't have any particles, bail
	if ( _vertices.size() <= 0 ) return;

	buffer();


	TransCache tempTrans = _trans ;

	if ( getParticleSpace() ) {
		tempTrans = _trans ;
		_trans = newTrans;
	}


	// This binds the vertex array and sets the active shader,
	// And sends all of the Object's uniforms.
	Object::drawPrep();

	// We can handle this ourselves, because we're pretty.
	glDrawArrays( _drawMode, 0, _vertices.size() );

	// Unbind our vertex, then draw children if any.
	glBindVertexArray(0);
	Scene::draw();

	// if we are using a detached thingy
	if ( getParticleSpace() ) {
		_trans = tempTrans ;
	}

}

bool 
ParticleSystem::getParticleSpace(void)  const
{
	return   _useGlobalParticleSpace;
}

void 
ParticleSystem::setParticleSpace( bool flag )
{
	_useGlobalParticleSpace = flag ;
}

void
ParticleSystem::setEmitterRadius( float r )
{
	this->_emitterRadius = r ;
}

//Update the particles in our system >>> AND ALSO UPDATE OUR DRAW BUFFER
void
ParticleSystem::update() {

	// used to stagger the creation of particles
	static unsigned currFillFrame=0;

	// if we are paused, don't change a damn thing.
	if ( this->_pauseTheSystem ) return;

	// if we need more particles...
	if ( _particles.size() < (unsigned int) this->_numParticles ){

		if ( currFillFrame == _fillSpeedLimit ) {
			updateNumParticles( NUM_PARTICLES_TO_ADD_ON_UPDATE );
			currFillFrame = 0;
		}
		currFillFrame++;
	}

	// if we need fewer particles...
	else if ( ( _particles.size() > (unsigned int) this->_numParticles ) &&
		  ( abs(this->_numParticles - (int)_particles.size() ) >= NUM_PARTICLES_TO_ADD_ON_UPDATE ) 
		  //^^ condition to make sure the system doesn't bang back and forth between values
		  ){

		if ( currFillFrame == _fillSpeedLimit ) {
			updateNumParticles( -1 * NUM_PARTICLES_TO_ADD_ON_UPDATE );
			currFillFrame = 0;
		}
		currFillFrame++;
	}

	_vertices.clear();
	_colors.clear();

	vector<ParticleP>::iterator i;
	//float maxHeight = this->_slaughterHeight ;

	for( i = _particles.begin() ; i != _particles.end() ; ++i) {


		float percentLifeRemaining = (*i)->getLifetime()/(*i)->getMaxLifetime();
		// call the update function on each particle
		(*i)->updateSelf();
		(*i)->setColor( (*_colorFunc)(percentLifeRemaining, (*i)->getPosition() ) );

		if( ((*i)->getLifetime() <= 0.0)
		/*|| ((*i)->getPosition().y >= maxHeight)*/ ) {
			respawnParticle(**i) ;
		}

		// apply the vector field function to the particle
		if ( this->_vecFieldFunc != NULL ) {
			(*i)->setVel( (*_vecFieldFunc)((*i)->getPosition() ) ) ;
		}

		_vertices.push_back((*i)->getPosition());
		_colors.push_back((*i)->getColor());
	}
}

void 
ParticleSystem::setVectorField(vec3 (*vectorFieldFunc)(vec4) )
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

void ParticleSystem::togglePause(void)
{
	_pauseTheSystem =  !_pauseTheSystem ;
}


void ParticleSystem::setPause(bool b)
{
	_pauseTheSystem = b ;
}
/* //square gen method
	float tempXV, tempYV, tempZV ;
	tempXV = rangeRandom( -0.001f, 0.001f );
	tempYV = rangeRandom( -0.001f, 0.001f );
	tempZV = rangeRandom( -0.001f, 0.001f );
	(*i)->setVel( vec3( tempXV, tempYV, tempZV ) );

	// sphere generating method
	float row   = rangeRandom( -0.001f, 0.002f ); // equivalent to magnitude
	float phi   = rangeRandom( 0.0f, 2 * M_PI );
	float theta = rangeRandom( 0.0f, 2 * M_PI );

	(*i)->setVel( vec3( row*sin(phi)*cos(theta),
	row*sin(phi)*sin(theta),
	row*cos(phi) ));
 */
