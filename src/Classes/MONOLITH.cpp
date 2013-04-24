/**
 @file MONOLITH.cpp
 @author UMLComputerGraphics <https://github.com/UMLComputerGraphics>
 @date 2013-03-29
 @brief This is a monolith of every component.
 @details Final project rough draft.
 Original engine based on Ed Angel's book code.
 **/

#include "MONOLITH.hpp"

MONOLITH::~MONOLITH( void ) {
  
}

/* Default and only constructor */
MONOLITH::MONOLITH( int argc, char** argv ) {
  _argc = argc;
  _argv = argv;
}

/**
 * Cleans up our scene graph.
 */
void MONOLITH::cleanup( void ) {
  Engine::instance()->rootScene()->delObject();
}

/**
 * Apply animations and whatever else your heart desires.
 */
void MONOLITH::monolith_idle( void ) {
  static Scene *rootScene = Engine::instance()->rootScene();
  
  // Animation variables.
  double timer = glutGet( GLUT_ELAPSED_TIME ) / 500.0;
  float percent = (sin( timer ) + 1) / 2;
  
  Object &candle = *((*rootScene)["Candle"]);
  candle.animation( simpleRotateAnim );
  
  // Update the morph percentage.
  (*rootScene)["bottle"]->morphPercentage( percent );
}

#ifndef WITHOUT_QT
void MONOLITH::slotParticleAdd(int value)
{ 
  ps->setNumParticles(value);
}
#endif //WITHOUT_QT
/**
 * This will initialize and run MONOLITH
 */
void MONOLITH::run() {
  Engine::instance()->init( &_argc, _argv,
                            "WE ARE THE BORG. RESISTANCE IS FUTILE!" );
  Engine::instance()->registerIdle( monolith_idle );
  
  // Get handles to the Scene and the Screen.
  rootScene = Engine::instance()->rootScene();
  primScreen = Engine::instance()->mainScreen();
  
  shader[0] = Angel::InitShader( "shaders/vEngine.glsl",
                                 "shaders/fMONOLITH.glsl" );
  shader[1] = rootScene->shader(); // Default shader we've already compiled.
  shader[2] = Angel::InitShader( "shaders/vParticle.glsl",
                                 "shaders/fFlameParticle.glsl" );
  
  tick.setTimeUniform( glGetUniformLocation( shader[1], "ftime" ) );
  
  // Create the Bottle Object handle...
  bottle = rootScene->addObject( "bottle" );
  // Load model from file.
  ObjLoader::loadModelFromFile( bottle, "../models/bottle_wine_high.obj" );
  ObjLoader::loadMaterialFromFile( bottle, "../models/bottle_wine_high.mtl" );
  {
    // Scale the bottle down!
    bottle->_trans._scale.set( 0.30 );
    
    for ( uint i = 0; i < bottle->_colors.size(); i++ ) {
      bottle->_colors[i].w = 0.4;
    }
    
    // Object class has-a pointer to an object which is the morph target.
    // they are created and buffered as follows:
    
    // this makes a new object and links it to the source object. it returns the addr of the new obj..
    bottle->genMorphTarget();
    
    // this obscure allusion to "the thong song" brought to you by Eric McCann
    GLuint sisqo = glGetUniformLocation( bottle->shader(),
                                         "letMeSeeThatPhong" );
    glUniform1f( sisqo, true );
    
    // we can get the addr of the morph object like this, also.
    Object *bottleMorphTarget = bottle->morphTarget();
    
    // with this model, we can use all the preexisting Object class functionality
    ObjLoader::loadModelFromFile( bottleMorphTarget,
                                  "../models/bottle_liquor_high.obj" );
    ObjLoader::loadMaterialFromFile( bottleMorphTarget,
                                     "../models/bottle_liquor_high.mtl" );
    bottleMorphTarget->_trans._scale.set( 0.30 );
    
    for ( uint i = 0; i < bottle->_colors.size(); i++ ) {
      bottleMorphTarget->_colors[i].w = 0.4;
    }
    
    // YES THIS IS THE REAL OBJECT, NOT THE TARGET.
    // IT SENDS THE MORPH VERTICES TO THE SHADER, NOT TO THE DRAW LIST TO BE DRAWN!
    matchInitialPoints( bottle, bottleMorphTarget );
    makeModelsSameSize( bottle, bottleMorphTarget );
    bottle->buffer();
    bottle->propagate();
  }
  
  // Let the bodies hit the floor
  floor = rootScene->addObject( "floor", shader[1] );
  quad( floor, vec4( -10, 0, 10, 1.0 ), vec4( -10, 0, -10, 1.0 ),
        vec4( 10, 0, -10, 1.0 ), vec4( 10, 0, 10, 1.0 ),
        vec4( 0.4, 0.4, 0.4, 0.9 ) );
  floor->buffer();
  
  // Load up that goddamned candle
  candle = rootScene->addObject( "Candle", shader[1] );
  ObjLoader::loadModelFromFile( candle, "../models/candle.obj" );
  ObjLoader::loadMaterialFromFile( candle, "../models/candle.mtl" );
  vec4 min = candle->getMin();
  vec4 max = candle->getMax();
  fprintf( stderr, "Min: (%f,%f,%f)\nMax: (%f,%f,%f)\n", min.x, min.y, min.z,
           max.x, max.y, max.z );
  candle->_trans._offset.set( 2.5, -min.y, 2.5 );
  //candle->propagate();
  candle->buffer();
  
  ps = new ParticleSystem( 10000, "ps1", shader[2] );
  ps->setLifespan( 5, 7.5 );
  ps->setEmitterRadius( 0.001 );
  candle->insertObject( ps );
  ps->_trans._offset.set( 0, min.y + (max.y - min.y), 0 );
  ps->fillSystemWithParticles();
  //ps->propagate();
  ps->buffer();
  
  candle->propagate();
  
  // Generic OpenGL setup: Enable the depth _buffer and set a nice background color.
  glEnable( GL_DEPTH_TEST );
  glClearColor( 0.0, 0.0, 0.0, 1.0 );
  
  printf( "LOOPING!\n" );
  glutMainLoop();
}
/**
 * A simple animation callback.
 * Rotates the object about its Y axis,
 * as it orbits the object around a point about the Y axis.
 * @param obj The object to animate.
 */
void MONOLITH::simpleRotateAnim( TransCache &obj ) {
  obj._rotation.rotateY( tick.scale() * 1.5 );
  //obj._offset.set( 1.5, 0, 0 );
  obj._orbit.rotateY( tick.scale() * -0.5 );
}
