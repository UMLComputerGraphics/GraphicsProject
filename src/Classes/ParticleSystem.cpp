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
    						Object( name, shader ), numParticles( particleAmt ), minLife( 0.1 ),
    						maxLife( 1 ), _emitterRadius(0.0), pauseTheSystem(false)
{
	this->drawMode(GL_POINTS) ;
	this->_vecFieldFunc = NULL   ;
	//this->fillSystemWithParticles();
}

ParticleSystem::~ParticleSystem( void ) {
	for ( size_t i = 0; i < particles.size(); i++ ) {
		free( particles[i] );
	}
	particles.clear();
}

vec4
ParticleSystem::getRandomCircularSpawnPoint(void)
{

    vec4 ret;
    // Generate a random position on a circle of radius radius (passed as arg)
    float randomTheta ;
    float jitteredRadius ;

    // I LOVE THIS FUNCTION SO MUCH
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
    // Generate a random position on a circle of radius radius (passed as arg)
    float randomTheta ;
    float randomPhi ;
    float jitteredRadius ;

    // I LOVE THIS FUNCTION SO MUCH
    //randomTheta = rangeRandom( 0.0, 360.0 ); // if degrees
    randomTheta = rangeRandom( 0.0, 2.0 * M_PI ); //if radians
    randomPhi   = rangeRandom( 0.0,       M_PI ); //if radians

    jitteredRadius = rangeRandom(this->_emitterRadius/100, this->_emitterRadius);

    // dont reindent this
    ret.x = sin(randomPhi) * cos(randomTheta)*(jitteredRadius);
    ret.y = cos(randomPhi) *                  (jitteredRadius);
    ret.z = sin(randomPhi) * sin(randomTheta)*(jitteredRadius);
    ret.w = 1.0 ; // ??? I don't know if this matters


    return ret;


}


Particle*
ParticleSystem::newRandomParticle(void)
{


	vec4  spawnPosition ;

	if ( this->_emitterRadius < MIN_EMITTER_RADIUS ) {

		spawnPosition.x = spawnPosition.y = spawnPosition.z = 0.0 ;
		spawnPosition.w = 1.0 ;

	} else {
	  //spawnPosition = this->getRandomCircularSpawnPoint();
		spawnPosition = this->getRandomHemisphericalSpawnPoint();
	}

	// Generate a particle on the random position (if radius was big enough)
	Particle *p = new Particle( spawnPosition, 1, generateLifespan() );

	return p ;
}


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
ParticleSystem::setSlaughterHeight(float f){

	this->_slaughterHeight = f ;

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

	while( numParticles % 3 ) numParticles++;

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

	glDrawArrays( _drawMode, 0, _vertices.size() );

	glBindVertexArray(0);
	Scene::draw();

}


void
ParticleSystem::setEmitterRadius( float r )
{
	this->_emitterRadius = r ;
}






//Update the particles in our system >>> AND ALSO UPDATE OUR DRAW BUFFER
void
ParticleSystem::update() {

	static bool waitFlag = false ;

	if ( this->pauseTheSystem ) {
		return;
	}

	if ( particles.size() < (unsigned int) this->numParticles ){

		if ( !waitFlag )
			addSomeParticles( NUM_PARTICLES_TO_ADD_ON_UPDATE );

		waitFlag = !waitFlag ;

	}

	_vertices.clear();

	vector<ParticleP>::iterator i;
	//float maxHeight = this->_slaughterHeight ;

	for( i = particles.begin() ; i != particles.end() ; ++i) {

		// apply the vector field to the particle
		if ( this->_vecFieldFunc != NULL )
			(*i)->setVel( (*_vecFieldFunc)((*i)->getPosition() ) ) ;

		// call the update function on each particle
		(*i)->updateSelf();

		_vertices.push_back((*i)->getPosition());


		if( ((*i)->getLifetime() <= 0.0) /*|| ((*i)->getPosition().y >= maxHeight)*/ ) {
			(*i)->setPos( this->getRandomHemisphericalSpawnPoint() );
			(*i)->setLifetime( (*i)->getMaxLifetime() );

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

		}

	}

}



void ParticleSystem::setVectorField(vec3 (*vectorFieldFunc)(vec4) )
{
	this->_vecFieldFunc = vectorFieldFunc ;
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
 */
float
ParticleSystem::generateLifespan(){
	// maxLifeMinor set to 75% of possible life range.
	srand(time(NULL));
	float maxLifeMinor = ((this->maxLife - this->minLife) * .3) + this->minLife;
	int dieRoll = rand();
	float life = 0.0;

	// Keep rolling for random life until it is less than maxLifeMinor or
	// greater than maxLifeMinor and successful die roll.
	while (1){
		life = rangeRandom(this->minLife, this->maxLife);
		// 1 of every 10000 particles live longer than maxLifeMinor
		if( dieRoll == 1 && life > maxLifeMinor)
		{
			return life;
		}
		else if ( life < maxLifeMinor )
		{
			return life;
		}
	}
}
