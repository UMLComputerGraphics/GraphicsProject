/**
   @file dumb.cpp
   @author Nicholas St.Pierre
   @authors John Huston, Nicholas VerVoort, Chris Compton
   @date 2013-02-23
   @details This is a tech demo for a particle system
*/

#include "Engine.hpp"
#include "ParticleSystem.hpp"
/* Utilities and Common */
#include "glut_callbacks.h"
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
int numberOfParticles = 100 ;

// Type Aliases
using Angel::vec3;
using Angel::vec4;
typedef Angel::vec4 color4;
typedef Angel::vec4 point4;

// Global objects for magical camera success
Screen myScreen( 800, 600 );
bool fixed_yaw = true;

// Initialization: load and compile shaders, initialize camera(s), load models.
void init() 
{

  GLuint  particleSystemShader, bottleShader;
  Screen *primScreen = Engine::instance()->mainScreen();
  Scene  *rootScene = Engine::instance()->rootScene();


  bottleShader         = Angel::InitShader("shaders/vmorph.glsl",
					   "shaders/fmorph.glsl");

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


  {
    ParticleSystem *particleSystem = new ParticleSystem( numberOfParticles, "ParticleSystem2", particleSystemShader );
    particleSystem->setLifespan(15.0,17.5);    particleSystem->setLifespan(25.0, 26.0);
    rootScene->insertObject( particleSystem );
    particleSystem->_trans._displacement.set(0.20, 0.0, 0.0);
    particleSystem->propagate();
    particleSystem->buffer();
  }

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
  glClearColor( 0.1, 0.3, 0.7, 1.0 );

  // also need this to render visible points
  glPointSize( 0.875 );
}

void cleanup( void ) 
{
  Engine::instance()->rootScene()->delObject();
}

//--------------------------------------------------------------------

void draw( void )
{
  static Scene *theScene  = Engine::instance()->rootScene();
  static Cameras *camList = Engine::instance()->cams();

  theScene->draw();
  camList->draw();
}

// GLUT display callback. Effectively calls displayViewport per-each Camera.
void display( void ) 
{
  static Cameras *camList = Engine::instance()->cams();
  
  // Clear the buffer.
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  // Tell camList to draw using our 'draw' rendering function.
  camList->view( draw );

  // Swap to the next buffer.
  glutSwapBuffers();

}

void idle( void ) 
{
  static Cameras *camList = Engine::instance()->cams();

  // Compute the time since last idle().
  tick.tock();

  // Move all camera(s).
  camList->idleMotion();
  glutPostRedisplay();
}

int main( int argc, char **argv ) {

  if ( argc == 2 )
    numberOfParticles = atoi(argv[1]);
  else
    std::cerr << "****************************************************************" << std::endl
	      << "* PLEASE RUN THIS PROGRAM WITH A NUMBER AS ITS FIRST ARGUMENT! *" << std::endl
	      << "****************************************************************" << std::endl;

  // OS X suppresses events after mouse warp.  This resets the suppression 
  // interval to 0 so that events will not be suppressed. This also found
  // at http://stackoverflow.com/questions/728049/
  // glutpassivemotionfunc-and-glutwarpmousepointer
#ifdef __APPLE__
  CGSetLocalEventsSuppressionInterval( 0.0 );
#endif
  Util::InitRelativePaths(argc, argv);

  glutInit( &argc, argv );
  glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
  glutInitWindowSize( myScreen.width(), myScreen.height() );
  glutCreateWindow( "Particle Test" );
  glutFullScreen();
  glutSetCursor( GLUT_CURSOR_NONE );

  GLEW_INIT();
  init();

  /* Register our Callbacks */
  glutDisplayFunc( display );
  glutKeyboardFunc( engineKeyboard );
  glutKeyboardUpFunc( engineKeylift );
  glutSpecialFunc( engineSpecialKeyboard );
  glutMouseFunc( engineMouse );
  glutMotionFunc( engineMouseMotion );
  glutPassiveMotionFunc( EngineMousePassive );
  glutIdleFunc( idle );
  glutReshapeFunc( engineResize );

  /* PULL THE TRIGGER */
  glutMainLoop();
  return EXIT_SUCCESS;

}
