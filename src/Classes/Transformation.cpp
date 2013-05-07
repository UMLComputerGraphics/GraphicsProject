/**
 * @file Transformation.cpp
 * @author John Huston
 * @date 2013-03-28
 * @brief Transformation Superclass Implementation
 * @details Implementation for a generic "Transformation"
 * specialized 4x4 matrix (mat4).
 */

#include "mat.hpp"
#include "Transformation.hpp"
#include "platform.h" //OpenGL types.
#include <stdexcept>

Transformation::Transformation( void ) {
  _inheritable = true;
  _new = false;
}
Transformation::~Transformation( void ) {
  // Nihil. Provided for inheritance only.
}

const Angel::mat4 &Transformation::matrix( void ) const {
  return _mat;
}

void Transformation::reset( void ) {
  _mat = Angel::mat4();
}

bool Transformation::inheritable( void ) const {
  return _inheritable;
}

void Transformation::inheritable( bool in ) {
  _inheritable = in;
}

void Transformation::markNew( void ) {
  _new = true;
}

void Transformation::markOld( void ) {
  _new = false;
}

bool Transformation::isNew( void ) {
  return _new;
}

/** Transformation x mat4 **/
Angel::mat4 Transformation::operator*( const Angel::mat4 &rhs ) const {
  return _mat * rhs;
}

/** Transformation x Transformation **/
Angel::mat4 Transformation::operator*( const Transformation &rhs ) const {
  return _mat * rhs.matrix();
}

/** mat4 x Transformation **/
Angel::mat4 operator*( const Angel::mat4 &lhs, const Transformation &rhs ) {
  return lhs * rhs.matrix();
}

void Transformation::combine( Transformation *rhs ) {
  if (type() != rhs->type()) {
    throw std::logic_error( "Cannot combine two Transformations of dissimilar types." );
  }

  _mat *= rhs->matrix();
  rhs->reset();
}

/* ROTATION */

RotMat::RotMat( float rx, float ry, float rz ) {
  rotateX( rx );
  rotateY( ry );
  rotateZ( rz );
}

const RotMat &RotMat::reset( const Angel::mat4 &NewState ) {
  _mat = NewState;
  return (*this);
}

const RotMat &RotMat::rotateX( GLfloat theta, bool postmult ) {
  return adjust( Angel::RotateX( theta ), postmult );
}

const RotMat &RotMat::rotateY( GLfloat theta, bool postmult ) {
  return adjust( Angel::RotateY( theta ), postmult );
}

const RotMat &RotMat::rotateZ( GLfloat theta, bool postmult ) {
  return adjust( Angel::RotateZ( theta ), postmult );
}

const RotMat &RotMat::adjust( const Angel::mat4 &adjustment, bool postmult ) {
  // If we are post-multiplying, The default,
  // Adjustments, which come "last", must appear first.
  if ( postmult ) _mat = adjustment * _mat;
  // Otherwise, we are pre-multiplying, and later adjustments
  // really do come last.
  else _mat = _mat * adjustment;
  return (*this);
}

Angel::mat4 RotMat::inverse( void ) const {

  return transpose( _mat );

}

Transformation::Subtype RotMat::type( void ) const {
  return Transformation::ROTATION;
}

void RotMat::coalesce( Transformation *rhs ) {
  if (NULL == dynamic_cast<RotMat*>(rhs)) {
    throw std::logic_error( "Cannot coalesce two Transformations of differing types.\n" );
  }

  // Two rotations can be combined via (lhs * rhs).
  // Not a lot of ways to escape that. We COULD optimize by
  // Considering only the upper-left 3x3,
  // But let's just let the hardware handle those zero multiplications
  // and use the standard matrix multiply instead.
  _mat *= rhs->matrix();
  rhs->reset();
}

Transformation *RotMat::newCopy( void ) const {
  return new RotMat( *this );
}

/* TRANSLATION */

TransMat::TransMat( float x, float y, float z ) {
  set( x, y, z );
}

