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

void candleMelt( TransCache &obj ) {

  ScaleMat melt;
  melt.inheritable( false );
  melt.adjust( 1.0, pow(0.999, tick.scale()), 1.0 );
  obj.push( melt );

}

void sgIdle( void ) {
  //Scene *s = Engine::instance()->rootScene();
  //Object *candle = s->search( "candle" );
  //candle->animation( candleMelt );
}

void act_one( void ) {

  vec4 min, max;
  TransMat initialPlacement;
  Scene *rootScene = Engine::instance()->rootScene();

#ifdef __APPLE__
  GLint macShader = Angel::InitShader( "shaders/vEngineNoMorph.glsl",
				       "shaders/fEngine.glsl" );
  // This call makes the default shader macShader for all newly created objects.
  // You can always override it in addObject( "name", shader ) though.
  // Note that PCs do not need anything beyond the Engine inherent default shader.
  rootScene->shader( macShader );
#endif

  initialPlacement.inheritable( false );

  // Table
  Object *table = rootScene->addObject( "table" );
  ObjLoader::loadModelFromFile(table, "../models/table.obj");
  ObjLoader::loadMaterialFromFile(table, "../models/table.mtl");
  table->texture( "../Textures/texture_wood.png" );
  max = table->getMax();
  initialPlacement.set( 0, -max.y, 0 );
  table->_trans.push( initialPlacement );
  table->buffer();

  // Bottle
  Object *bottle = table->addObject( "bottle" );
  ObjLoader::loadModelFromFile( bottle, "../models/bottle_wine_high.obj" );
  ObjLoader::loadMaterialFromFile( bottle, "../models/bottle_wine_high.mtl" );
  // Non-propagating bottle-adjustment.
  min = bottle->getMin();
  initialPlacement.set( 0, -min.y, 0 );
  bottle->_trans.push( initialPlacement );
  bottle->buffer();

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
  
  Engine::init( &argc, argv, "wow!" );
  act_one();  
  Engine::instance()->registerIdle( sgIdle );
  Engine::run();

}
