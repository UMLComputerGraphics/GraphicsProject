/**
 @file partTest.cpp
 @author Nicholas St.Pierre
 @authors John Huston, Nicholas VerVoort, Chris Compton
 @date 2013-02-23
 @details This is a tech demo for a particle system
 */

#include "partTest.hpp"


partTest::partTest() {
    
}

partTest::~partTest() {
    
}

void partTest::init(void)
{
    
    GLuint  particleSystemShader/*, bottleShader*/;
    Screen *primScreen = Engine::instance()->mainScreen();
    Scene  *rootScene = Engine::instance()->rootScene();
    
    
    /*  bottleShader         = Angel::InitShader("shaders/vmorph.glsl",
     "shaders/fmorph.glsl");*/
    
    particleSystemShader = Angel::InitShader("shaders/vParticle.glsl",
                                             "shaders/fParticle.glsl");
    //"shaders/gParticle.glsl");
    
    _thetaLoc = glGetUniformLocation(particleSystemShader, "theta");
    
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
    
    
    _funInTheSin = new ParticleSystem(partTest::_numberOfParticles, "FunInTheSine", particleSystemShader);
    _funInTheSin->setLifespan(0.5, 11.0);
    _funInTheSin->setEmitterRadius( 0.1 ) ;
    rootScene->insertObject( _funInTheSin );
    _funInTheSin->propagateOLD();
    _funInTheSin->fillSystemWithParticles();
    _funInTheSin->buffer();
    
    tick.setTimeUniform(glGetUniformLocation( particleSystemShader, "ftime" ));
    
    // Generic OpenGL setup: Enable the depth buffer and set a nice background color.
    glEnable( GL_DEPTH_TEST );
    glClearColor( 0.2, 0.1, 0.1, 1.0 );
    
    // also need this to render visible points
    glPointSize( 1.1 );

}

void partTest::cleanup(void)
{
    Engine::instance()->rootScene()->delObject();
}

void partTest::idle(void)
{
    _funInTheSin->setEmitterRadius(sin(_theta));
	_theta += 0.01;
	glUniform1f(_thetaLoc, _theta);
}


void partTest::run(int &argc, char* argv[])
{
    if ( argc == 2 ){}
        //_numberOfParticles = atoi(argv[1]);
    else
        std::cerr << "****************************************************************" << std::endl
        << "* This program will run with the default number of particles 400000 *" << std::endl
        << "****************************************************************" << std::endl;
    Engine::instance()->init( &argc, argv, "Particle Test" );
    Engine::instance()->registerIdle(idle);
    init();

    
}