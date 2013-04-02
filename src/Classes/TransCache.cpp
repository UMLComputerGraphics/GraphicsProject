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

void TransCache::updateCache( void ) {
  std::deque<Transformation>::reverse_iterator rit;
  // Clear out our [I]nheritable [T]ransformation [M]atrix,
  // And our [C]urrent [T]ransformation [M]atrix.
  _itm = _ctm = Angel::mat4();

  for (rit = _transformations.rbegin(); rit != _transformations.rend(); ++rit ) {
    _ctm = (*rit) * _ctm;
    if (rit->inheritable()) _itm = (*rit) * _itm;
  }

  // Recompute our [O]bject [T]ransformation [M]atrix,
  // The result of our parent's and our own transformations combined.
  _otm = _ctm * _ptm;

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
