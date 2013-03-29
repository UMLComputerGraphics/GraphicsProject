/**
 @file LightSource.cpp
 @author Nicholas StPierre
 @authors John Huston, Nicholas StPierre, Chris Compton
 @since 2012-11-20
 @date 2012-12-04
 @brief Implementation for the Light Source class.
 **/

#include "LightSource.hpp"
#include "mat.hpp"
#include "vec.hpp"

using Angel::vec2;
using Angel::vec3;
using Angel::vec4;
using Angel::mat4;

LightSource::LightSource() {
  //id = nextId ;
  //nextId++;
}

LightSource::LightSource( const LightSource& orig ) {
  
  //  id = nextId ;
  //  nextId++;
  
}

LightSource::LightSource( const Point4& pos ) :
    position( pos ) //, id(nextId) 
{
  //nextId++;
}

LightSource::LightSource( const Point4& pos, const Color4& color ) :
    position( pos ), light_color( color ), complexSwitch( false ) {
  
}

LightSource::LightSource( const Point4& pos, const Color4& ambient,
                          const Color4& diffuse, const Color4& specular ) :
    position( pos ), light_ambient( ambient ), light_diffuse( diffuse ),
    light_specular( specular ), complexSwitch( true )
//, id(nextId)
{
  
  //id = nextId ;
  // nextId++;
  
}

LightSource::LightSource( const Point4& pos, const vec4& dir,
                          const Color4& ambient, const Color4& diffuse,
                          const Color4& specular ) :
    position( pos ), direction( dir ), light_ambient( ambient ),
    light_diffuse( diffuse ), light_specular( specular ), complexSwitch( true )//id(nextId)
{
  //id = nextId ;
  //nextId++;
  
}

LightSource::~LightSource() {
  // nothing special
}

bool LightSource::GetComplexSwitch() const {
  return complexSwitch;
}

void LightSource::SetLight_specular( Color4 &light_specular ) {
  this->light_specular = light_specular;
}

Color4 LightSource::GetLight_specular() const {
  return light_specular;
}

void LightSource::SetLight_diffuse( Color4 &light_diffuse ) {
  this->light_diffuse = light_diffuse;
}

Color4 LightSource::GetLight_diffuse() const {
  return light_diffuse;
}

void LightSource::SetLight_ambient( Color4 &light_ambient ) {
  this->light_ambient = light_ambient;
}

Color4 LightSource::GetLight_ambient() const {
  return light_ambient;
}

void LightSource::SetLight_color( Color4 &c ) {
  this->light_color = c;
}

Color4 LightSource::GetLight_color() const {
  return light_color;
}

void LightSource::SetDirection( vec4 &direction ) {
  this->direction = direction;
}

vec4 LightSource::GetDirection() const {
  return direction;
}

void LightSource::SetPosition( Point4 &position ) {
  this->position = position;
}

Point4 LightSource::GetPosition() const {
  return position;
}

/*
 void LightSource::SetShaderHandle( GLuint g ) {
 shaderHandle = g ;
 }
 GLuint LightSource::GetShaderHandle() const {
 return shaderHandle ;
 }
 */
