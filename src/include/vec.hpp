/**
 * @file vec.hpp
 * @date 2013-03-29
 * @author Ed Angel
 * @brief Headers for vec2, vec3, and vec4.
 * @details Headers for 2D, 3D and 4D vectors, as well as supporting utility functions.
 */

#ifndef __ANGEL_VEC_H__
#define __ANGEL_VEC_H__

#include "OpenGL.h"
#include <iostream>

/**
 * Namespace to encompass Ed Angel's functions and classes.
 * @author Ed Angel
 * @date 2013-03-30
 */
namespace Angel {
  
/**
 * vec2: 2D vector.
 * @author Ed Angel
 * @date 2013-03-30
 */
  struct vec2 {
    
    GLfloat x;
    GLfloat y;

    //  --- Constructors and Destructors ---
    vec2( GLfloat s = GLfloat( 0.0 ) );
    vec2( GLfloat x, GLfloat y );
    vec2( const vec2& v );

    //  --- Indexing Operator ---
    GLfloat& operator []( int i );
    GLfloat operator []( int i ) const;

    //  --- (non-modifying) Arithematic Operators ---
    vec2 operator -() const; // unary minus operator
    vec2 operator +( const vec2& v ) const;
    vec2 operator -( const vec2& v ) const;
    vec2 operator *( const GLfloat s ) const;
    vec2 operator *( const vec2& v ) const;
    friend vec2 operator *( const GLfloat s, const vec2& v );
    vec2 operator /( const GLfloat s ) const;

    //  --- (modifying) Arithematic Operators ---
    vec2& operator +=( const vec2& v );
    vec2& operator -=( const vec2& v );
    vec2& operator *=( const GLfloat s );
    vec2& operator *=( const vec2& v );
    vec2& operator /=( const GLfloat s );

    //  --- Insertion and Extraction Operators ---
    friend std::ostream& operator <<( std::ostream& os, const vec2& v );
    friend std::istream& operator >>( std::istream& is, vec2& v );

    //  --- Conversion Operators ---
    operator const GLfloat*() const;
    operator GLfloat*();
  };
  
  //  Non-class vec2 Methods
  GLfloat dot( const vec2& u, const vec2& v );
  GLfloat length( const vec2& v );
  vec2 normalize( const vec2& v );
  
/**
 * vec3 - 3D vector.
 * @author Ed Angel
 * @date 2013-03-30
 */
  struct vec3 {
    
    GLfloat x;
    GLfloat y;
    GLfloat z;

    //  --- Constructors and Destructors ---
    vec3( GLfloat s = GLfloat( 0.0 ) );
    vec3( GLfloat x, GLfloat y, GLfloat z );
    vec3( const vec3& v );
    vec3( const vec2& v, const float f );

    //  --- Indexing Operator ---
    GLfloat& operator []( int i );
    GLfloat operator []( int i ) const;

    //  --- (non-modifying) Arithematic Operators ---
    vec3 operator -() const;
    vec3 operator +( const vec3& v ) const;
    vec3 operator -( const vec3& v ) const;
    vec3 operator *( const GLfloat s ) const;
    vec3 operator *( const vec3& v ) const;
    friend vec3 operator *( const GLfloat s, const vec3& v );
    vec3 operator /( const GLfloat s ) const;

    //  --- (modifying) Arithematic Operators ---
    vec3& operator +=( const vec3& v );
    vec3& operator -=( const vec3& v );
    vec3& operator *=( const GLfloat s );
    vec3& operator *=( const vec3& v );
    vec3& operator /=( const GLfloat s );

    //  --- Insertion and Extraction Operators ---
    friend std::ostream& operator <<( std::ostream& os, const vec3& v );
    friend std::istream& operator >>( std::istream& is, vec3& v );

    //  --- Conversion Operators ---
    operator const GLfloat*() const;
    operator GLfloat*();
    
  };
  
  //  Non-class vec3 Methods
  GLfloat dot( const vec3& u, const vec3& v );
  GLfloat length( const vec3& v );
  vec3 normalize( const vec3& v );
  vec3 cross( const vec3& a, const vec3& b );
  
/**
 * vec4 - 4D vector.
 * @author Ed Angel
 * @date 2013-03-30
 */
  struct vec4 {
    
    GLfloat x;
    GLfloat y;
    GLfloat z;
    GLfloat w;

    //  --- Constructors and Destructors ---
    vec4( GLfloat s = GLfloat( 0.0 ) );
    vec4( GLfloat x, GLfloat y, GLfloat z, GLfloat w );
    vec4( const vec4& v );
    vec4( const vec3& v, const float w = 1.0 );
    vec4( const vec2& v, const float z, const float w );

    //  --- Indexing Operator ---
    GLfloat& operator []( int i );
    GLfloat operator []( int i ) const;

    //  --- (non-modifying) Arithematic Operators ---
    vec4 operator -() const;
    vec4 operator +( const vec4& v ) const;
    vec4 operator -( const vec4& v ) const;
    vec4 operator *( const GLfloat s ) const;
    vec4 operator *( const vec4& v ) const;
    friend vec4 operator *( const GLfloat s, const vec4& v );
    vec4 operator /( const GLfloat s ) const;

    //  --- (modifying) Arithematic Operators ---
    vec4& operator +=( const vec4& v );
    vec4& operator -=( const vec4& v );
    vec4& operator *=( const GLfloat s );
    vec4& operator *=( const vec4& v );
    vec4& operator /=( const GLfloat s );

    //  --- Insertion and Extraction Operators ---
    friend std::ostream& operator <<( std::ostream& os, const vec4& v );
    friend std::istream& operator >>( std::istream& is, vec4& v );

    //  --- Conversion Operators ---
    operator const GLfloat*() const;
    operator GLfloat*();
    
  };
  
  //  Non-class vec4 Methods
  GLfloat dot( const vec4& u, const vec4& v );
  GLfloat length( const vec4& v );
  vec4 normalize( const vec4& v );
  vec3 cross( const vec4& a, const vec4& b );
  vec3 xyz( const vec4& a );

}  // namespace Angel

/** Simple alias of Angel::vec4 to emphasize semantic meaning. */
typedef Angel::vec4 Color4;
/** Simple alias of Angel::vec4 to emphasize semantic meaning. */
typedef Angel::vec4 Point4;

#endif // __ANGEL_VEC_H__
