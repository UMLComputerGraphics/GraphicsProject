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
#include <stdexcept>

void TransCache::ptmOLD( const Angel::mat4 &ptm_in, bool postmult ) {

  // Update our cache of our Parent's Matrix
  _ptm = ptm_in;

  // Mark that we have a new Parent matrix,
  // And we need to update our children, too.
  _parent = true;
  _cascade = true;
  
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

//
// TransCache v2.0
//

TransCache::TransCache( bool createInvertedSibling, bool isInverted ) {

  dirty( false );
  _premult = false;
  _cascade = false;

  _inverted = isInverted;
  if ( (_invert = createInvertedSibling) )
    _invertedCache = new TransCache(  false, true );
  else _invertedCache = NULL;

}

TransCache::~TransCache( void ) {

  TransformationsType::iterator it;
  for ( it = _transformations.begin(); it != _transformations.end(); ++it ) {
    delete (*it);
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
const Angel::mat4 &TransCache::itm( void ) const {
  return _itm;
}
Angel::mat4 TransCache::pitm( void ) const {
  if (_premult)
    return ptm() * itm();
  else
    return itm() * ptm();
}

void TransCache::ptm( const Angel::mat4 &ptm_in ) {

  // Update our cache of our Parent's Matrix
  _ptm = ptm_in;

  // Mark that we have a new Parent matrix,
  // And we need to update our children, too.
  _parent = true;
  _cascade = true;

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

void TransCache::push( const Transformation &newTrans ) {

  this->push( newTrans.newCopy() );

}

void TransCache::insert( unsigned index, const Transformation & newTrans ) {

	dirty( true );

	Transformation *copiedNew = newTrans.newCopy();
	TransformationsType::iterator it = queryImpl( index, newTrans.type() );
	// The ordering is [new][old], so we collapse the transformation into [new].
	copiedNew->coalesce( *it );
	// We unfortunately need to copy [new] back over [old]
	**it = *copiedNew;
	// Now delete the new node, we're done with it!
	delete copiedNew;

}

void TransCache::append( unsigned index, const Transformation & newTrans ){

	dirty(true);

	Transformation *copiedNew = newTrans.newCopy();
	TransformationsType::iterator it = queryImpl( index, newTrans.type() );
	(*it)->coalesce( copiedNew );
	// Copied-New is now Identity and can be deleted.
	delete copiedNew;

}

TransCache::TransformationsType::iterator TransCache::queryImpl( unsigned index, Transformation::Subtype type ) {
  return _transformations.begin();
}

const Transformation &TransCache::query( unsigned index, Transformation::Subtype type ){
	return RotMat();
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
  TransformationsType::reverse_iterator rit;
  // Clear out our [I]nheritable [T]ransformation [M]atrix,
  // And our [C]urrent [T]ransformation [M]atrix.
  _itm = _ctm = Angel::mat4();

  for ( rit = _transformations.rbegin(); rit != _transformations.rend();
      ++rit ) {
    if ( _premult ) {
      _ctm = _ctm * (**rit);
      if ( (*rit)->inheritable() ) _itm = _itm * (**rit);
    } else {
      _ctm = (**rit) * _ctm;
      if ( (*rit)->inheritable() ) _itm = (**rit) * _itm;
    }
  }

  // Recompute our [O]bject [T]ransformation [M]atrix,
  // The result of our parent's and our own transformations combined.
  _otm = _ctm * _ptm;

  // Mark our node as clean.
  dirty( false );

  // Mark that we need to send updates to our children. (Using the _itm!)
  _cascade = true;
}

/**
 * Attempt to intelligently re-compute cache.
 */
void TransCache::clean( void ) {

  if ( _rebuild ) {
    rebuild();
  }

  if ( _new ) {
    Angel::mat4 lhs, rhs, ilhs, irhs;
    TransformationsType::iterator it;

    // Run a quick scan to see if any of the new matrices are inheritable.
    // Mark this node as needing to cascade to children if so.
    for ( it = _transformations.begin(); it != _transformations.end(); ++it ) {
      if ( (*it)->inheritable() ) _cascade = true;
    }

    // If the transformation we want is A->B->C->D->E->F
    // We will have pushed in that order,
    // So our stack will look like A-B-C-D-E-F.
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

    // Calculate new transformations on one end of the stack.
    // We're going to see new transformations that should be applied before others.
    // So we'll be seeing A-B-C ... [Old Transformations] ... X-Y-Z.
    for ( it = _transformations.begin(); it != _transformations.end(); ++it ) {

      // If we find non-new transformations, ignore them and skip to the next step.
      if ( !(*it)->isNew() ) break;

      if ( _premult ) {
        lhs = lhs * (**it);
        if ( (*it)->inheritable() ) ilhs = ilhs * (**it);
        //lhs = I * A
        //lhs = A * B
        //lhs = (A*B) * C
        //lhs gets (ABC).
      } else {
        rhs = (**it) * rhs;
        if ( (*it)->inheritable() ) irhs = (**it) * irhs;
        //rhs = A * I;
        //rhs = B * A
        //rhs = C * (B*A)
        //rhs gets (CBA)
      }

      (*it)->markOld();

    }

    // Fast forward through "Old" transformations.
    for ( ; it != _transformations.end(); ++it ) {
      if ( (*it)->isNew() ) break;
    }

    // Compute new post-transformations.
    // I.e, for A-B-C-...[OLD]...-X-Y-Z, we'll be looking at X-Y-Z in that order.
    for ( ; it != _transformations.end(); ++it ) {
      if ( !(*it)->isNew() )
        throw std::logic_error(
            "TransCache SceneGraph error: Non-new post transformations found.\n"
            "What? It means that the TransCache::clean() function is broken,\n"
            "And you should blame jhuston@cs.uml.edu." );
      if ( _premult ) {
        rhs = rhs * (**it);
        if ( (*it)->inheritable() ) irhs = irhs * (**it);
      } else {
        lhs = (**it) * lhs;
        if ( (*it)->inheritable() ) ilhs = (**it) * ilhs;
      }

      (*it)->markOld();

    }

    // Recompute our transformation matrix component.
    _ctm = lhs * _ctm * rhs;
    // Recompute our inheritable transformation matrix component.
    _itm = ilhs * _itm * irhs;

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

  // Alright, our node state is self-consistent now.
  dirty( false );

  // Let's condense our matrices if we can, though.
  condense();

}

void TransCache::condense( void ) {

  bool modified = false;
  // A, B, C, D, E, F
  // In a LTR scheme, we condense as AB, CD, EF...
  // In a RTL scheme, we should condense as FE, DC, BA.

  TransformationsType::iterator it;
  for (it = _transformations.begin(); (it != _transformations.end())
  && ((it + 1) != _transformations.end()); ) {
    Transformation *lhs = *(it);
    Transformation *rhs = *(it + 1);
    if (lhs->type() == rhs->type()) {
      modified = true;
      if (_premult) {
        lhs->coalesce(rhs);
        _transformations.erase( it + 1 );
        delete rhs;
      } else {
        rhs->coalesce(lhs);
        _transformations.erase( it );
        delete lhs;
        // Do not advance iterator:
        // Deleting the current iterator advances for us.
        continue;
      }
    }
    ++it;
  }

  if (modified && dirty()) _rebuild = true;

}

/**
 * Do we need to invoke clean()?
 */
bool TransCache::dirty( void ) const {
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

bool TransCache::cascade( void ) const {
  return _cascade;
}

void TransCache::cascade( bool newState ) {
  _cascade = newState;
}

unsigned TransCache::size(void) const {
  return _transformations.size();
}

void TransCache::debug( void ) const {

  std::cerr << "premult: " << _premult << "; new: " << _new <<
    "; rebuild: " << _rebuild << "; cascade: " << _cascade << 
    "; parent: " << _parent << "; size: " << size() << ";";

}

void TransCache::debugMat( void ) const { 

  //pcoi
  for ( size_t i = 0; i < 4; ++i ) {
    std::cerr << ptm()[i] << " " << ctm()[i] << " " <<
      otm()[i] << " " << itm()[i] << "\n";
  }

}
	   
