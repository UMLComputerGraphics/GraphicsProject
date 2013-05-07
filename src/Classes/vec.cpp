/**
 @file vec.cpp
 @author Ed Angel
 @date 2012-12-04
 @brief Implementation for the vec2, vec3, and vec4 classes.
 @details
 Modified heavily from code available from Ed Angel's website,
 http://www.cs.unm.edu/~angel/BOOK/INTERACTIVE_COMPUTER_GRAPHICS/SIXTH_EDITION/
 Published from his book,
 Interactive Computer Graphics
 A Top-Down Approach with OpenGL, Sixth Edition
 Addison-Wesley 2012
 **/

#include "vec.hpp"
#include <cmath>
#include "globals.h"

namespace Angel {
  
//////////////////////////////////////////////////////////////////////////////
//
//  vec2.h - 2D vector
//
  
//
//  --- Constructors and Destructors ---
//
  
  vec2::vec2( GLfloat s ) :
      x( s ), y( s ) {
  }
  
  vec2::vec2( GLfloat x, GLfloat y ) :
      x( x ), y( y ) {
  }
  
  vec2::vec2( const vec2& v ) {
    x = v.x;
    y = v.y;
  }
  
  //
  //  --- Indexing Operator ---
  //
  
  GLfloat& vec2::operator []( int i ) {
    return *(&x + i);
  }
  GLfloat vec2::operator []( int i ) const {
    return *(&x + i);
  }
  
  //
  //  --- (non-modifying) Arithematic Operators ---
  //
  
  vec2 vec2::operator -() const // unary minus operator
  {
    return vec2( -x, -y );
  }
  
  vec2 vec2::operator +( const vec2& v ) const {
    return vec2( x + v.x, y + v.y );
  }
  
  vec2 vec2::operator -( const vec2& v ) const {
    return vec2( x - v.x, y - v.y );
  }
  
  vec2 vec2::operator *( const GLfloat s ) const {
    return vec2( s * x, s * y );
  }
  
  vec2 vec2::operator *( const vec2& v ) const {
    return vec2( x * v.x, y * v.y );
  }
  
  vec2 vec2::operator /( const GLfloat s ) const {
#ifdef DEBUG
    if ( std::fabs( s ) < DIVIDE_BY_ZERO_TOLERANCE ) {
      std::cerr << "[" << __FILE__ << ":" << __LINE__ << "] "
                << "Division by zero" << std::endl;
      return vec2();
    }
#endif // DEBUG
    GLfloat r = GLfloat( 1.0 ) / s;
    return *this * r;
  }
  
  //
  //  --- (modifying) Arithematic Operators ---
  //
  
  vec2& vec2::operator +=( const vec2& v ) {
    x += v.x;
    y += v.y;
    return *this;
  }
  
  vec2& vec2::operator -=( const vec2& v ) {
    x -= v.x;
    y -= v.y;
    return *this;
  }
  
  vec2& vec2::operator *=( const GLfloat s ) {
    x *= s;
    y *= s;
    return *this;
  }
  
  vec2& vec2::operator *=( const vec2& v ) {
    x *= v.x;
    y *= v.y;
    return *this;
  }
  
  vec2& vec2::operator /=( const GLfloat s ) {
#ifdef DEBUG
    if ( std::fabs( s ) < DIVIDE_BY_ZERO_TOLERANCE ) {
      std::cerr << "[" << __FILE__ << ":" << __LINE__ << "] "
                << "Division by zero" << std::endl;
    }
#endif // DEBUG
    GLfloat r = GLfloat( 1.0 ) / s;
    *this *= r;
    
    return *this;
  }
  
  vec2::operator const GLfloat*() const {
    return static_cast< const GLfloat* >( &x );
  }
  
  vec2::operator GLfloat*() {
    return static_cast< GLfloat* >( &x );
  }
  
  //
  //  --- Insertion and Extraction Operators ---
  //
  
//----------------------------------------------------------------------------
//
//  Non-class vec2 Methods
//
  
  vec2 operator *( const GLfloat s, const vec2& v ) {
    return v * s;
  }
  
  std::ostream& operator <<( std::ostream& os, const vec2& v ) {
    return os << "( " << v.x << ", " << v.y << " )";
  }
  
  std::istream& operator >>( std::istream& is, vec2& v ) {
    return is >> v.x >> v.y;
  }
  
  GLfloat dot( const vec2& u, const vec2& v ) {
    return u.x * v.x + u.y * v.y;
  }
  
  GLfloat length( const vec2& v ) {
    return std::sqrt( dot( v, v ) );
  }
  
  vec2 normalize( const vec2& v ) {
    return v / length( v );
  }
  
//////////////////////////////////////////////////////////////////////////////
//
//  vec3.h - 3D vector
//
//////////////////////////////////////////////////////////////////////////////
  
