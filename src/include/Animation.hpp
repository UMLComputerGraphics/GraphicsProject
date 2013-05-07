#ifndef __ANIMATION_HPP
#define __ANIMATION_HPP

#include "vec.hpp"
#include "TransCache.hpp"
#include "Transformation.hpp"
#include "Object.hpp"

namespace Animation {

  void seekTopTo( Object *obj, int y = 0 );
  void seekBottomTo( Object *obj, int y = 0 );

}

#endif
