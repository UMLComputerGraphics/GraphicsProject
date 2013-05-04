/**
 @file Light.hpp
 @author Nick Ver Voort
 @authors Nick Ver Voort
 @date 2013-05-02
 @brief Header for the Light class.
 **/
#ifndef __LIGHT_H
#define __LIGHT_H

#include <string>
#include "mat.hpp"
#include "vec.hpp"

using Angel::vec2;
using Angel::vec3;
using Angel::vec4;
using Angel::mat4;
using std::string;

class Light {
  
public:

  void commonInit( void );
  
  /**
   Initialization Constructor; sets the x,y,z coordinates explicitly.
   @param name The name of this Light/Object.
   @param x The initial x coordinate.
   @param y The initial y coordinate.
   @param z The initial z coordinate.
   **/
  Light( const std::string &name, float x = 0.0, float y = 0.0,
          float z = 0.0 );

  /**
   Initialization Constructor, uses a vec3 as its initial coordinates.
   @param name The name of this Light/Object.
   @param in A vec3 representing the initial coordinates.
   **/
  Light( const std::string &name, vec3 &in );

  /**
   Initialization Constructor, uses a vec4 as its initial coordinates.
   @param name The name of this Light/Object.
   @param in A vec4 representing the initial coordinates. The w component is ignored.
   **/
  Light( const std::string &name, vec4 &in );

  /**
   Default destructor. Defined only to allow inheritance.
   **/
  virtual ~Light( void );

  /**
   Sets the absolute position of the light.
   @param x The new x coordinate of the light.
   @param y The new y coordinate of the light.
   @param z The new z coordinate of the light.
   @return Void.
   **/
  void pos( const float &x, const float &y, const float &z );

  /**
   Sets the absolute position of the light.
   @param in A vec3 containing the x, y, and z coordinates to set the light to.
   @return Void.
   **/
  void pos( const vec3 &in );

  /**
   Sets the absolute position of the light.
   @param in A vec4 containing the x, y, and z coordinates to set the light to. The w coordinate is ignored.
   @return Void.
   **/
  void pos( const vec4 &in );

  void dX( const float &by );
  void dY( const float &by );
  void dZ( const float &by );

  /**
   Moves the light along the x, y, and z axes.
   @param x the x-axis displacement.
   @param y the y-axis displacement.
   @param z the z-axis displacement.
   @return Void.
   **/
  void dPos( const float &x, const float &y, const float &z );

  /**
   Moves the light along the x, y, and z axes.
   @param by A vec3 containing the x, y, and z axis displacements.
   @return Void.
   **/
  void dPos( const vec3 &by );

  /**
   Moves the light along the x, y, and z axes.
   @param by A vec4 containing the x, y, and z axis displacements. The w component is ignored.
   @return Void.
   **/
  void dPos( const vec4 &by );

  float x( void ) const;
  float y( void ) const;
  float z( void ) const;
 
  void x( const float &in );
  void y( const float &in );
  void z( const float &in );

  /**
   pos() gets the current light position in model coordinates.
   @return A vec4 that represents the current light coordinates.
   **/
  vec4 pos( void ) const;

  /**
     color() gets the current light color.
     @return A vec3 that represents the current light color.
   **/
  vec3 color( void ) const;
  /**
     Sets the color of the light.
     @return void.
   **/
  void color( const vec3 &in ); 

  /**
     intensity() gets the current light intensity.
     @return A float that represents the current light intensity.
   **/
  float intensity( void ) const;
  /**
     Sets the intensity of the light.
     @return void.
   **/
  void intensity( const float &in ); 
  
  /**
   * Functions to get at the underlying GLfloat arrays
   **/
  GLfloat* getGLAmbient( void );
  GLfloat* getGLPosition( void );
  GLfloat* getGLDiffuse( void );
  GLfloat* getGLSpecular( void );

private:

  /** Current world position of the Light **/
  vec3 _position;
  
  /** Color of the emitted light **/
  vec3 _color;

  /** The brightness level of the light **/
  float _intensity;

  GLfloat* _lightAmbient;
  GLfloat* _lightPositions;
  GLfloat* _lightDiffuse;
  GLfloat* _lightSpecular;

};

#endif
