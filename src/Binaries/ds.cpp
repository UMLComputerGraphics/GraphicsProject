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
/* Utilities and Common */
#include "model.hpp"
#include "InitShader.hpp"
#include "glut_callbacks.h"

/**
 * Initialization: load and compile shaders, initialize camera(s), load models.
 */
void init() {
  
  GLuint shader[3];
  Cameras *camList = Engine::instance()->cams();
  Scene *rootScene = Engine::instance()->rootScene();
  
  shader[0] = Angel::InitShader( "shaders/vred.glsl", "shaders/fragment.glsl" );
  shader[1] = Angel::InitShader( "shaders/vblu.glsl", "shaders/fragment.glsl" );
  shader[2] = Angel::InitShader( "shaders/vtex.glsl", "shaders/ftex.glsl" );
  
  camList->addCamera( "Camera1" );
  camList->next();
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
  triangle( D, vec4( -1, 0, 0, 1), vec4( 0, 0, 0, 1 ), vec4( 0, -1, 0, 1 ), 0 );
  D->_texUVs.push_back( vec2( 0, 0 ) );
  D->_texUVs.push_back( vec2( 1, 1 ) );
  D->_texUVs.push_back( vec2( 0, 1 ) );
  D->buffer();
  D->texture( "../Textures/GoodTextures_0013291.jpg" );

  glEnable( GL_DEPTH_TEST );
  glClearColor( 0, 0, 0, 1.0 );


  Engine::instance()->texMan()->rebind();
  
}

/**
 * Cleans up our scene graph.
 */
void cleanup( void ) {
  //Engine::instance()->rootScene()->DestroyObject();
}

//--------------------------------------------------------------------

/**
 * Implementation of drawing the display with regards to a single viewport.
 */
void draw( void ) {
  static Scene *theScene = Engine::instance()->rootScene();
  static Cameras *camList = Engine::instance()->cams();
  
  theScene->draw();
  camList->draw();
}

/**
 * Display/Render the entire screen.
 */
void display( void ) {
  static Cameras *camList = Engine::instance()->cams();
  
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  
  // Tell camList to draw using our 'draw' rendering function.
  camList->view( draw );
  
  glutSwapBuffers();
}

/**
 * Compute time since last idle, update camera positions, redisplay.
 */
void idle( void ) {
  
  static Cameras *camList = Engine::instance()->cams();
  
  // Compute the time since last idle().
  tick.tock();
  
  // Move all camera(s).
  camList->idleMotion();
  glutPostRedisplay();
  
}

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
  glutInitWindowSize( 0, 0 );
  glutCreateWindow( "Linear Interpolation Morphing Demo" );
  glutFullScreen();
  glutSetCursor( GLUT_CURSOR_NONE );
  
  GLEW_INIT();
  init();
  
  /* Register our Callbacks */
  glutDisplayFunc( display );
  glutKeyboardFunc( keyboard );
  glutKeyboardUpFunc( keylift );
  glutSpecialFunc( keyboard_ctrl );
  glutMouseFunc( mouse );
  glutMotionFunc( mouseroll );
  glutPassiveMotionFunc( mouselook );
  glutIdleFunc( idle );
  glutReshapeFunc( resizeEvent );

  fprintf( stderr, "GL_TEXTURE0: %x\n", GL_TEXTURE0 );
  fprintf( stderr, "GL_TEXTURE1: %x\n", GL_TEXTURE1 );
  
  /* PULL THE TRIGGER */
  glutMainLoop();
  return EXIT_SUCCESS;
  
}