  //
  //  --- Constructors and Destructors ---
  //
  
  vec3::vec3( GLfloat s ) :
      x( s ), y( s ), z( s ) {
  }
  
  vec3::vec3( GLfloat x, GLfloat y, GLfloat z ) :
      x( x ), y( y ), z( z ) {
  }
  
  vec3::vec3( const vec3& v ) {
    if (!v)
      x=y=z=0.0;
    else
    {
      x = v.x;
      y = v.y;
      z = v.z;
    }
  }
  
  vec3::vec3( const vec2& v, const float f ) {
    x = v.x;
    y = v.y;
    z = f;
  }
  
  //
  //  --- Indexing Operator ---
  //
  
  GLfloat& vec3::operator []( int i ) {
    return *(&x + i);
  }
  GLfloat vec3::operator []( int i ) const {
    return *(&x + i);
  }
  
  //
  //  --- (non-modifying) Arithematic Operators ---
  //
  
  vec3 vec3::operator -() const  // unary minus operator
  {
    return vec3( -x, -y, -z );
  }
  
  vec3 vec3::operator +( const vec3& v ) const {
    return vec3( x + v.x, y + v.y, z + v.z );
  }
  
  vec3 vec3::operator -( const vec3& v ) const {
    return vec3( x - v.x, y - v.y, z - v.z );
  }
  
  vec3 vec3::operator *( const GLfloat s ) const {
    return vec3( s * x, s * y, s * z );
  }
  
  vec3 vec3::operator *( const vec3& v ) const {
    return vec3( x * v.x, y * v.y, z * v.z );
  }
  
  vec3 vec3::operator /( const GLfloat s ) const {
#ifdef DEBUG
    if ( std::fabs( s ) < DIVIDE_BY_ZERO_TOLERANCE ) {
      std::cerr << "[" << __FILE__ << ":" << __LINE__ << "] "
                << "Division by zero" << std::endl;
      return vec3();
    }
#endif // DEBUG
    GLfloat r = GLfloat( 1.0 ) / s;
    return *this * r;
  }
  
  //
  //  --- (modifying) Arithematic Operators ---
  //
  
  vec3& vec3::operator +=( const vec3& v ) {
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
  }
  
  vec3& vec3::operator -=( const vec3& v ) {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
  }
  
  vec3& vec3::operator *=( const GLfloat s ) {
    x *= s;
    y *= s;
    z *= s;
    return *this;
  }
  
  vec3& vec3::operator *=( const vec3& v ) {
    x *= v.x;
    y *= v.y;
    z *= v.z;
    return *this;
  }
  
  vec3& vec3::operator /=( const GLfloat s ) {
#ifdef DEBUG
    if ( std::fabs( s ) < DIVIDE_BY_ZERO_TOLERANCE ) {
      std::cerr << "[" << __FILE__ << ":" << __LINE__ << "] "
                << "Division by zero" << std::endl;
    }
#endif // DEBUG
    GLfloat r = GLfloat( 1.0 ) / s;
    *this *= r;
    
    return *this;
  }
  
  //
  //  --- Insertion and Extraction Operators ---
  //
  
  //
  //  --- Conversion Operators ---
  //
  
  vec3::operator const GLfloat*() const {
    return static_cast< const GLfloat* >( &x );
  }
  
  vec3::operator GLfloat*() {
    return static_cast< GLfloat* >( &x );
  }
  
//----------------------------------------------------------------------------
//
//  Non-class vec3 Methods
//
  
  vec3 operator *( const GLfloat s, const vec3& v ) {
    return v * s;
  }
  
  std::ostream& operator <<( std::ostream& os, const vec3& v ) {
    return os << "( " << v.x << ", " << v.y << ", " << v.z << " )";
  }
  
  std::istream& operator >>( std::istream& is, vec3& v ) {
    return is >> v.x >> v.y >> v.z;
  }
  
  GLfloat dot( const vec3& u, const vec3& v ) {
    return u.x * v.x + u.y * v.y + u.z * v.z;
  }
  
  GLfloat length( const vec3& v ) {
    return std::sqrt( dot( v, v ) );
  }
  
  vec3 normalize( const vec3& v ) {
    return v / length( v );
  }
  
  vec3 cross( const vec3& a, const vec3& b ) {
    return vec3( a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z,
                 a.x * b.y - a.y * b.x );
  }
  
//////////////////////////////////////////////////////////////////////////////
//
//  vec4 - 4D vector
//
//////////////////////////////////////////////////////////////////////////////
  
  //
  //  --- Constructors and Destructors ---
  //
  
  vec4::vec4( GLfloat s ) :
      x( s ), y( s ), z( s ), w( s ) {
  }
  
