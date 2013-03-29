/**
 @file Light.hpp
 @author Nicholas StPierre
 @authors John Huston, Nicholas StPierre, Chris Compton
 @since 2012-11-20
 @date 2012-12-04
 @brief Implementation for the Light container class.
 **/

#ifndef __LIGHTS
#define __LIGHTS

#include "LightSource.hpp"
#include "mat.hpp"
#include "vec.hpp"
#include <vector>

using Angel::vec2;
using Angel::vec3;
using Angel::vec4;
using Angel::mat4;
using std::vector;

/*
 Point4   position( 0.0, 0.0, 0.0, 0.0 ) ;
 vec3    direction( 0.0, 0.0, 0.0 ) ;
 color4 light_ambient(  0.0, 0.0, 0.0, 1.0 );
 color4 light_diffuse(  0.0, 0.0, 0.0, 1.0 );
 color4 light_specular( 0.0, 0.0, 0.0, 1.0 );
 light_effect effect ;
 const unsigned int id;
 */

#define MAX_LIGHTS 5

class Lights {
  
private:
  
  vector< LightSource > theLights;

  // We live in a very strange world. 
  // We need these arrays to get the light data sent to the shader.
  // All the gross OpenGl garbage shall be handled in this class,
  // with love and attention to data.
  float ambient[MAX_LIGHTS * 3];
  float diffuse[MAX_LIGHTS * 3];
  float specular[MAX_LIGHTS * 3];

  float color[MAX_LIGHTS * 3]; // only used if we invoke simple lighting.
  
  float position[MAX_LIGHTS * 3];
  float direction[MAX_LIGHTS * 3];

  // To get the arrays to the shader, we are going to need handles to them.
  // These variables will hold the handles.
  GLuint g_ambient;
  GLuint g_diffuse;
  GLuint g_specular;

  // This requires some careful explainiation.
  // If you are using a shader that supports simple lighting, then this will be used. 
  // Otherwise, the above three will be used.
  GLuint g_color;

  GLuint g_position;
  GLuint g_direction;
  GLuint g_numLights;

  const bool complexSwitch;

public:
  
  //Lights(); 
  Lights( bool );
  Lights( GLuint &, bool complexSwitch );// Initialization Constructor (side-steps the need for init_lights)
  virtual ~Lights(); // default destructor
  
  void addLightSource( const LightSource & );     // add a light
  void removeLightSource( const unsigned int );       // remove a light by index
  void removeLastLightSource();        // remove a light by index
  void modifyLightSource( const unsigned int u, const LightSource &l );

  void init_lights( /*const*/GLuint & );   // init the vertex shading variables.
  unsigned int getNumLights() const;// get the number of lights in the container.
  
  void moveLight( unsigned int u, Point4 p );

  bool getComplex() const;

  void sendAll();
  
  //void sendAll(bool);
  
};

#endif

/* Ignore this garbaggio :
 color4 LightSource::GetLight_specular() const {
 return light_specular ;
 }
 color4 LightSource::GetLight_diffuse() const {
 return light_diffuse ;
 }
 color4 LightSource::GetLight_ambient() const {
 return light_ambient ;
 }
 vec3 LightSource::GetDirection() const {
 return direction ;
 }
 Point4 LightSource::GetPoint() const {
 return position ;
 }
 */
