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
  
  void ptmOLD( const Angel::mat4 &ptm_in, bool postmult = true );

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

  typedef std::deque< Transformation* > TransformationDeque;

  TransCache( void );

  const Angel::mat4 &ptm( void ) const;  // Retrieve current parent transformation
  const Angel::mat4 &ctm( void ) const;  // Retrieve current isolated object transformation
  const Angel::mat4 &otm( void ) const;  // Retrieve current cumulative object transformation
  const Angel::mat4 &itm( void ) const;  // Retrieve current inheritable object transformations

  void ptm( const Angel::mat4 &ptm_in ); // Set new Parent Transform.

  void push( Transformation *newTrans ); // Add New Transformation
  void pop( void );                      // Remove Transformation
  void clear( void );                    // Clear all Transformations
  void rebuild( void );                  // Recalculate Cache
  void clean( void );                    // Smartly Update Cache

  bool dirty( void );
  void dirty( bool newState );
  bool cascade( void );
  void cascade( bool newState );

private:
  
  // Cached Result Matrices
  Angel::mat4 _ptm; /* Parent's Cumulative Transformation Matrix */
  Angel::mat4 _ctm; /* Current Transformation Matrix */
  Angel::mat4 _itm; // Inheritable Trans Mat: CTM, minus transformations we don't want our kids to have.
  Angel::mat4 _otm; /* Cached Result Transformation Matrix: e.g; ctm * ptmOLD */
  TransformationDeque _transformations; // Transformation Stack
  
  bool _premult; // Should we premult instead of postmult?
  bool _new;     // CTM needs new additions flag
  bool _rebuild; // Cache needs to be rebuilt, no optimizations.
  bool _cascade; // Children need update flag
  bool _parent;  // New Parent Matrix.

};
