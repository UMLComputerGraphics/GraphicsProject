/**
 * @file Transformation.hpp
 * @author John Huston
 * @date 2013-03-29
 * @brief Headers for Transformation, RotMat, TransMat and ScaleMat.
 * @details Headers for the Transformation superclass, and Rotation,
 * Translation, and Scale specialization classes.
 */

#ifndef __TRANSMAT_HPP
#define __TRANSMAT_HPP

#include "mat.hpp"
#include "vec.hpp"
#include "platform.h" //OpenGL types.

class Transformation {
  
public:

  typedef enum Subtype {
    GENERIC,
    ROTATION,
    TRANSLATION,
    SCALE
  } Subtype;

  /*
   These are all default and are not currently needed.
   Transformation( const Transformation &copy );
   Transformation &operator=( const Transformation &assignment );
   */
  Transformation( void );
  virtual ~Transformation( void );

  const Angel::mat4 &matrix( void ) const;
  Angel::mat4 operator*( const Angel::mat4 &rhs ) const;
  Angel::mat4 operator*( const Transformation &rhs ) const;

  void combine( Transformation *rhs );
  void reset( void );

  bool inheritable( void ) const;
  void inheritable( bool in );
  void markNew( void );
  void markOld( void );
  bool isNew( void );

  virtual Angel::mat4 inverse( void ) const = 0;
  virtual Transformation::Subtype type( void ) const = 0;
  virtual void coalesce( Transformation *rhs ) = 0;
  virtual Transformation *newCopy( void ) const = 0;

protected:
  bool _inheritable;
  bool _new;
  Angel::mat4 _mat;
  
};

Angel::mat4 operator*( const Angel::mat4 &lhs, const Transformation &rhs );

/** Rotations **/

class RotMat : public Transformation {
  
public:
  
  RotMat( float rx = 0.0, float ry = 0.0, float rz = 0.0 );
  const RotMat &reset( const Angel::mat4 &NewState );
  const RotMat &rotateX( const GLfloat theta, bool postmult = true );
  const RotMat &rotateY( const GLfloat theta, bool postmult = true );
  const RotMat &rotateZ( const GLfloat theta, bool postmult = true );
  const RotMat &adjust( const Angel::mat4 &Adjustment, bool postmult = true );

  virtual Angel::mat4 inverse( void ) const;
  virtual Transformation::Subtype type( void ) const;
  virtual void coalesce( Transformation *rhs );
  virtual Transformation *newCopy( void ) const;

};

/** Translations **/

class TransMat : public Transformation {
  
public:

  TransMat( float x = 0.0, float y = 0.0, float z = 0.0 );
  
  const TransMat &setX( const float x );
  const TransMat &setY( const float y );
  const TransMat &setZ( const float z );

  const TransMat &set( const float x, const float y, const float z );
  const TransMat &set( const Angel::vec3 &arg );

  const TransMat &delta( const float x, const float y, const float z );
  const TransMat &delta( const Angel::vec3 &arg );

  virtual Angel::mat4 inverse( void ) const;
  virtual Transformation::Subtype type( void ) const;
  virtual void coalesce( Transformation *rhs );
  virtual Transformation *newCopy( void ) const;

};

class ScaleMat : public Transformation {
  
public:

  ScaleMat( float amt = 1.0 );
  ScaleMat( float x, float y, float z );
  
  const ScaleMat &set( const float x, const float y, const float z );
  const ScaleMat &set( const float pct );

  const ScaleMat &adjust( const float x, const float y, const float z );
  const ScaleMat &adjust( const float pct );

  virtual Angel::mat4 inverse( void ) const;
  virtual Transformation::Subtype type( void ) const;  
  virtual void coalesce( Transformation *rhs );
  virtual Transformation *newCopy( void ) const; 
};

#endif
