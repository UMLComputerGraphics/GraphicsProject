/**
 * @file vf2.cpp
 * @author Nick1
 * @authors Nick1 and Nick2
 * @date 2013-02-20
 * @brief Dual-shader demo.
 * @details Work in progress!
 * Based loosely on Ed Angel's tutorials.
 **/

/* Engine Classes */
#include "Engine.hpp"

typedef vec4 color4;

int numberOfParticles = 999;

/**
 * Initialization: load and compile shaders, initialize camera(s), load models.
 */
void init() {
  
  GLuint ashader;
  Cameras *camList = Engine::instance()->cams();
  Scene *rootScene = Engine::instance()->rootScene();
  
  GLuint particleSystemShader;
  Screen *primScreen = Engine::instance()->mainScreen();
  
  ashader = Angel::InitShader( "shaders/vEngine.glsl", "shaders/fEngine.glsl" );
  particleSystemShader = Angel::InitShader( "shaders/vParticle2.glsl",
                                            "shaders/fParticle2.glsl",
                                            "shaders/gParticle.glsl", GL_POINTS,
                                            GL_TRIANGLE_STRIP, 12 );
  
  rootScene->shader( particleSystemShader );
  primScreen->_camList.shader( particleSystemShader );
  
  camList->active()->changePerspective( Camera::IDENTITY );
  
  Object *vfield = rootScene->addObject( "vfield", ashader );
  
  vfield->drawMode( GL_LINES );
  
  float resolution = 0.002;
  vec4 vecEnd;
  vec4 vecBegin;
  
  float start = -.012;
  float stop = .012;
  float scale = 2.0; // viewability hack
  
  //  float y = 0.0 ;
  
  for ( float x = start; x <= stop; x += resolution ) {
    if ( x == 0.0 ) continue;
    for ( float y = 0.0; y <= stop * 4.0; y += resolution * 2.0 ) {
      for ( float z = start; z <= stop; z += resolution ) {
        if ( z == 0.0 ) continue;
        vecBegin = vec4( x, y, z, 1.0 );
        vfield->_vertices.push_back( vecBegin );
        
        vecEnd = vec4( ParticleFieldFunctions::flame( vecBegin, NULL ), 1.0 );
        //	vecEnd = normalize(vecEnd);
        
        vecEnd.x += vecBegin.x;
        vecEnd.y = vecBegin.y;
        vecEnd.z += vecBegin.z;
        
        vecEnd.x *= scale;
        vecEnd.z *= scale;
        
        vecEnd.w = 1.0;
        
        vfield->_vertices.push_back( vecEnd );
        vfield->_colors.push_back( color4( 0.0, 0.0, 1.0, 1.0 ) );
        vfield->_colors.push_back( color4( 1.0, 1.0, 1.0, 1.0 ) );
        
        //std::cout << vecEnd.x << vecEnd.y << vecEnd.z << std::endl ;
        
      }
    }
  }
  
  vfield->buffer();
  
  {
    ParticleSystem *particleSystem = new ParticleSystem( numberOfParticles,
                                                         "emitter",
                                                         particleSystemShader );
    
    particleSystem->setLifespan( 14.0, 18.0 );
    particleSystem->setVectorField( ParticleFieldFunctions::flame );
    particleSystem->setEmitterRadius( 0.02 );
    particleSystem->drawMode( GL_TRIANGLES );
    particleSystem->propagateOLD();
    
    rootScene->insertObject( particleSystem );
    
  }
  
  glLineWidth( 1.0 );
  glClearColor( 0, 0, 0, 1.0 );
  
}

//--------------------------------------------------------------------

/**
 * This is a dual-shader demo! It looks very simple,
 * but it illustrates quickly and effectively how to use two shaders.
 *
 * @param argc Not used.
 * @param argv Not used.
 * @return EXIT_SUCCESS.
 *
 */
int main( int argc, char **argv ) {
  
  Engine::instance()->init( &argc, argv, "Multi-Shader Demo" );
  init();
  
  glutMainLoop();
  return EXIT_SUCCESS;
  
}
