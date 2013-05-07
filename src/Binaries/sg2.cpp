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
#include "Animation.hpp"

void sgIdle( void ) {
  Scene *s = Engine::instance()->rootScene();

  // Candle Melting Animation
  Object *candle = s->search( "candle" );
  Object *candletip = s->search( "candletip" );
  Animation::candleMelt( candle, candletip, 0.999 );

}

void act_one( void ) {

  Scene *rootScene = Engine::instance()->rootScene();

#ifdef __APPLE__
  GLint macShader = Angel::InitShader( "shaders/vEngineNoMorph.glsl",
				       "shaders/fEngine.glsl" );
  // This call makes the default shader macShader for all newly created objects.
  // You can always override it in addObject( "name", shader ) though.
  // Note that PCs do not need anything beyond the Engine inherent default shader.
  rootScene->shader( macShader );
#endif

  // Table
  Object *table = rootScene->addObject( "table" );
  ObjLoader::loadModelFromFile(table, "../models/table_tx.obj");
  ObjLoader::loadMaterialFromFile(table, "../models/table_tx.mtl");
  table->texture( "../Textures/texture_wood.png" );
  Animation::seekTopTo( table, 0 );
  table->buffer();

  // Bottle
  Object *bottle = table->addObject( "bottle" );
  ObjLoader::loadModelFromFile( bottle, "../models/bottle_wine_high.obj" );
  ObjLoader::loadMaterialFromFile( bottle, "../models/bottle_wine_high.mtl" );
  Animation::seekBottomTo( bottle, 0 );
  bottle->buffer();

  // Candlestick
  Object *candlestick = table->addObject( "candlestick" );
  ObjLoader::loadModelFromFile(candlestick, "../models/candlestick.obj");
  ObjLoader::loadMaterialFromFile(candlestick, "../models/candlestick.mtl");
  // Note that, unlike earlier adjustment, we DO want this translation to propagate.
  TransMat initialPlacement;
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
  
  Engine::init( &argc, argv, "wow!" );
  act_one();
  glClearColor( 0.1, 0.3, 0.5, 1.0 );
  Engine::instance()->registerIdle( sgIdle );
  Engine::run();

}
