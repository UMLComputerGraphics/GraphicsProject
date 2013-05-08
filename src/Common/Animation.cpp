#include "Animation.hpp"
#include "Object.hpp"
#include "vec.hpp"
#include "Transformation.hpp"
#include "TransCache.hpp"
#include "Timer.hpp"

void Animation::seekTopTo( Object *obj, float y ) {

  TransMat initialPlacement;
  initialPlacement.inheritable( false );
  initialPlacement.set( 0, y - obj->getMax().y, 0 );
  obj->_trans.push( initialPlacement );

}

void Animation::seekBottomTo( Object *obj, float y ) {

  TransMat initialPlacement;
  initialPlacement.inheritable( false );
  initialPlacement.set( 0, y - obj->getMin().y, 0 );
  obj->_trans.push( initialPlacement );

}

void Animation::seekCenterTo( Object *obj, float x, float z, bool inherit ) {

  TransMat seek;
  seek.inheritable( inherit );
  vec3 min = obj->getMin();
  vec3 max = obj->getMax();

  float xSeek = (max.x + min.x) / 2.0;
  float zSeek = (max.z + min.z) / 2.0;
  seek.set( x - xSeek, 0, z - zSeek );
  obj->_trans.push( seek );

}


float Animation::scaleBottomFixed( Object *obj, float scaleAmt ) {

  // This is where the minimum coordinates are, right now ...
  vec4 min = obj->_trans.otm() * vec4( obj->getMin(), 1.0 );
  vec4 max = obj->_trans.otm() * vec4( obj->getMax(), 1.0 );

  float height = max.y - min.y;
  float yAdjustment = min.y - (scaleAmt * min.y);

  ScaleMat shrink( 1, scaleAmt, 1 );
  shrink.inheritable( false );
  TransMat adjustment;
  adjustment.set( 0, yAdjustment, 0 );
  adjustment.inheritable( false );

  obj->_trans.push( shrink );
  obj->_trans.push( adjustment );

  return -((1 - scaleAmt) * height);

}

void Animation::candleMelt( Object *candle, Object *tip, float rawScale ) {
  
  float adj = Animation::scaleBottomFixed( candle, pow(rawScale, tick.scale()) );
  vec4 currentTipPos = tip->_trans.otm() * vec4(tip->getMax(),1.0);
  tip->_trans.push( TransMat( 0, adj, 0 ) );

  currentTipPos.y += adj;

  if (*(Engine::instance()->getNumLights()) > 0) {
    Engine::instance()->getLights()->at(0)->y( currentTipPos.y );
    Engine::instance()->setLights();
  }

}
