/**
 @file partTest2.cpp
 @author Nicholas St.Pierre
 @authors John Huston, Nicholas VerVoort, Chris Compton
 @date 2013-02-23
 @details This is a tech demo for a particle system
 */

#include "Engine.hpp"
/* System Headers */
#include <cmath>
#include <cstdio>
#include <sstream>
#include <cstdlib>
#include <time.h>

// variable used to initialize the particle system
// If there is an argv[1], we will use it to initialize the particle system.
int numberOfParticles = 9999;

// Type Aliases
using Angel::vec3;
using Angel::vec4;

// Initialization: load and compile shaders, initialize camera(s), load models.
void init() {
  
  GLuint particleSystemShader;
  Screen *primScreen = Engine::instance()->mainScreen();
  Scene *rootScene = Engine::instance()->rootScene();
  
  particleSystemShader = Angel::InitShader( "shaders/vParticle2.glsl",
                                            "shaders/fParticle2.glsl",
                                            "shaders/gParticle.glsl",//"shaders/gParticle.glsl",
      GL_TRIANGLES, GL_TRIANGLE_STRIP, 12 );
  
  rootScene->shader( particleSystemShader );
  primScreen->_camList.shader( particleSystemShader );
  
  {
    ParticleSystem *particleSystem = new ParticleSystem( numberOfParticles, 
							 "emitter", 
							 particleSystemShader );

    particleSystem->setLifespan(8.0, 16.0);
    //particleSystem->setVectorField( ParticleFieldFunctions::userSupplied ) ;
    particleSystem->setVectorField( ParticleFieldFunctions::flame );
    particleSystem->setColorFunc( ColorFunctions::flame ) ;
    particleSystem->setEmitterRadius( 0.04 ) ;
    particleSystem->setEmitterShape(PS_HEMI_D);
    particleSystem->drawMode( GL_TRIANGLES ) ; // NEED THIS IF WE USE A GEOMETRY SHADER!
    //particleSystem->setSlaughterHeight(0.2455);
    //particleSystem->_trans._displacement.set(0.0, 0.25, 0.0);
    //particleSystem->_trans._offset.set( 0.0, 0.2, 0.0 );
    particleSystem->propagateOLD();
    //particleSystem->fillSystemWithParticles();
    rootScene->insertObject( particleSystem );
    // PARTICLE SYSTEMS buffer() THEMSELVES
  }
  
  glClearColor( 0.0, 0.0, 0.0, 1.0 );
  // if not using geo shader, we need this to render visible points
  // glPointSize( 1.1 );
}

//----------------------------------------------------------------------------------------

void simpleRotateAnim( TransCache &obj ) {
  obj._rotation.rotateY( tick.scale() * 1.0 );
  obj._offset.set( 1.5, 0, 0 );
  obj._orbit.rotateY( tick.scale() * -1.0 );
}

void part_idle( void ) {
  
  //Object &Emitter = *(theScene["emitter"]);
  //Emitter.animation( simpleRotateAnim );
  
}

int main( int argc, char **argv ) {
  
  if ( argc == 2 ) numberOfParticles = atoi( argv[1] );
  else std::cerr
      << "****************************************************************"
      << std::endl
      << "* PLEASE RUN THIS PROGRAM WITH A NUMBER AS ITS FIRST ARGUMENT! *"
      << std::endl
      << "****************************************************************"
      << std::endl;
  
  Engine::instance()->init( &argc, argv, "Particle Demo #2" );
  init();
  Engine::instance()->registerIdle( part_idle );
  
  /* PULL THE TRIGGER */
  glutMainLoop();
  return EXIT_SUCCESS;
  
}
