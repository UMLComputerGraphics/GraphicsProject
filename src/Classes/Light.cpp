/**
 @file Light.cpp
 @author Nick Ver Voort
 @authors Nick Ver Voort
 @date 2013-05-02
 @brief Implementation for the Light class.
 **/

#include <iostream>
#include <stdexcept>

#include "vec.hpp"
#include "Light.hpp"
#include "Util.hpp"
using namespace Angel;

void Light::commonInit( void ) {

  _lightAmbient = (GLfloat *) malloc(sizeof(GLfloat) * 4);
  _lightAmbient[0] = _lightAmbient[1] = _lightAmbient[2] = 0.08;
  
  _lightPositions = (GLfloat *) malloc(sizeof(GLfloat) * 4);
  _lightPositions[0] = _lightPositions[1] =  _lightPositions[2] = 0.0;

  _lightDiffuse = (GLfloat *) malloc(sizeof(GLfloat) * 4);
  this->color( vec3( 1.0, 1.0, 1.0 ) );
  
  _lightSpecular = (GLfloat *) malloc(sizeof(GLfloat) * 4);
  _lightSpecular[0] = _lightSpecular[1] = _lightSpecular[2] = 0.2;

  //w 
  _lightAmbient[3] = _lightPositions[3] = _lightDiffuse[3] = _lightSpecular[3] = 1.0;

  _intensity = 1.0;

}

Light::Light( const std::string &name, float x, float y,
                float z ) {
  commonInit();
  this->pos( x, y, z );
}

Light::Light( const std::string &name, vec3 &in ) {
  commonInit();
  this->pos( in );
}

Light::Light( const std::string &name, vec4 &in ) {
  commonInit();
  this->pos( in );
}

Light::~Light( void ) {

  if (_lightAmbient) free( _lightAmbient );
  if (_lightPositions) free( _lightPositions );
  if (_lightDiffuse) free( _lightDiffuse );
  if (_lightSpecular) free( _lightSpecular );
  
}

void Light::x( const float &in ) {
  _position.x = in;
  _lightPositions[0] = in;
}
void Light::y( const float &in ) {
  _position.y = in;
  _lightPositions[1] = in;
}
void Light::z( const float &in ) {
  _position.z = in;
  _lightPositions[2] = in;
}

void Light::pos( const float &newX, const float &newY, const float &newZ ) {
  x( newX );
  y( newY );
  z( newZ );
}

void Light::pos( const vec4 &in ) {
  this->pos( in.x, in.y, in.z );
}

void Light::pos( const vec3 &in ) {
  this->pos( in.x, in.y, in.z );
}

void Light::dX( const float &by ) {
  x( x() + by );
}

void Light::dY( const float &by ) {
  y( y() + by );
}

void Light::dZ( const float &by ) {
  z( z() + by );
}

void Light::dPos( const float &x, const float &y, const float &z ) {
  dX( x );
  dY( y );
  dZ( z );
}

void Light::dPos( const vec3 &by ) {
  dPos( by.x, by.y, by.z );
}

void Light::dPos( const vec4 &by ) {
  dPos( by.x, by.y, by.z );
}

float Light::x( void ) const {
  return _position.x;
}
float Light::y( void ) const {
  return _position.y;
}
float Light::z( void ) const {
  return _position.z;
}

vec4 Light::pos( void ) const {
  return vec4( x(), y(), z(), 1.0 );
}

vec3 Light::color( void ) const {
  return _color;
}

void Light::color( const vec3 &in ) {
  _color = in;
  _lightDiffuse[0] = _color.x;
  _lightDiffuse[1] = _color.y;
  _lightDiffuse[2] = _color.z;
}

float Light::intensity( void ) const {
  return _intensity;
}

void Light::intensity( const float &in ) {
  _intensity = in;
}

GLfloat* Light::getGLAmbient( void ) {
  return _lightAmbient;
}
GLfloat* Light::getGLPosition( void ) {
  return _lightPositions;
}
GLfloat* Light::getGLDiffuse( void ) {
  return _lightDiffuse;
}
GLfloat* Light::getGLSpecular( void ) {
  return _lightSpecular;
}