  vec4::vec4( GLfloat x, GLfloat y, GLfloat z, GLfloat w ) :
      x( x ), y( y ), z( z ), w( w ) {
  }
  
  vec4::vec4( const vec4& v ) {
    x = v.x;
    y = v.y;
    z = v.z;
    w = v.w;
  }
  
  vec4::vec4( const vec3& v, const float w ) :
      w( w ) {
    x = v.x;
    y = v.y;
    z = v.z;
  }
  
  vec4::vec4( const vec2& v, const float z, const float w ) :
      z( z ), w( w ) {
    x = v.x;
    y = v.y;
  }
  
  //
  //  --- Indexing Operator ---
  //
  
  GLfloat& vec4::operator []( int i ) {
    return *(&x + i);
  }
  GLfloat vec4::operator []( int i ) const {
    return *(&x + i);
  }
  
  //
  //  --- (non-modifying) Arithematic Operators ---
  //
  
  vec4 vec4::operator -() const  // unary minus operator
  {
    return vec4( -x, -y, -z, -w );
  }
  
  vec4 vec4::operator +( const vec4& v ) const {
    return vec4( x + v.x, y + v.y, z + v.z, w + v.w );
  }
  
  vec4 vec4::operator -( const vec4& v ) const {
    return vec4( x - v.x, y - v.y, z - v.z, w - v.w );
  }
  
  vec4 vec4::operator *( const GLfloat s ) const {
    return vec4( s * x, s * y, s * z, s * w );
  }
  
  vec4 vec4::operator *( const vec4& v ) const {
    return vec4( x * v.x, y * v.y, z * v.z, w * v.z );
  }
  
  vec4 vec4::operator /( const GLfloat s ) const {
#ifdef DEBUG
    if ( std::fabs( s ) < DIVIDE_BY_ZERO_TOLERANCE ) {
      std::cerr << "[" << __FILE__ << ":" << __LINE__ << "] "
                << "Division by zero" << std::endl;
      return vec4();
    }
#endif // DEBUG
    GLfloat r = GLfloat( 1.0 ) / s;
    return *this * r;
  }
  
  //
  //  --- (modifying) Arithematic Operators ---
  //
  
  vec4& vec4::operator +=( const vec4& v ) {
    x += v.x;
    y += v.y;
    z += v.z;
    w += v.w;
    return *this;
  }
  
  vec4& vec4::operator -=( const vec4& v ) {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    w -= v.w;
    return *this;
  }
  
  vec4& vec4::operator *=( const GLfloat s ) {
    x *= s;
    y *= s;
    z *= s;
    w *= s;
    return *this;
  }
  
  vec4& vec4::operator *=( const vec4& v ) {
    x *= v.x, y *= v.y, z *= v.z, w *= v.w;
    return *this;
  }
  
  vec4& vec4::operator /=( const GLfloat s ) {
#ifdef DEBUG
    if ( std::fabs( s ) < DIVIDE_BY_ZERO_TOLERANCE ) {
      std::cerr << "[" << __FILE__ << ":" << __LINE__ << "] "
                << "Division by zero" << std::endl;
    }
#endif // DEBUG
    GLfloat r = GLfloat( 1.0 ) / s;
    *this *= r;
    
    return *this;
  }
  
  //
  //  --- Insertion and Extraction Operators ---
  //
  
  //
  //  --- Conversion Operators ---
  //
  
  vec4::operator const GLfloat*() const {
    return static_cast< const GLfloat* >( &x );
  }
  
  vec4::operator GLfloat*() {
    return static_cast< GLfloat* >( &x );
  }
  
//--------------------------------------------------------------------------
//
//  Non-class vec4 Methods
//
  
  vec4 operator *( const GLfloat s, const vec4& v ) {
    return v * s;
  }
  
  std::ostream& operator <<( std::ostream& os, const vec4& v ) {
    return os << "( " << v.x << ", " << v.y << ", " << v.z << ", " << v.w
              << " )";
  }
  
  std::istream& operator >>( std::istream& is, vec4& v ) {
    return is >> v.x >> v.y >> v.z >> v.w;
  }
  
  GLfloat dot( const vec4& u, const vec4& v ) {
    return u.x * v.x + u.y * v.y + u.z * v.z + u.w + v.w;
  }
  
  GLfloat length( const vec4& v ) {
    return std::sqrt( dot( v, v ) );
  }
  
  vec4 normalize( const vec4& v ) {
    return v / length( v );
  }
  
  vec3 cross( const vec4& a, const vec4& b ) {
    return vec3( a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z,
                 a.x * b.y - a.y * b.x );
  }
  
  vec3 xyz( const vec4& a ) {
    
    return vec3( a.x, a.y, a.z );
    
  }

//----------------------------------------------------------------------------

}// namespace Angel
