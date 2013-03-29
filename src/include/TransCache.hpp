/**
 * @file TransCache.hpp
 * @date 2013-03-29
 * @author John Huston
 * @brief Header for the "Transformation Cache" class.
 * @details Encompasses all of the necessary details
 * to apply a variety of common transformations to a single object
 * in a scene graph.
 */

#include "mat.hpp"
#include "Transformation.hpp"

class TransCache {
  
public:
  
  void ptm( const Angel::mat4 &ptm_in, bool postmult = true );

  const Angel::mat4 &ptm( void ) const;
  const Angel::mat4 &ctm( void ) const;
  const Angel::mat4 &otm( void ) const;

  // Hacky: For Camera.
  TransMat _preOffset;
  RotMat _preRotation;

  // Component Transformations //
  ScaleMat _scale; /* Scale Matrix */
  RotMat _rotation; /* Rotation-in-place Matrix */
  TransMat _offset; /* Radius, or _offset from origin. */
  RotMat _orbit; /* Rotation about the origin */
  TransMat _displacement; /* Offset of entire animation from origin. */
  
  /* Updates our ctm. Private Use. */
  void calcCTM( bool postmult = true );

private:
  
  // Cached Result Matrices
  Angel::mat4 _ptm; /* Parent's Cumulative Transformation Matrix */
  Angel::mat4 _ctm; /* Current Transformation Matrix */
  Angel::mat4 _otm; /* Cached Result Transformation Matrix: e.g; ctm * ptm */
  
};
