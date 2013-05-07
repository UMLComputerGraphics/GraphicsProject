#include "Animation.hpp"
#include "Object.hpp"
#include "vec.hpp"
#include "Transformation.hpp"
#include "TransCache.hpp"

void Animation::seekTopTo( Object *obj, int y ) {

  TransMat initialPlacement;
  initialPlacement.inheritable( false );
  initialPlacement.set( 0, y - obj->getMax().y, 0 );
  obj->_trans.push( initialPlacement );

}


void Animation::seekBottomTo( Object *obj, int y ) {

  TransMat initialPlacement;
  initialPlacement.inheritable( false );
  initialPlacement.set( 0, y - obj->getMin().y, 0 );
  obj->_trans.push( initialPlacement );

}
