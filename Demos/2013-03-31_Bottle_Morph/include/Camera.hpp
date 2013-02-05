#ifndef __CAMERA_H
#define __CAMERA_H


#include <string>
#include "mat.hpp"
#include "vec.hpp"
#include "Object.hpp"

using Angel::vec2;
using Angel::vec3;
using Angel::vec4;
using Angel::mat4;
using std::string;

/**
   @author John Huston, jhuston@cs.uml.edu
   @since 16 Nov 2012

   @brief The Camera class represents a logical camera in a model view,
   which posesses a current viewing angle and an absolute position 
   in space as its state.

   Functions are provided to adjust the rotation according to 
   pitch(), yaw() and roll() motions; surge(), sway(), and heave()
   are provided to adjust position in space.

   Move(), Stop(), and Idle() are provided to help the camera
   automatically move along the X, Y, or Z axes.
**/
class Camera : public Object {

public:

  /**
     The Direction enumeration lists all of the possible directions
     the camera may travel in. 'Begin' and 'End' are special sentinel
     directions for the purposes of iteration, and are ignored by
     any functions that accept a Direction.
  **/
  typedef enum {
    Forward,
    Backward,
    Left,
    Right,
    Up,
    Down,
    Direction_End,
    Direction_Begin = Forward
  } Direction;


  /**
     The view_type enumeration lists the various possibilities
     for the current viewing mode that can be switched between.
     The default is PERSPECTIVE.
  **/
  typedef enum {
    PERSPECTIVE,
    ORTHO,
    ORTHO2D,
    IDENTITY,
    FRUSTUM
  } view_type;


  /** 
      The glsl_var enumeration lists the various variables the
      Camera class is capable of sending to the shader.
      The NumGlslVars variable is a sentinel value that is ignored
      by any functions that accept a glsl_var.
  **/
  typedef enum Uniforms {
    Begin = Object::End,
    TRANSLATION = Begin,
    ROTATION,
    VIEW,
    CTM,
    End
  } Uniform;
  

  Camera( const std::string &name, GLuint gShader,
	  float x = 0.0, float y = 0.0, float z = 0.0 );
  Camera( const std::string &name, GLuint gShader, vec3 &in );
  Camera( const std::string &name, GLuint gShader, vec4 &in );
  virtual ~Camera( void );
  
  /* Set positionals: forcibly move the camera */
  void X( const float &in, const bool &update = true );
  void Y( const float &in, const bool &update = true );
  void Z( const float &in, const bool &update = true );
  void pos( const float &x, const float &y, 
	    const float &z, const bool &update = true );
  void pos( const vec3 &in, const bool &update = true );
  void pos( const vec4 &in, const bool &update = true );
  
  /* Adjust positionals: move the camera by an offset */
  void dX( const float &by, const bool &update = true );
  void dY( const float &by, const bool &update = true );
  void dZ( const float &by, const bool &update = true );
  void dPos( const float &x, const float &y, 
	     const float &z );
  void dPos( const vec3 &by );
  void dPos( const vec4 &by );
  
  /* Field-of-View */
  void FOV( const float &fovy );
  float FOV( void ) const;
  void dFOV( const float &by );
  void changePerspective( const view_type &vType );
  void refreshPerspective( void );
  void viewport( size_t _X, size_t _Y,
		 size_t _width, size_t _height );

  /* Adjust the camera position with regards to its current vector */
  void sway( const float &by );
  void surge( const float &by );
  void heave( const float &by );
  
  /* Adjust the camera's current view angle */
  void pitch( const float &by, const bool &fixed = false );
  void yaw( const float &by, const bool &fixed = false );
  void roll( const float &by, const bool &fixed = false );
  
  /* Instruct the camera to automatically move. */
  void Move( const Camera::Direction &Dir );
  void Stop( const Camera::Direction &Dir );
  void Idle( void );
  void Accel( const vec3 &accel );

  /* Get Position */
  float X( void ) const;
  float Y( void ) const;
  float Z( void ) const;
  vec4 pos( void ) const;

  /* OpenGL Methods */
  virtual void Send( Object::UniformEnum which );
  void View( void );

  /* In case of emergency, ... */
  void resetRotation( void );

    
private:

  void adjustRotation( const mat4 &adjustment, const bool &fixed = false );
  void commonInit( void );

  /** The current view matrix (defaultly perspective) for this camera. **/
  mat4 view;

  /** The Current Transformation state for this Camera. **/
  TransCache ctm;

  /** The current viewing mode type. **/
  view_type currView;

  /** Current Speed of camera motion. **/
  GLfloat speed;

  /** Current Velocity of camera motion. **/
  vec3 velocity;

  /** Current Speed Capacity: (speed/MaxSpeed) **/
  GLfloat speed_cap;

  /** Maximum Acceleration Magnitude **/
  GLfloat MaxAccel;

  /** Maximum Speed **/
  GLfloat MaxSpeed;

  /** Friction. Should be less than MaxAccel. **/
  GLfloat FrictionMagnitude;

  /** Current aspect ratio for certain perspectives. **/
  GLfloat aspect;

  /** Current field-of-view angle for perspective view. **/
  GLfloat fovy;

  /** Camera's Drawbox Width and Height **/
  Angel::vec2 size;

  /** Camera's Drawbox X,Y Coordinate (Upper-Left Pixel) **/
  Angel::vec2 position; /* XPos and YPos */

  /** Booleans correlating to the different motion directions. **/
  bool Motion[ Camera::Direction_End ];
  
};

#endif
