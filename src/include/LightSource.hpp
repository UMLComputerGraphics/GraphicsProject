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

typedef vec4 point4, color4;

class LightSource
{

  typedef enum {
    OFF=0, 
    ON,
    BLINK,
    STROBE,
    PULSE
  } light_effect;


 private:

   point4   position ; //( 0.0, 0.0, 0.0, 0.0 ) ;
   vec4    direction ; //( 0.0, 0.0, 0.0 ) ;

   color4 light_ambient ; //(  0.0, 0.0, 0.0, 1.0 );
   color4 light_diffuse ; //(  0.0, 0.0, 0.0, 1.0 );
   color4 light_specular ; //( 0.0, 0.0, 0.0, 1.0 );

   color4 light_color ; //( 0.0, 0.0, 0.0, 1.0 );

   bool complexSwitch ;

   light_effect effect ;



  //const  unsigned int id;
  // static unsigned int nextId;

  //GLuint shaderHandle ;

 public:

   LightSource();
   LightSource(const point4&);
   LightSource(const point4&, const color4&);
   LightSource(const point4&, const color4&, const color4&, const color4&);
   LightSource(const point4&, const vec4&, const color4&, const color4&, const color4&);
  //LightSource(point4, vec4, color4, color4, color4, unsigned int);

   LightSource(const LightSource& orig);

  /*virtual*/ ~LightSource();


   // Set/Get Member pairs
   void SetLight_specular(color4 &light_specular);
   color4 GetLight_specular() const;

   void SetLight_diffuse(color4 &light_diffuse);
   color4 GetLight_diffuse() const;

   void SetLight_ambient(color4 &light_ambient);
   color4 GetLight_ambient() const;

   void SetLight_color(color4 &light_color);
   color4 GetLight_color() const;

   void SetDirection(vec4 &direction);
   vec4 GetDirection() const;

   void SetPosition(point4 &z);
   point4 GetPosition() const;

   bool GetComplexSwitch() const ;


   //void SetShaderHandle(GLuint);
   //GLuint GetShaderHandle() const;

   // void SetId(vec4 z); // this shouldn't work
   //unsigned int GetId() const;


  // Effect Fxns/Modifiers
   void DoEvents(void);
   void setEffect(int);


  // Predicate Functions
   //bool isDistant(); // check the homogenous coordinate. if zero, true.


  /* OpenGL Methods */
  //void send( const glsl_var &which );
  //void link( GLuint &program,
  //             const glsl_var &which,
  //         const string &glslVarName );

};

#endif
