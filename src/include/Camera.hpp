/**
 @file Camera.hpp
 @author John Huston
 @authors John Huston, Nicholas StPierre, Chris Compton
 @date 2013-03-13
 @brief Header for the Camera class.
 **/
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

 move(), stop(), and idle() are provided to help the camera
 automatically move along the x, y, or z axes.
 **/
class Camera : public Object {
  
public:
  
  /**
   The Direction enumeration lists all of the possible directions
   the camera may travel in. 'BEGIN' and 'END' are special sentinel
   directions for the purposes of iteration, and are ignored by
   any functions that accept a Direction.
   **/
  typedef enum Direction {
    DIR_FORWARD,
    DIR_BACKWARD,
    DIR_LEFT,
    DIR_RIGHT,
    DIR_UP,
    DIR_DOWN,
    DIR_END,
    DIR_BEGIN = DIR_FORWARD
  } Direction;

  /**
   The ViewType enumeration lists the various possibilities
   for the current viewing mode that can be switched between.
   The default is PERSPECTIVE.
   **/
  typedef enum ViewType {
    PERSPECTIVE, ORTHO, ORTHO2D, IDENTITY, FRUSTUM
  } ViewType;

  /** 
   The glsl_var enumeration lists the various variables the
   Camera class is capable of sending to the shader.
   The NumGlslVars variable is a sentinel value that is ignored
   by any functions that accept a glsl_var.
   **/
  typedef enum Uniforms {
    BEGIN = Object::END, TRANSLATION = BEGIN, ROTATION, VIEW, CTM, END
  } Uniform;

  /**
   Initialization Constructor; sets the x,y,z coordinates explicitly.
   @param name The name of this Camera/Object.
   @param gShader A handle to this camera's associated shader object.
   @param x The initial x coordinate.
   @param y The initial y coordinate.
   @param z The initial z coordinate.
   **/
  Camera( const std::string &name, GLuint gShader, float x = 0.0, float y = 0.0,
          float z = 0.0 );

  /**
   Initialization Constructor, uses a vec3 as its initial coordinates.
   @param name The name of this Camera/Object.
   @param gShader A handle to this camera's associated shader object.
   @param in A vec3 representing the initial coordinates.
   **/
  Camera( const std::string &name, GLuint gShader, vec3 &in );

  /**
   Initialization Constructor, uses a vec4 as its initial coordinates.
   @param name The name of this Camera/Object.
   @param gShader A handle to this camera's associated shader object.
   @param in A vec4 representing the initial coordinates. The w component is ignored.
   **/
  Camera( const std::string &name, GLuint gShader, vec4 &in );

  /**
   Default destructor. Defined only to allow inheritance.
   **/
  virtual ~Camera( void );

  /* Set positionals: forcibly move the camera */
  /**
   Sets the x coordinate of the camera.
   @param in     The new x coordinate of the camera.
   @param update Whether or not to update the shader with the new coordinates.
   @return Void.
   **/
  void x( const float &in, const bool &update = true );
  /**
   Sets the y coordinate of the camera.
   @param in     The new y coordinate of the camera.
   @param update Whether or not to update the shader with the new coordinates.
   @return Void.
   **/
  void y( const float &in, const bool &update = true );
  /**
   Sets the z coordinate of the camera.
   @param in     The new z coordinate of the camera.
   @param update Whether or not to update the shader with the new coordinates.
   @return Void.
   **/
  void z( const float &in, const bool &update = true );

  /**
   Sets the absolute position of the camera.
   @param x The new x coordinate of the camera.
   @param y The new y coordinate of the camera.
   @param z The new z coordinate of the camera.
   @param update Whether or not to update the shader with the new coordinates.
   @return Void.
   **/
  void pos( const float &x, const float &y, const float &z, const bool &update =
                true );

  /**
   Sets the absolute position of the camera.
   @param in A vec3 containing the x, y, and z coordinates to set the camera to.
   @param update Whether or not to update the shader with the new coordinates.
   @return Void.
   **/
  void pos( const vec3 &in, const bool &update = true );

  /**
   Sets the absolute position of the camera.
   @param in A vec4 containing the x, y, and z coordinates to set the camera to. The w coordinate is ignored.
   @param update Whether or not to update the shader with the new coordinates.
   @return Void.
   **/
  void pos( const vec4 &in, const bool &update = true );

  /* Adjust positionals: move the camera by an offset */
  /**
   Moves the camera along the x axis.
   @param by The float value of the x-axis displacement.
   @param update A boolean indicating whether or not to update the shader.
   update defaults to true.
   @return void.
   **/
  void dX( const float &by, const bool &update = true );

