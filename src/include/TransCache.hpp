/**
 * @file TransCache.hpp
 * @date 2013-03-29
 * @author John Huston
 * @brief Header for the "Transformation Cache" class.
 * @details Encompasses all of the necessary details
 * to apply a variety of common transformations to a single object
 * in a scene graph.
 */


#ifndef __TRANSCACHE_HPP
#define __TRANSCACHE_HPP

#include "mat.hpp"
#include "Transformation.hpp"
#include <list>
#include <deque>
#include <vector>

class TransCache {
  
public:

  /** Scene Graph 1.0: These properties and methods are deprecated. **/
  
  void ptmOLD( const Angel::mat4 &ptm_in, bool postmult = true ) __attribute__ ((deprecated));

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

  /** End Scene Graph 1.0 **/

  // Scene Graph V2.0 //

  typedef std::vector< Transformation* > TransformationsType;

  TransCache( bool _invert = false, bool _inverted = false );
  virtual ~TransCache( void );

  const Angel::mat4 &ptm( void ) const;  // Retrieve current parent transformation
  const Angel::mat4 &ctm( void ) const;  // Retrieve current isolated object transformation
  const Angel::mat4 &otm( void ) const;  // Retrieve current cumulative object transformation
  const Angel::mat4 &itm( void ) const;  // Retrieve current inheritable object transformations
  Angel::mat4 pitm( void ) const;  // Retrieve the ptm and itm combined.

  void ptm( const Angel::mat4 &ptm_in ); // Set new Parent Transform.

  bool dirty( void ) const;
  void dirty( bool newState );
  bool cascade( void ) const;
  void cascade( bool newState );
  unsigned size( void ) const;
  void premult( void ) const;

  void debug( void ) const;
  void debugMat( void ) const;

  // Interface:
  void push( const Transformation &newTrans );
  void insert( unsigned index, const Transformation & newTrans );
  void append( unsigned index, const Transformation & newTrans );
  const Transformation &query( unsigned index, Transformation::Subtype type );

  // Maintenance/management:
  void pop( void );                      // Remove Transformation
  void clear( void );                    // Clear all Transformations
  void rebuild( void );                  // Recalculate Cache
  void clean( void );                    // Smartly Update Cache
  void condense( void );                 // Find adjoining transformations
                                         // Of the same type, and join them
private:

  // Pointer operations: Dangerous!
  TransformationsType::iterator queryImpl( unsigned index, Transformation::Subtype type );
  void push( Transformation *newTrans ); // Add New Transformation
  
  // Cached Result Matrices
  Angel::mat4 _ptm; /* Parent's Cumulative Transformation Matrix */
  Angel::mat4 _ctm; /* Current Transformation Matrix */
  Angel::mat4 _itm; // Inheritable Trans Mat: CTM, minus transformations we don't want our kids to have.
  Angel::mat4 _otm; /* Cached Result Transformation Matrix: e.g; ctm * ptmOLD */
  TransformationsType _transformations; // Transformation Stack
  
  bool _premult; // Should we premult instead of postmult?
  bool _new;     // CTM needs new additions flag
  bool _rebuild; // Cache needs to be rebuilt, no optimizations.
  bool _cascade; // Children need update flag
  bool _parent;  // New Parent Matrix.

// ;)
  bool _inverted; // Am I an inverted cache?
  bool _invert;   // Am I responsible for keeping an inverted cache?
  TransCache *_invertedCache; // Pointer to inverted cache if applicable.

};

#endif
