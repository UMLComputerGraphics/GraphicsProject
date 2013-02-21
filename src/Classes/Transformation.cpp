#include "mat.hpp"
#include "Transformation.hpp"
#include "platform.h" //OpenGL types.

Transformation::~Transformation( void ) {
  // Nihil. Provided for inheritance only.
}

const Angel::mat4 &Transformation::Matrix( void ) const {
  return mat;
}

Angel::mat4 Transformation::operator*( const Angel::mat4 &rhs ) const {
  return mat * rhs;
}

Angel::mat4 Transformation::operator*( const Transformation &rhs ) const {
  return mat * rhs.Matrix();
}

Angel::mat4 operator*( const Angel::mat4 &lhs, const Transformation &rhs ) {
  return lhs * rhs.Matrix();
}

/* ROTATION */

const RotMat &RotMat::Reset( const Angel::mat4 &NewState ) {
  mat = NewState;
  return (*this);
}

const RotMat &RotMat::RotateX( GLfloat theta, bool postmult ) {
  return Adjust( Angel::RotateX( theta ), postmult );
}

const RotMat &RotMat::RotateY( GLfloat theta, bool postmult ) {
  return Adjust( Angel::RotateY( theta ), postmult );
}

const RotMat &RotMat::RotateZ( GLfloat theta, bool postmult ) {
  return Adjust( Angel::RotateZ( theta ), postmult );
}

const RotMat &RotMat::Adjust( const Angel::mat4 &adjustment, bool postmult ) {
  // If we are post-multiplying, The default,
  // Adjustments, which come "last", must appear first.
  if (postmult) mat = adjustment * mat;
  // Otherwise, we are pre-multiplying, and later adjustments
  // really do come last.
  else mat = mat * adjustment;
  return (*this);
}

/* TRANSLATION */

const TransMat &TransMat::SetX( const float x ) {
  //fprintf( stderr, "SetX( %f )\n", x );
  mat[0][3] = x;
  return (*this);
}

const TransMat &TransMat::SetY( const float y ) {
  //fprintf( stderr, "SetY( %f )\n", y );
  mat[1][3] = y;
  return (*this);
}

const TransMat &TransMat::SetZ( const float z ) {
  //fprintf( stderr, "SetZ( %f )\n", z );
  mat[2][3] = z;
  return (*this);
}

const TransMat &TransMat::Set( const float x, const float y, const float z ) {
  mat[0][3] = x;
  mat[1][3] = y;
  mat[2][3] = z;
  return (*this);
}

const TransMat &TransMat::Set( const Angel::vec3 &arg ) {
  return Set( arg.x, arg.y, arg.z );
}

const TransMat &TransMat::Delta( const float x, const float y, const float z ) {
  mat[0][3] += x;
  mat[1][3] += y;
  mat[2][3] += z;
  return (*this);
}
  
const TransMat &TransMat::Delta( const Angel::vec3 &arg ) {
  return Delta( arg.x, arg.y, arg.z );
}

/* SCALE */

const ScaleMat &ScaleMat::Set( const float x, const float y, const float z ) {

  mat[0][0] = x;
  mat[1][1] = y;
  mat[2][2] = z;
  return (*this);

}

const ScaleMat &ScaleMat::Set( const float pct ) {
  return Set( pct, pct, pct );
}

const ScaleMat &ScaleMat::Adjust( const float x, const float y, const float z ) {
  mat[0][0] *= x;
  mat[1][1] *= y;
  mat[2][2] *= z;
  return (*this);
}

const ScaleMat &ScaleMat::Adjust( const float pct ) {
  return Adjust( pct, pct, pct );
}