  /**
   Moves the camera along the y axis.
   @param by The float value of the y-axis displacement.
   @param update A boolean indicating whether or not to update the shader.
   update defaults to true.
   @return Void.
   **/
  void dY( const float &by, const bool &update = true );

  /**
   Moves the camera along the z axis.
   @param by The float value of the z-axis displacement.
   @param update A boolean indicating whether or not to update the shader.
   update defaults to true.
   @return Void.
   **/
  void dZ( const float &by, const bool &update = true );

  /**
   Moves the camera along the x, y, and z axes.
   @param x the x-axis displacement.
   @param y the y-axis displacement.
   @param z the z-axis displacement.
   @return Void.
   **/
  void dPos( const float &x, const float &y, const float &z );

  /**
   Moves the camera along the x, y, and z axes.
   @param by A vec3 containing the x, y, and z axis displacements.
   @return Void.
   **/
  void dPos( const vec3 &by );

  /**
   Moves the camera along the x, y, and z axes.
   @param by A vec4 containing the x, y, and z axis displacements. The w component is ignored.
   @return Void.
   **/
  void dPos( const vec4 &by );

  /* Field-of-view */
  /**
   fieldOfView sets the current camera Field-of-view angle.
   This function will send the new perspective matrix to the shader.
   @param fovy The new field of view angle.
   @return Void.
   **/
  void fieldOfView( const float &fovy );
  /**
   fieldOfView() gets the current camera Field-of-view angle.
   @return A float that is the y axis viewing angle.
   **/
  float fieldOfView( void ) const;
  /**
   adjustFieldOfView adjusts the field of view angle up or down by an amount.
   @param by The float to adjust the fieldOfView angle by.
   @return Void.
   **/
  void adjustFieldOfView( const float &by );
  /**
   changePerspective changes the current perspective of the camera.
   @param vType Which perspective to use. see enum ViewType for possibilities.
   @return Void.
   **/
  void changePerspective( const ViewType &vType );
  /**
   refreshPerspective re-generates the current view/perspective matrix of the camera.
   This function should be called after physical or virtual (viewport) screen resizes.
   @return Void.
   **/
  void refreshPerspective( void );
  /**
   viewport instructs this camera what his expected drawing window will be.
   This allows the camera to generate his viewing matrices with the
   correct aspect ratio.
   @param _X The x coordinate of the lower-left corner of our viewport.
   @param _Y the y coordinate of the lower-left corner of our viewport.
   @param _width The width of our viewport.
   @param _height the height of our viewport.
   @return Void.
   **/
  void viewport( size_t _X, size_t _Y, size_t _width, size_t _height );

  /* Adjust the camera position with regards to its current vector */
  /**
   Adjusts the camera's x coordinate relative to its current position.
   Negative values move the camera left,
   and positive values move the camera right.
   @param by The float to adjust the x coordinate by.
   @return Void.
   **/
  void sway( const float &by );
  /**
   Adjusts the camera's z coordinate relative to its current position.
   Positive values move the camera forward,
   and negative values move the camera backward.
   Note that the camera uses model coordinates internally,
   so moving forward will increase the camera's z position negatively.
   @param by The float to adjust the z coordinate by.
   @return Void.
   **/
  void surge( const float &by );
  /**
   Adjusts the camera's y coordinate relative to its current position.
   Positive values move the camera up,
   and negative values move the camera down.
   @param by The float to adjust the y coordinate by.
   @return Void.
   **/
  void heave( const float &by );

  /* Adjust the camera's current view angle */
  /**
   pitch adjusts the x axis rotation; up/down look.
   A positive value represents looking up,
   while a negative value represents looking down.
   @param by A float, in degrees, to adjust the pitch by.
   @param fixed Should this rotation be fixed about the origin?
   @return Void.
   **/
  void pitch( const float &by, const bool &fixed = false );
  /**
   yaw adjusts the y axis rotation; left/right look.
   A positive value represents looking right,
   while a negative value represents looking left.
   @param by A float, in degrees, to adjust the yaw by.
   @param fixed Should this rotation be fixed about the origin?
   @return Void.
   **/
  void yaw( const float &by, const bool &fixed = false );
  /**
   roll adjusts the z axis rotation; tilt or lean left/right.
   A positive value represents leaning right,
   while a negative value represents leaning left.
   @param by A float, in degrees, to adjust the roll by.
   @param fixed Should this rotation be fixed about the origin?
   @return Void.
   **/
  void roll( const float &by, const bool &fixed = false );

