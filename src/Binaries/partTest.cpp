/**
   @file partTest.cpp
   @author Nicholas St.Pierre
   @authors John Huston, Nicholas VerVoort, Chris Compton
   @date 2013-02-23
   @details This is a tech demo for a particle system
*/

#include "Engine.hpp"
#include "ParticleSystem.hpp"
/* Utilities and Common */
#include "ObjLoader.hpp"
#include "InitShader.hpp"
#include "model.hpp"
/* System Headers */
#include <cmath>
#include <cstdio>
#include <sstream>
#include <cstdlib>
#include <time.h>

// variable used to initialize the particle system
// If there is an argv[1], we will use it to initialize the particle system.
int numberOfParticles = 400000 ;

// Initialization: load and compile shaders, initialize camera(s), load models.
void init() 
{

  GLuint  particleSystemShader/*, bottleShader*/;
  Screen *primScreen = Engine::instance()->mainScreen();
  Scene  *rootScene = Engine::instance()->rootScene();


  /*  bottleShader         = Angel::InitShader("shaders/vmorph.glsl",
      "shaders/fmorph.glsl");*/

  particleSystemShader = Angel::InitShader("shaders/vParticle.glsl",
					   "shaders/fParticle.glsl");
                                         //"shaders/gParticle.glsl");

  rootScene->shader(particleSystemShader);
  primScreen->_camList.shader(particleSystemShader);

  primScreen->_camList.addCamera( "Camera1" );
  primScreen->_camList.next();

  /*
  {
    Object *bottle = rootScene->addObject( "bottle" );
    ObjLoader::loadModelFromFile( bottle, "../models/bottle-a.obj" );
    bottle->_trans._scale.set( 0.01 );
    bottle->buffer();
  }
  */

  /*
  {

    Object *my_sphere = rootScene->addObject( "sphere" );
    sphere(my_sphere);


  }
  */


  {
    ParticleSystem *particleSystem = new ParticleSystem( numberOfParticles, "ParticleSystem1", particleSystemShader );
    particleSystem->setLifespan(15.0,17.5);    particleSystem->setLifespan(25.0, 26.0);
    particleSystem->setEmitterRadius( 0.4 ) ;
    rootScene->insertObject( particleSystem );
    particleSystem->_trans._rotation.rotateY(M_PI);
    particleSystem->propagate();
    particleSystem->fillSystemWithParticles();
    particleSystem->buffer();
  }

  {
    ParticleSystem *particleSystem = new ParticleSystem( numberOfParticles, "ParticleSystem1b", particleSystemShader );
    particleSystem->setLifespan(15.0,17.5);    particleSystem->setLifespan(25.0, 26.0);
    particleSystem->setEmitterRadius( 0.1 ) ;
    rootScene->insertObject( particleSystem );
    particleSystem->propagate();
    particleSystem->fillSystemWithParticles();
    particleSystem->buffer();
  }

  /*
  {
    ParticleSystem *particleSystem = new ParticleSystem( numberOfParticles, "ParticleSystem2", particleSystemShader );
    particleSystem->setLifespan(15.0,17.5);    particleSystem->setLifespan(25.0, 26.0);
    rootScene->insertObject( particleSystem );
    particleSystem->_trans._displacement.set(0.20, 0.0, 0.0);
    particleSystem->propagate();
    particleSystem->buffer();
  }
  */
  /*
  {
    ParticleSystem *particleSystem = new ParticleSystem( numberOfParticles, "ParticleSystem3", particleSystemShader );
    particleSystem->setLifespan(15.0,17.5);    particleSystem->setLifespan(8.0, 18.0);
    rootScene->insertObject( particleSystem );
    particleSystem->_trans._displacement.set(0.0, 0.20, 0.0);
    particleSystem->propagate();
    particleSystem->buffer();
  }

  {
    ParticleSystem *particleSystem = new ParticleSystem( numberOfParticles, "ParticleSystem4", particleSystemShader );
    particleSystem->setLifespan(8.0,18.0);
    rootScene->insertObject( particleSystem );
    particleSystem->_trans._displacement.set(-0.20, 0.0, 0.0);
    particleSystem->propagate();
    particleSystem->buffer();
  }

  {
    ParticleSystem *particleSystem = new ParticleSystem( numberOfParticles, "ParticleSystem5", particleSystemShader );
    particleSystem->setLifespan(8.0, 18.0);
    rootScene->insertObject( particleSystem );
    particleSystem->_trans._displacement.set(0.0, -0.20, 0.0);
    particleSystem->propagate();
    particleSystem->buffer();
  }
  */

  // Generic OpenGL setup: Enable the depth buffer and set a nice background color.
  glEnable( GL_DEPTH_TEST );
  glClearColor( 0.2, 0.1, 0.1, 1.0 );

  // also need this to render visible points
  glPointSize( 1.1 );
}

void cleanup( void ) 
{
  Engine::instance()->rootScene()->delObject();
}

//--------------------------------------------------------------------

int main( int argc, char **argv ) {

  if ( argc == 2 )
    numberOfParticles = atoi(argv[1]);
  else
    std::cerr << "****************************************************************" << std::endl
	      << "* PLEASE RUN THIS PROGRAM WITH A NUMBER AS ITS FIRST ARGUMENT! *" << std::endl
	      << "****************************************************************" << std::endl;

  Engine::instance()->init( &argc, argv, "Particle Test" );
  init();

  /* PULL THE TRIGGER */
  glutMainLoop();
  return EXIT_SUCCESS;

}
