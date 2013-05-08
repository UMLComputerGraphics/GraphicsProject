/**
 @file partTest.cpp
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

#define ALPHAHACK 1

// variable used to initialize the particle system
// If there is an argv[1], we will use it to initialize the particle system.
int numberOfParticles = 60000;
ParticleSystem *funInTheSin;
double theta = 0.0;
GLuint thetaLoc;

// Initialization: load and compile shaders, initialize camera(s), load models.
void init() {
  
  GLuint particleSystemShader;
  Screen *primScreen = Engine::instance()->mainScreen();
  Scene  *rootScene  = Engine::instance()->rootScene();
  
  particleSystemShader = Angel::InitShader( "shaders/vScreenTex.glsl",
                                            "shaders/fScreenTex.glsl" );
  
  thetaLoc = glGetUniformLocation( particleSystemShader, "theta" );
  
  rootScene->shader( particleSystemShader );
  primScreen->_camList.shader( particleSystemShader );

  funInTheSin = new ParticleSystem( numberOfParticles, "FunInTheSine",
                                    particleSystemShader );
  funInTheSin->setLifespan( 8.0, 14.0 );
  funInTheSin->setEmitterRadius( 0.01 );

  rootScene->insertObject( funInTheSin );

  funInTheSin->setEmitterShape(PS_LINE);
  funInTheSin->setVectorField( ParticleFieldFunctions::up );
  //funInTheSin->texture("../Textures/texture_logo.png");
  funInTheSin->texture("../Textures/texture_logo_b.png");

  funInTheSin->setNumToAddEachFrame(200);



  funInTheSin->_trans.push(TransMat(0,-0.5,0));

  funInTheSin->buffer();
  
  tick.setTimeUniform( glGetUniformLocation( particleSystemShader, "ftime" ) );
  
  // Generic OpenGL setup: Set a nice background color.
  glClearColor( 0.1, 0.1, 0.1, 1.0 );
  // also need this to render visible points
  glPointSize( 2.5 );
}


void simpleRotateY( TransCache &obj ) {
  obj._rotation.rotateY( tick.scale() * 1.5 );
}

void part_idle() {

  float s = sin( theta*2.2 );
  float c = cos( theta*2.2 );

  funInTheSin->setEmitterRadius( (c*c+(1-c)/2)/2 + 0.02);
  theta += 0.01;

  funInTheSin->_emitterLoc._displacement.set(s/10);
  funInTheSin->propagateOLD();
  //glUniform1f( thetaLoc, theta );

}

//--------------------------------------------------------------------

int main( int argc, char **argv ) {
  
  if ( argc == 2 ) numberOfParticles = atoi( argv[1] );
  else std::cerr
      << "****************************************************************"
      << std::endl
      << "* PLEASE RUN THIS PROGRAM WITH A NUMBER AS ITS FIRST ARGUMENT! *"
      << std::endl
      << "****************************************************************"
      << std::endl;
  
  Engine::instance()->init( &argc, argv, "Particle Test" );
  Engine::instance()->registerIdle( part_idle );
  init();
  
  /* PULL THE TRIGGER */
  glutMainLoop();
  return EXIT_SUCCESS;
  
}
