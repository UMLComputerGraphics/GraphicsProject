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

Transformation::Transformation( void ) {
  _inheritable = true;
  _new = false;
}
Transformation::~Transformation( void ) {
  // Nihil. Provided for inheritance only.
}

const Angel::mat4 &Transformation::matrix( void ) const {
  return mat;
}

Angel::mat4 Transformation::operator*( const Angel::mat4 &rhs ) const {
  return mat * rhs;
}

Angel::mat4 Transformation::operator*( const Transformation &rhs ) const {
  return mat * rhs.matrix();
}

bool Transformation::inheritable( void ) const {
  return _inheritable;
}

void Transformation::markNew( void ) {
  _new = true;
}

Angel::mat4 operator*( const Angel::mat4 &lhs, const Transformation &rhs ) {
  return lhs * rhs.matrix();
}

/* ROTATION */

const RotMat &RotMat::reset( const Angel::mat4 &NewState ) {
  mat = NewState;
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
  if ( postmult ) mat = adjustment * mat;
  // Otherwise, we are pre-multiplying, and later adjustments
  // really do come last.
  else mat = mat * adjustment;
  return (*this);
}

Angel::mat4 RotMat::inverse( void ) const {

  return transpose( mat );

}

Transformation::Subtype RotMat::type( void ) const {
  return Transformation::ROTATION;
}

/* TRANSLATION */

const TransMat &TransMat::setX( const float x ) {
  //fprintf( stderr, "setX( %f )\n", x );
  mat[0][3] = x;
  return (*this);
}

const TransMat &TransMat::setY( const float y ) {
  //fprintf( stderr, "setY( %f )\n", y );
  mat[1][3] = y;
  return (*this);
}

const TransMat &TransMat::setZ( const float z ) {
  //fprintf( stderr, "setZ( %f )\n", z );
  mat[2][3] = z;
  return (*this);
}

const TransMat &TransMat::set( const float x, const float y, const float z ) {
  mat[0][3] = x;
  mat[1][3] = y;
  mat[2][3] = z;
  return (*this);
}

const TransMat &TransMat::set( const Angel::vec3 &arg ) {
  return set( arg.x, arg.y, arg.z );
}

const TransMat &TransMat::delta( const float x, const float y, const float z ) {
  mat[0][3] += x;
  mat[1][3] += y;
  mat[2][3] += z;
  return (*this);
}

const TransMat &TransMat::delta( const Angel::vec3 &arg ) {
  return delta( arg.x, arg.y, arg.z );
}

Angel::mat4 TransMat::inverse( void ) const {

  return Angel::mat4( 1, 0, 0, -mat[0][3],
                      0, 1, 0, -mat[1][3],
                      0, 0, 1, -mat[2][3],
                      0, 0, 0, 1);

}

Transformation::Subtype TransMat::type( void ) const {
  return Transformation::TRANSLATION;
}

/* SCALE */

const ScaleMat &ScaleMat::set( const float x, const float y, const float z ) {
  
  mat[0][0] = x;
  mat[1][1] = y;
  mat[2][2] = z;
  return (*this);
  
}

const ScaleMat &ScaleMat::set( const float pct ) {
  return set( pct, pct, pct );
}

const ScaleMat &ScaleMat::adjust( const float x, const float y,
                                  const float z ) {
  mat[0][0] *= x;
  mat[1][1] *= y;
  mat[2][2] *= z;
  return (*this);
}

const ScaleMat &ScaleMat::adjust( const float pct ) {
  return adjust( pct, pct, pct );
}

Angel::mat4 ScaleMat::inverse( void ) const {

  return Angel::mat4( (1/mat[0][0]), 0, 0, 0,
                      0, (1/mat[1][1]), 0, 0,
                      0, 0, (1/mat[2][2]), 0,
                      0, 0, 0, 1);

}


Transformation::Subtype ScaleMat::type( void ) const {
  return Transformation::SCALE;
}
