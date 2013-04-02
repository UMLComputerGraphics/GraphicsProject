/**
 * @file TransCache.cpp
 * @author John Huston
 * @date 2013-03-28
 * @brief TransCache Implementation
 * @details Transformation Cache implementation; This class
 * serves as a wrapper for containing all the necessary
 * transformations for the Scene graph.
 */

#include "TransCache.hpp"
#include "Transformation.hpp"
#include "mat.hpp"

void TransCache::ptm( const Angel::mat4 &new_ptm, bool postmult ) {
  
  /* Update our cached ptm. */
  this->_ptm = new_ptm;
  
  /* Update our Result Matrix. */
  if ( postmult ) _otm = _ptm * _ctm;
  else _otm = _ctm * _ptm;
  
}

void TransCache::calcCTM( bool postmult ) {
  
  if ( postmult ) {
    /* Recompute our ctm */
    _ctm = _displacement * _orbit * _offset * _rotation * _scale * _preRotation;
    /* Recompute our Cached Result Transformation Matrix */
    _otm = _ptm * _ctm;
  } else {
    /* Recompute our ctm */
    _ctm = _preRotation * _scale * _rotation * _offset * _orbit * _displacement;
    /* Recompute our Cached Result Transformation Matrix */
    _otm = _ctm * _ptm;
  }
}

const Angel::mat4 &TransCache::ptm( void ) const {
  return _ptm;
}
const Angel::mat4 &TransCache::ctm( void ) const {
  return _ctm;
}
const Angel::mat4 &TransCache::otm( void ) const {
  return _otm;
}

//
// TransCache v2.0
//

TransCache::TransCache( void ) {

  dirty( false );
  _premult = false;
  _cascade = false;

}

/**
 * Add a new transformation to the end.
 * @param newTrans
 */
void TransCache::push( Transformation *newTrans ) {

  //TODO: If mat on stack is same type as new mat, combine them.

  newTrans->markNew();
  _transformations.push_back( newTrans );

  // We need to update, but only additional multiplications!
  _new = true;

  // If this transformation is inheritable, mark this node as
  // needing to propagate to children, as well.
  if ( newTrans->inheritable() ) _cascade = true;

}

/**
 * Remove a transformation from the end.
 */
void TransCache::pop( void ) {

  Transformation *back = _transformations.back();
  if ( back->inheritable() ) _cascade = true;

  dirty( true );

  // TODO: Optimization: rollback the CTM,ITM,OTM by
  // using the Inverse() of the popped matrix.
  // Can add new _removed flag, perhaps.
  _transformations.pop_back();

}

/**
 * Delete all transformations.
 */
void TransCache::clear( void ) {

  // Clear our transformations.
  _ctm = _itm = Angel::mat4();
  _transformations.clear();

  // Set our transformation to whatever our parent's is.
  // To the knowledge of this node, our parent cache is
  // never wrong, so we should be able to use it immediately.
  _otm = _ptm;

  // Our node is clean, but we need to propagate to children.
  dirty( false );
  _cascade = true;

}

/**
 * Recompute entirely all transformational caches.
 */
void TransCache::rebuild( void ) {
  std::deque< Transformation* >::reverse_iterator rit;
  // Clear out our [I]nheritable [T]ransformation [M]atrix,
  // And our [C]urrent [T]ransformation [M]atrix.
  _itm = _ctm = Angel::mat4();

  if (_premult) {
    for ( rit = _transformations.rbegin();
        rit != _transformations.rend();
        ++rit ) {
      _ctm = _ctm * (**rit);
      if ((*rit)->inheritable()) _itm = _itm * (**rit);
    }
  } else {
    for ( rit = _transformations.rbegin(); rit != _transformations.rend();
        ++rit ) {
      _ctm = (**rit) * _ctm;
      if ( (*rit)->inheritable() ) _itm = (**rit) * _itm;
    }
  }

  // Recompute our [O]bject [T]ransformation [M]atrix,
  // The result of our parent's and our own transformations combined.
  _otm = _ctm * _ptm;

  dirty( false );
  _cascade = true;

}

/**
 * Attempt to intelligently re-compute cache.
 */
void TransCache::clean( void ) {

  // TODO: Stub.
  dirty( false );

}

void TransCache::adopt( const Angel::mat4 &ptm_in ) {

  //TODO: Stub.
  _ptm = ptm_in;
  _parent = true;

}



/**
 * Do we need to invoke clean()?
 */
bool TransCache::dirty( void ) {
  return (_new || _rebuild || _parent);
}

void TransCache::dirty( bool newState ) {
  if (newState == false) {
    _new = _rebuild = _parent = false;
  } else {
    // Using the dirty method is a blunt object.
    _new = _parent = false;
    _rebuild = true;
  }
}
