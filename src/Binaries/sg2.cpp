/**
 * @file sg2.cpp
 * @author John Huston
 * @date 2013-04-23
 * @brief Scene Graph Demo
 * @details Showcases a new Scene Graph API,
 * as well as demonstrates a new reduced line-count for
 * a minimalistic Engine API.
 */

#include <cstdio>
#include <unistd.h>
#include <cstdlib>
#include "Engine.hpp"

void simpleRotate( TransCache &obj ) {
  
  RotMat newrot;
  newrot.rotateY( tick.scale() * 1.5 );
  newrot.rotateX( tick.scale() * 1.4 );
  newrot.rotateZ( tick.scale() * 1.3 );
  obj.push( newrot );
  
}

void sgIdle( void ) {
  //Scene &s = *(Engine::instance()->rootScene());
  //Object &cube = *(s["cube"]);
  //cube.animation( simpleRotate );
}

void act_one( void ) {

  vec4 min, max;
  TransMat initialPlacement;
  Scene *rootScene = Engine::instance()->rootScene();

  initialPlacement.inheritable( false );

  // Table
  Object *table = rootScene->addObject( "table" );
  ObjLoader::loadModelFromFile(table, "../models/table.obj");
  ObjLoader::loadMaterialFromFile(table, "../models/table.mtl");
  max = table->getMax();
  initialPlacement.set( 0, -max.y, 0 );
  table->_trans.push( initialPlacement );
  table->buffer();
  fprintf( stderr, "Moving table y:(%f)\n", -max.y );

  // Bottle
  Object *bottle = table->addObject( "bottle" );
  ObjLoader::loadModelFromFile( bottle, "../models/bottle_wine_high.obj" );
  ObjLoader::loadMaterialFromFile( bottle, "../models/bottle_wine_high.mtl" );
  // Non-propagating bottle-adjustment.
  min = bottle->getMin();
  initialPlacement.set( 0, -min.y, 0 );
  bottle->_trans.push( initialPlacement );
  bottle->buffer();
  fprintf( stderr, "Moving bottle y:(%f)\n", -min.y );

  // Candlestick
  Object *candlestick = table->addObject( "candlestick" );
  ObjLoader::loadModelFromFile(candlestick, "../models/candlestick.obj");
  ObjLoader::loadMaterialFromFile(candlestick, "../models/candlestick.mtl");
  // Note that, unlike earlier adjustment, we DO want this translation to propagate.
  initialPlacement.inheritable( true );
  initialPlacement.set( 5.0, 0, 5.0 );
  candlestick->_trans.push( initialPlacement );
  candlestick->buffer();

  // Candle
  Object *candle = candlestick->addObject( "candle" );
  ObjLoader::loadModelFromFile( candle, "../models/candle_bottom.obj");
  ObjLoader::loadMaterialFromFile( candle, "../models/candle.mtl" );
  candle->buffer();

  // Candletip
  Object *candletip = candle->addObject( "candletip" );
  ObjLoader::loadModelFromFile( candletip, "../models/candle_top_unmelted.obj" );
  ObjLoader::loadMaterialFromFile( candletip, "../models/candle.mtl" );
  candletip->buffer();

}

int main( int argc, char *argv[] ) {
  
  Engine::instance()->init( &argc, argv, "wow!" );
  act_one();  
  Engine::instance()->registerIdle( sgIdle );

  glutMainLoop();
  return EXIT_SUCCESS;
}
