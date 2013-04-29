/**
 * @file ds.cpp
 * @author John Huston
 * @authors John Huston, Greg Giannone
 * @date 2013-02-20
 * @brief Dual-shader demo.
 * @details Work in progress!
 * Based loosely on Ed Angel's tutorials.
 **/

/* Engine Classes */
#include "Engine.hpp"

/**
 * Initialization: load and compile shaders, initialize camera(s), load models.
 */
void init() {
  
  GLuint shader[3];
  Cameras *camList = Engine::instance()->cams();
  Scene *rootScene = Engine::instance()->rootScene();
  
  shader[0] = Angel::InitShader( "shaders/vred.glsl", "shaders/fEngine.glsl" );
  shader[1] = Angel::InitShader( "shaders/vblu.glsl", "shaders/fEngine.glsl" );
  shader[2] = Angel::InitShader( "shaders/vEngineNoMorph.glsl",
                                 "shaders/fEngine.glsl" );
  
  camList->active()->changePerspective( Camera::IDENTITY );
  
  // Adding objects without a default shader:
  Object *A = rootScene->addObject( "Object A (RED)", shader[0] );
  
  // Setting a default and adding objects without:
  rootScene->shader( shader[1] );
  Object *B = rootScene->addObject( "Object B (BLU)" );
  
  // Third Object, over-ride default shader.
  Object *C = rootScene->addObject( "Object C (TEX)", shader[2] );
  
  // Fourth Object.
  Object *D = rootScene->addObject( "Object D (TEX2)", shader[2] );
  
  // draw a square in the upper left
  triangle( A, vec4( -1, 0, 0, 1 ), vec4( 0, 0, 0, 1 ), vec4( -1, 1, 0, 1 ),
            0 );
  triangle( A, vec4( 0, 0, 0, 1 ), vec4( -1, 1, 0, 1 ), vec4( 0, 1, 0, 1 ), 0 );
  A->buffer();
  
  // draw a square in the lower right
  triangle( B, vec4( 0, -1, 0, 1 ), vec4( 1, -1, 0, 1 ), vec4( 0, 0, 0, 1 ),
            0 );
  triangle( B, vec4( 1, -1, 0, 1 ), vec4( 0, 0, 0, 1 ), vec4( 1, 0, 0, 1 ), 0 );
  B->buffer();
  
  // draw a triangle in the lower left, and texture it.
  triangle( C, vec4( -1, -1, 0, 1 ), vec4( 0, -1, 0, 1 ), vec4( -1, 0, 0, 1 ),
            0 );
  C->_texUVs.push_back( vec2( 0, 0 ) );
  C->_texUVs.push_back( vec2( 1, 1 ) );
  C->_texUVs.push_back( vec2( 0, 1 ) );
  C->buffer();
  const char *filename = "../Textures/GrassGreenTexture0002.jpg";
  C->texture( filename );
  
  // draw another triangle in the lower left, texture it differently.
  triangle( D, vec4( -1, 0, 0, 1 ), vec4( 0, 0, 0, 1 ), vec4( 0, -1, 0, 1 ),
            0 );
  D->_texUVs.push_back( vec2( 0, 0 ) );
  D->_texUVs.push_back( vec2( 1, 1 ) );
  D->_texUVs.push_back( vec2( 0, 1 ) );
  D->buffer();
  D->texture( "../Textures/GoodTextures_0013291.jpg" );
  
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
  
  Engine::init( &argc, argv, "Multi-Shader Demo" );
  init();
  Engine::run();

  return EXIT_SUCCESS;
  
}
