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
#include <deque>

class TransCache {

public:

  TransCache( void );

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

  // Scene Graph V2.0 //
  // Everything below here will eventually replace everything above.

  void push( Transformation *newTrans ); // Add New Transformation
  void pop( void );                      // Remove Transformation
  void clear( void );                    // Clear all Transformations
  void rebuild( void );                  // Recalculate Cache
  void clean( void );                    // Smartly Update Cache
  void adopt( const Angel::mat4 &ptm_in ); // Set new Parent Transform.

  bool dirty( void );
  void dirty( bool newState );
  bool cascade( void );
  void cascade( bool newState );

private:

  // Cached Result Matrices
  Angel::mat4 _ptm; /* Parent's Cumulative Transformation Matrix */
  Angel::mat4 _ctm; /* Current Transformation Matrix */
  Angel::mat4 _itm; // Inheritable Trans Mat: CTM, minus transformations we don't want our kids to have.
  Angel::mat4 _otm; /* Cached Result Transformation Matrix: e.g; ctm * ptm */
  std::deque< Transformation* > _transformations; // Transformation Stack

  bool _premult; // Should we premult instead of postmult?
  bool _new;     // CTM needs new additions flag
  bool _rebuild; // Cache needs to be rebuilt, no optimizations.
  bool _cascade; // Children need update flag
  bool _parent;  // New Parent Matrix.

};
