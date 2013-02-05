#include "TransCache.hpp"
#include "Transformation.hpp"
#include "mat.hpp"

void TransCache::PTM( const Angel::mat4 &new_ptm,
		      bool postmult ) {

  /* Update our cached PTM. */
  this->ptm = new_ptm;

  /* Update our Result Matrix. */
  if (postmult) otm = ptm * ctm;
  else otm = ctm * ptm;

}

void TransCache::CalcCTM( bool postmult ) {

  if (postmult) {
    /* Recompute our CTM */
    ctm = displacement * orbit * offset * rotation * scale * PreRotation;
    /* Recompute our Cached Result Transformation Matrix */
    otm = ptm * ctm;
  } else {
    /* Recompute our CTM */
    ctm = PreRotation * scale * rotation * offset * orbit * displacement;
    /* Recompute our Cached Result Transformation Matrix */
    otm = ctm * ptm;
  }
}

const Angel::mat4 &TransCache::PTM( void ) const { return ptm; }
const Angel::mat4 &TransCache::CTM( void ) const { return ctm; }
const Angel::mat4 &TransCache::OTM( void ) const { return otm; }
