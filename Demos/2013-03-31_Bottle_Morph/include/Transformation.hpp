#ifndef __TRANSMAT_HPP
#define __TRANSMAT_HPP

#include "mat.hpp"
#include "vec.hpp"
#include "platform.h" //OpenGL types.

class Transformation {

public:
  /*
    These are all default and are not currently needed.
    Transformation( void );
    Transformation( const Transformation &copy );
    Transformation &operator=( const Transformation &assignment );
  */
  virtual ~Transformation( void );

  const Angel::mat4 &Matrix( void ) const;
  Angel::mat4 operator*( const Angel::mat4 &rhs ) const;
  Angel::mat4 operator*( const Transformation &rhs ) const;

protected:
  Angel::mat4 mat;

};

Angel::mat4 operator*( const Angel::mat4 &lhs, const Transformation &rhs );


/** Rotations **/

class RotMat : public Transformation {
  
public:

  const RotMat &Reset( const Angel::mat4 &NewState );
  const RotMat &RotateX( const GLfloat theta, bool postmult = true );
  const RotMat &RotateY( const GLfloat theta, bool postmult = true );
  const RotMat &RotateZ( const GLfloat theta, bool postmult = true );
  const RotMat &Adjust( const Angel::mat4 &Adjustment, bool postmult = true );
  
};

/** Translations **/

class TransMat : public Transformation {
  
public:

  const TransMat &SetX( const float x );
  const TransMat &SetY( const float y );
  const TransMat &SetZ( const float z );
  
  const TransMat &Set( const float x, const float y, const float z );
  const TransMat &Set( const Angel::vec3 &arg );

  const TransMat &Delta( const float x, const float y, const float z );
  const TransMat &Delta( const Angel::vec3 &arg );

};

class ScaleMat : public Transformation {

public:

  const ScaleMat &Set( const float x, const float y, const float z );
  const ScaleMat &Set( const float pct );

  const ScaleMat &Adjust( const float x, const float y, const float z );
  const ScaleMat &Adjust( const float pct );

};

#endif
