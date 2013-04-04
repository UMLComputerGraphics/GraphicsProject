/*****************************************
 * Lighting Class for OpenGL             *
 * Nick St.Pierre                        *
 * November 20, 2012                     *
 ****************************************/

#ifndef _LIGHTSOURCE
#define _LIGHTSOURCE

#include <vector>
#include "platform.h"
#include "mat.hpp"
#include "vec.hpp"

using Angel::vec2;
using Angel::vec3;
using Angel::vec4;
using Angel::mat4;
using std::vector;

typedef vec4 Point4, Color4;

class LightSource {
  
  typedef enum {
    OFF = 0, ON, BLINK, STROBE, PULSE
  } light_effect;

private:
  
  Point4 position; //( 0.0, 0.0, 0.0, 0.0 ) ;
  vec4 direction; //( 0.0, 0.0, 0.0 ) ;
  
  Color4 light_ambient; //(  0.0, 0.0, 0.0, 1.0 );
  Color4 light_diffuse; //(  0.0, 0.0, 0.0, 1.0 );
  Color4 light_specular; //( 0.0, 0.0, 0.0, 1.0 );
  
  Color4 light_color; //( 0.0, 0.0, 0.0, 1.0 );
  
  bool complexSwitch;

  light_effect effect;

  //const  unsigned int id;
  // static unsigned int nextId;
  
  //GLuint shaderHandle ;
  
public:
  
  LightSource();
  LightSource( const Point4& );
  LightSource( const Point4&, const Color4& );
  LightSource( const Point4&, const Color4&, const Color4&, const Color4& );
  LightSource( const Point4&, const vec4&, const Color4&, const Color4&,
               const Color4& );
  //LightSource(Point4, vec4, Color4, Color4, Color4, unsigned int);
  
  LightSource( const LightSource& orig );

  /*virtual*/
  ~LightSource();

  // Set/Get Member pairs
  void SetLight_specular( Color4 &light_specular );
  Color4 GetLight_specular() const;

  void SetLight_diffuse( Color4 &light_diffuse );
  Color4 GetLight_diffuse() const;

  void SetLight_ambient( Color4 &light_ambient );
  Color4 GetLight_ambient() const;

  void SetLight_color( Color4 &light_color );
  Color4 GetLight_color() const;

  void SetDirection( vec4 &direction );
  vec4 GetDirection() const;

  void SetPosition( Point4 &z );
  Point4 GetPosition() const;

  bool GetComplexSwitch() const;

  //void SetShaderHandle(GLuint);
  //GLuint GetShaderHandle() const;
  
  // void SetId(vec4 z); // this shouldn't work
  //unsigned int GetId() const;
  
  // Effect Fxns/Modifiers
  void DoEvents( void );
  void setEffect( int );
  
  // Predicate Functions
  //bool isDistant(); // check the homogenous coordinate. if zero, true.
  
  /* OpenGL Methods */
  //void send( const glsl_var &which );
  //void link( GLuint &program,
  //             const glsl_var &which,
  //         const string &glslVarName );
};

#endif
