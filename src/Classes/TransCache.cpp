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

  if ( _premult ) {
    for ( rit = _transformations.rbegin(); rit != _transformations.rend();
        ++rit ) {
      _ctm = _ctm * (**rit);
      if ( (*rit)->inheritable() ) _itm = _itm * (**rit);
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

  if ( _rebuild ) return rebuild();

  if ( _new ) {
    Angel::mat4 lhs, rhs;
    TransformationDeque::iterator it;

    // If the transformation we want is A->B->C->D->E->F
    // We will have pushed in that order,
    // So our stack will look like A-B-C-D-E.
    // Let's assume that our new matrices are E-F, and A-B.
    // So we have the existing product:
    //
    // _ctm = D*C
    // post = F*E
    // pre = B*A
    // _ctm = post * _ctm * pre
    //
    // or
    //
    // _ctm = C*D
    // pre = A*B
    // post = E*F
    // _ctm = pre * _ctm * post

    // lhs = (F*E) or (A*B)
    // rhs = (B*A) or (E*F)
    // _ctm = lhs * _ctm * rhs;


    for ( it = _transformations.begin(); it != _transformations.end(); ++it ) {
      if ((*it)->isNew()) {
        if ( _premult ) {
        }
        else {
        }
      }
      else break;
    }
    // TODO: stub ...

    // Get the lower block to compute
    // our new _otm for us by pretending
    // that we received a new parent.
    _parent = true;
  }

  if ( _parent ) {
    // If we received a new parent matrix,
    // Recompute our OTM.
    if ( _premult ) _otm = _ctm * _ptm;
    else _otm = _ptm * _ctm;
    _parent = false;
  }
  dirty( false );

}

void TransCache::adopt( const Angel::mat4 &ptm_in ) {

  // Update our cache of our Parent's Matrix
  _ptm = ptm_in;

  // Mark that we have a new Parent matrix,
  // And we need to update our children, too.
  _parent = true;
  _cascade = true;

}

/**
 * Do we need to invoke clean()?
 */
bool TransCache::dirty( void ) {
  return (_new || _rebuild || _parent);
}

void TransCache::dirty( bool newState ) {
  if ( newState == false ) {
    _new = _rebuild = _parent = false;
  } else {
    // Using the dirty method is a blunt object.
    _new = _parent = false;
    _rebuild = true;
  }
}