const TransMat &TransMat::setX( const float x ) {
  //fprintf( stderr, "setX( %f )\n", x );
  _mat[0][3] = x;
  return (*this);
}

const TransMat &TransMat::setY( const float y ) {
  //fprintf( stderr, "setY( %f )\n", y );
  _mat[1][3] = y;
  return (*this);
}

const TransMat &TransMat::setZ( const float z ) {
  //fprintf( stderr, "setZ( %f )\n", z );
  _mat[2][3] = z;
  return (*this);
}

const TransMat &TransMat::set( const float x, const float y, const float z ) {
  _mat[0][3] = x;
  _mat[1][3] = y;
  _mat[2][3] = z;
  return (*this);
}

const TransMat &TransMat::set( const Angel::vec3 &arg ) {
  return set( arg.x, arg.y, arg.z );
}

const TransMat &TransMat::delta( const float x, const float y, const float z ) {
  _mat[0][3] += x;
  _mat[1][3] += y;
  _mat[2][3] += z;
  return (*this);
}

const TransMat &TransMat::delta( const Angel::vec3 &arg ) {
  return delta( arg.x, arg.y, arg.z );
}

Angel::mat4 TransMat::inverse( void ) const {

  return Angel::mat4( 1, 0, 0, -_mat[0][3],
                      0, 1, 0, -_mat[1][3],
                      0, 0, 1, -_mat[2][3],
                      0, 0, 0, 1);

}

Transformation::Subtype TransMat::type( void ) const {
  return Transformation::TRANSLATION;
}

void TransMat::coalesce( Transformation *rhs ) {
  if (NULL == dynamic_cast<TransMat*>(rhs)) {
    throw std::logic_error( "Cannot coalesce two Transformations of differing types.\n" );
  }

  // We can combine two translations by adding up the fourth column.
  _mat[0][3] += rhs->matrix()[0][3];
  _mat[1][3] += rhs->matrix()[1][3];
  _mat[2][3] += rhs->matrix()[2][3];
  rhs->reset();
}

Transformation *TransMat::newCopy( void ) const {
  return new TransMat( *this );
}

/* SCALE */

ScaleMat::ScaleMat( float amt ) {
  set( amt );
}
ScaleMat::ScaleMat( float x, float y, float z ) {
  set( x, y, z );
}

const ScaleMat &ScaleMat::set( const float x, const float y, const float z ) {
  
  _mat[0][0] = x;
  _mat[1][1] = y;
  _mat[2][2] = z;
  return (*this);
  
}

const ScaleMat &ScaleMat::set( const float pct ) {
  return set( pct, pct, pct );
}

const ScaleMat &ScaleMat::adjust( const float x, const float y,
                                  const float z ) {
  _mat[0][0] *= x;
  _mat[1][1] *= y;
  _mat[2][2] *= z;
  return (*this);
}

const ScaleMat &ScaleMat::adjust( const float pct ) {
  return adjust( pct, pct, pct );
}

Angel::mat4 ScaleMat::inverse( void ) const {

  return Angel::mat4( (1/_mat[0][0]), 0, 0, 0,
                      0, (1/_mat[1][1]), 0, 0,
                      0, 0, (1/_mat[2][2]), 0,
                      0, 0, 0, 1);

}


Transformation::Subtype ScaleMat::type( void ) const {
  return Transformation::SCALE;
}

void ScaleMat::coalesce( Transformation *rhs ) {
  if (NULL == dynamic_cast<ScaleMat*>(rhs)) {
    throw std::logic_error( "Cannot coalesce two Transformations of differing types.\n" );
  }

  // Rotational Matrices contain only diagonal scaling elements.
  // We can combine by multiplying only three elements.
  _mat[0][0] *= rhs->matrix()[0][0];
  _mat[1][1] *= rhs->matrix()[1][1];
  _mat[2][2] *= rhs->matrix()[2][2];

  rhs->reset();
}

Transformation *ScaleMat::newCopy( void ) const {
  return new ScaleMat( *this );
}
