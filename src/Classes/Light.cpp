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

Light::Light( const std::string &name, float x, float y,
                float z ) {
  this->pos( x, y, z );
}

Light::Light( const std::string &name, vec3 &in ) {
  this->pos( in );
}

Light::Light( const std::string &name, vec4 &in ) {
  this->pos( in );
}

Light::~Light( void ) {
  
}

void Light::x( const float &in ) {
  _position.x = in;
}
void Light::y( const float &in ) {
  _position.y = in;
}
void Light::z( const float &in ) {
  _position.z = in;
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
}

float Light::intensity( void ) const {
  return _intensity;
}

void Light::intensity( const float &in ) {
  _intensity = in;
}