  /* Instruct the camera to automatically move. */
  /**
   move instructs the camera to begin moving in the specified direction.
   @param Dir The direction in which to move.
   Can be any direction in the enumerated type Camera::Direction.
   @return Void.
   **/
  void move( const Camera::Direction &Dir );
  /**
   stop instructs the camera to stop moving in the specified direction.
   @param Dir The direction in which to stop moving.
   @return Void.
   **/
  void stop( const Camera::Direction &Dir );
  /**
   stopAll is a compilation of the 6 directions of stop functions.
   Intended for use in resetPosition, but can apply elsewhere if needed.
   @return Void.
   **/
  void stopAll( void );
  /**
   idle moves the camera forward in whichever directions it is
   configured to move in. Call it in the glut idle function.
   @return Void.
   **/
  void idle( void );
  /**
   accel takes an input vec2 which represents an acceleration,
   and applies it to the motion vectors with regards to
   the maximum acceleration and the maximum speed of the camera.
   @param accel The vec3 which represents the (x,y,z) acceleration, where x,y,z are [-1,1].
   @return Void.
   **/
  void accel( const vec3 &accel );

  /* Get Position */
  /**
   x() returns the current position of the camera in model coordinates.
   @return The current x coordinate of the camera in model coordinates.
   **/
  float x( void ) const;
  /**
   y() returns the current position of the camera in model coordinates.
   @return The current y coordinate of the camera in model coordinates.
   **/
  float y( void ) const;
  /**
   z() returns the current position of the camera in model coordinates.
   @return The current z coordinate of the camera in model coordinates.
   **/
  float z( void ) const;
  /**
   pos() gets the current camera position in model coordinates.
   @return A vec4 that represents the current camera coordinates.
   **/
  vec4 pos( void ) const;

  /* Get Velocity */
  vec3 dPos ( void ) const;

  /* Get directional vectors */
  vec4 forward ( void ) const;
  vec4 up ( void ) const;




  /* OpenGL Methods */
  /**
   send will send a glsl variable to the shader.
   @param which The parameter to send. Can be any from enum glsl_var.
   @return Void.
   **/
  virtual void send( Object::UniformEnum which );
  /**
   view will instruct OpenGL of the viewport we want, and then send all of our
   current matrices to the shader for rendering.
   @return Void.
   **/
  void view( void );

  /**
   resetRotation adjusts the camera's rotational state back to
   its default state (The Identity Matrix.)

   @return void.
   **/
  void resetRotation( void );

  /**
     resetPosition adjusts the camera's position back to
     its default state (The origin at (0,0,0))

     @return void.
  **/
  void resetPosition( void );

  void debug( void );
  
  void setSpeed(float num);
  
  void setMaxAcceleration(float num);
	 
  void setFriction(float num);
	 
  float getSpeed();
	 
  float getMaxAcceleration();
	 
  float getFriction();

  void setMaxSpeed(float num);

  float getMaxSpeed();

  void setCurrentView(int num);

private:
  
  /**
   adjustRotation is an internal function that rotates the camera.
   Technically, any transformation, not just a rotation, is possible.
   @param adjustment The 4x4 matrix to transform the CTM by.
   @param fixed Should this rotation be fixed about the origin?
   @return Void.
   **/
  void adjustRotation( const mat4 &adjustment, const bool &fixed = false );
  /**
   commonInit is a private function that initializes local object attributes.
   It should be called by all available constructors.
   @return Void.
   **/
  void commonInit( void );

  /** The current view matrix (defaultly perspective) for this camera. **/
  mat4 _view;

  /** The Current Transformation state for this Camera. **/
  TransCache _ctm;

  /** The current viewing mode type. **/
  ViewType _currentView;

  /** Current Speed of camera motion. **/
  GLfloat _speed;

  /** Current Velocity of camera motion. **/
  vec3 _velocity;

  /** Current Speed Capacity: (speed/MaxSpeed) **/
  GLfloat _speed_cap;

  /** Maximum Acceleration Magnitude **/
  GLfloat _maxAccel;

  /** Maximum Speed **/
  GLfloat _maxSpeed;

  /** Friction. Should be less than MaxAccel. **/
  GLfloat _frictionMagnitude;

  /** Current aspect ratio for certain perspectives. **/
  GLfloat _aspectRatio;

  /** Current field-of-view angle for perspective view. **/
  GLfloat _fovy;

  /** Camera's Drawbox Width and Height **/
  Angel::vec2 _viewportSize;

  /** Camera's Drawbox x,y Coordinate (Upper-Left Pixel) **/
  Angel::vec2 _viewportPosition; // XPos and YPos
  
  /** Booleans correlating to the different motion directions. **/
  bool _motion[Camera::DIR_END];
  
};

#endif
