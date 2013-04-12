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
ParticleSystem *funInTheSin;
double theta = 0.0;
GLuint thetaLoc;

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

  thetaLoc = glGetUniformLocation(particleSystemShader, "theta");

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


  funInTheSin = new ParticleSystem(numberOfParticles, "FunInTheSine", particleSystemShader);
  funInTheSin->setLifespan(0.5, 11.0);  
  funInTheSin->setEmitterRadius( 0.1 ) ;
  rootScene->insertObject( funInTheSin );
  funInTheSin->propagate();
  funInTheSin->fillSystemWithParticles();
  funInTheSin->buffer();
  
  tick.setTimeUniform(glGetUniformLocation( particleSystemShader, "ftime" ));

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


void part_idle()
{
	funInTheSin->setEmitterRadius(sin(theta));
	theta += 0.01;
	glUniform1f(thetaLoc, theta);
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
  Engine::instance()->registerIdle(part_idle);
  init();

  /* PULL THE TRIGGER */
  glutMainLoop();
  return EXIT_SUCCESS;

}
