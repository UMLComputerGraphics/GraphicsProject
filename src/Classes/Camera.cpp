/**
 @file Camera.cpp
 @author John Huston
 @authors John Huston, Nicholas StPierre, Chris Compton
 @date 2012-12-20
 @brief Implementation for the Camera class.
 **/

#include <stdexcept>
#include <iostream>

#include "mat.hpp"
#include "vec.hpp"
#include "Camera.hpp"
#include "globals.h" //Math constants and macros (SQRT2, POW5)
#include "Timer.hpp" //Global timer to sync with framerate.
#include "Util.hpp"
using namespace Angel;

void Camera::commonInit( void ) {

  gprint( PRINT_INFO, " -- Extending Object (%s) -- \n", _name.c_str() );

  gprint( PRINT_DEBUG, "Extending Uniforms Array to %d entries.\n", Camera::END );
  _handles.resize( Camera::END, -1 );
  
  /* Default Variable Links */
  link( Camera::TRANSLATION, "T" );
  link( Camera::ROTATION, "R" );
  link( Camera::VIEW, "P" );
  link( Camera::CTM, "CTM" );
  
  for ( size_t i = (size_t) Camera::DIR_BEGIN; i != (size_t) DIR_END; ++i )
    _motion[i] = false;
  
  _speed = 0;
  _speed_cap = 0;
  _maxAccel = 500;
  _maxSpeed = 2000;
  _frictionMagnitude = 50;
  _aspectRatio = 1;
  _currentView = PERSPECTIVE;
  _fovy = 45.0;

}

Camera::Camera( const std::string &name, GLuint gShader, float x, float y,
                float z ) :
    Object( name, gShader ) {
  commonInit();
  this->pos( x, y, z, false );
}

Camera::Camera( const std::string &name, GLuint gShader, vec3 &in ) :
    Object( name, gShader ) {
  commonInit();
  this->pos( in, false );
}

Camera::Camera( const std::string &name, GLuint gShader, vec4 &in ) :
    Object( name, gShader ) {
  commonInit();
  this->pos( in, false );
}

Camera::~Camera( void ) {
  
}

void Camera::x( const float &in, const bool &update ) {
  
  _ctm._offset.setX( -in );
  _trans._offset.setX( in );
  if ( update ) {
    _ctm.calcCTM();
    _trans.calcCTM();
    send( TRANSLATION );
  }
}

void Camera::y( const float &in, const bool &update ) {
  _ctm._offset.setY( -in );
  _trans._offset.setY( in );
  if ( update ) {
    _ctm.calcCTM();
    _trans.calcCTM();
    send( TRANSLATION );
  }
}

void Camera::z( const float &in, const bool &update ) {
  _ctm._offset.setZ( -in );
  _trans._offset.setZ( in );
  if ( update ) {
    _trans.calcCTM();
    _ctm.calcCTM();
    send( TRANSLATION );
  }
}

void Camera::pos( const float &newX, const float &newY, const float &newZ,
                  const bool &update ) {
  x( newX, false );
  y( newY, false );
  z( newZ, false );
  if ( update ) {
    _trans.calcCTM();
    _ctm.calcCTM();
    send( TRANSLATION );
  }
}

void Camera::pos( const vec4 &in, const bool &update ) {
  this->pos( in.x, in.y, in.z, update );
}

void Camera::pos( const vec3 &in, const bool &update ) {
  this->pos( in.x, in.y, in.z, update );
}

void Camera::dX( const float &by, const bool &update ) {
  x( x() + by, update );
}

void Camera::dY( const float &by, const bool &update ) {
  y( y() + by, update );
}

void Camera::dZ( const float &by, const bool &update ) {
  z( z() + by, update );
}

void Camera::dPos( const float &x, const float &y, const float &z ) {
  dX( x, false );
  dY( y, false );
  dZ( z, false );
  _trans.calcCTM();
  _ctm.calcCTM();
  send( TRANSLATION );
}

void Camera::dPos( const vec3 &by ) {
  dPos( by.x, by.y, by.z );
}

void Camera::dPos( const vec4 &by ) {
  dPos( by.x, by.y, by.z );
}

void Camera::adjustRotation( const mat4 &adjustment, const bool &fixed ) {
  
  /*
   By default, the 'order' bool represents the PREMULT behavior.
   However, if the fixed bool is present, toggle the order bool.
   This stealthily inserts this _rotation "first",
   So that it is applied before any other rotations.
   */
  bool order = POSTMULT;
  if ( fixed ) order = !order;
  
  // Apply our rotational adjustment to the camera.
  // Unintuitively, we need to adjust the Orbit.
  _ctm._orbit.adjust( adjustment, order );
  
  /*
   Next, our Camera may have a physical object whose
   Rotations need to be calculated as well.
   We need to apply the OPPOSITE rotations,
   So Transpose the Adjustment to obtain that.
   */
  _trans._rotation.adjust( transpose( adjustment ), !order );
  
  // Update our state.
  _trans.calcCTM();
  _ctm.calcCTM();
  send( ROTATION );
}

/** 
 ROTATE_OFFSET is a macro which is used to normalize
 the six camera motion directions with respect to the
 current camera _rotation. It is used in heave(), sway() and surge().
 @param V a vec4 representing the movement _offset vector.
 @return A rotated vec4.
 **/
#define ROTATE_OFFSET(V) (V * _ctm._orbit.matrix())

void Camera::sway( const float &by ) {
  dPos( ROTATE_OFFSET(vec4(by,0,0,0)) );
}

void Camera::surge( const float &by ) {
  dPos( ROTATE_OFFSET(vec4(0,0,-by,0)) );
}

void Camera::heave( const float &by ) {
  dPos( ROTATE_OFFSET(vec4(0,by,0,0)) );
}

void Camera::pitch( const float &by, const bool &fixed ) {
  /*
   Since negative values are interpreted as pitching down,
   We leave the input uninverted, because a negative _rotation
   about the x axis rotates counter-clockwise (looking right),
   and clockwise (looking left), which achieves the effect of
   looking 'down'.
   */
  adjustRotation( RotateX( -by ), fixed );
}

void Camera::yaw( const float &by, const bool &fixed ) {
  /*
   Since a positive 'by' should represent looking right,
   we invert the _rotation because rotating by a positive value
   will rotate right, which simulates looking left.
   Therefore, invert.
   */
  adjustRotation( RotateY( by ), fixed );
}

void Camera::roll( const float &by, const bool &fixed ) {
  adjustRotation( RotateZ( by ), fixed );
}

void Camera::accel( const vec3 &raw_accel ) {
  
  /*
   This scale factor is the cumulation of several scaling factors.
   (A) (MaxAccel/SQRT3)
   scales the vector so that it has a magnitude
   Between -MAX_ACCEL and MAX_ACCEL.

   (B) (1-POW5(speed_cap))
   scales the vector with relation to the
   current speed. This effectively models "Drag".
   The closer we are to maximum speed, this scaling factor
   Will approach 0, so that if we are at MAX SPEED,
   our acceleration will be scaled to (nearly) zero.

   (C) (tick.scale())
   This scales the amount of acceleration by an amount [0,1]
   so that if we are drawing many frames, we'll apply less acceleration.
   If we are drawing not so many, we'll apply more.
   This should keep the animation relatively consistent across platforms.
   */

  float Scale = (_maxAccel / SQRT3) * (1 - POW5(_speed_cap)) * (tick.scale());
  vec3 accel = raw_accel * Scale;
  
  if ( DEBUG_MOTION ) {
    fprintf( stderr, "Accel(); raw_accel = (%f,%f,%f)\n", raw_accel.x,
             raw_accel.y, raw_accel.z );
    fprintf(
        stderr,
        "Accel(); Scale = (MaxAccel/SQRT3) * (1-POW5(speed_cap)) * Tick.Scale()\n" );
    fprintf( stderr, "Accel(); Scale = (%f/%f) * (%f) * (%f)\n", _maxAccel,
             SQRT3, (1 - POW5(_speed_cap)), tick.scale() );
    fprintf( stderr, "Accel(); Scale = %f\n", Scale );
    fprintf( stderr, "Accle(); accel = raw_accel * Scale = (%f,%f,%f)\n",
             accel.x, accel.y, accel.z );
  }
  
  //The acceleration is finally applied to the velocity vector.
  _velocity += accel;
  
  //speed and speed_cap must now be recalculated.
  _speed_cap = (_speed = length( _velocity )) / _maxSpeed;
  
  if ( DEBUG_MOTION )
    fprintf( stderr, "Applied Acceleration to Velocity, Is now: (%f,%f,%f)\n",
             _velocity.x, _velocity.y, _velocity.z );
}

void Camera::move( const Camera::Direction &Dir ) {
  _motion[Dir] = true;
}

void Camera::stop( const Camera::Direction &Dir ) {
  _motion[Dir] = false;
}

void Camera::stopAll( void ) {

	// This function is a compilation of the 6 directions of stop functions
	// intended for use in resetPosition, but can apply elsewhere if needed

	// Putting the brakes on acceleration
	_velocity = vec3( 0, 0, 0 );

	// Halting all movement input temporarily
	if ( _motion[Camera::DIR_FORWARD] ) stop( Camera::DIR_FORWARD );
	if ( _motion[Camera::DIR_BACKWARD] ) stop( Camera::DIR_BACKWARD );
    if ( _motion[Camera::DIR_RIGHT] ) stop( Camera::DIR_RIGHT );
	if ( _motion[Camera::DIR_LEFT] ) stop( Camera::DIR_LEFT );
	if ( _motion[Camera::DIR_UP] ) stop( Camera::DIR_UP );
	if ( _motion[Camera::DIR_DOWN] ) stop( Camera::DIR_DOWN );

}

void Camera::idle( void ) {
  
  /* Apply the automated motion instructions, if any --
   These are primarily from the keyboard. */
  if ( _motion[Camera::DIR_FORWARD] ) accel( vec3( 0, 0, 1 ) );
  if ( _motion[Camera::DIR_BACKWARD] ) accel( vec3( 0, 0, -1 ) );
  if ( _motion[Camera::DIR_RIGHT] ) accel( vec3( 0, 1, 0 ) );
  if ( _motion[Camera::DIR_LEFT] ) accel( vec3( 0, -1, 0 ) );
  if ( _motion[Camera::DIR_UP] ) accel( vec3( 1, 0, 0 ) );
  if ( _motion[Camera::DIR_DOWN] ) accel( vec3( -1, 0, 0 ) );
  
  if ( _speed ) {
    /* Apply the velocity vectors computed from accel,
     which includes instructions from keyboard and the Balance Board. */
    /* 1/20000 is a magic constant which converts our velocity units
     into model units. */
    /* tick.scale() helps keep animation speed consistent
     between different hardware. */
    float UnitScale = (1.0 / 20000.0);
    float Scale = tick.scale() * UnitScale;
    
    if ( DEBUG_MOTION )
      fprintf( stderr, "Applying Translation: + (%f,%f,%f)\n",
               _velocity.x * Scale, _velocity.y * Scale, _velocity.z * Scale );
    
    heave( _velocity.x * Scale );
    sway( _velocity.y * Scale );
    surge( _velocity.z * Scale );
    
    // Friction Calculations
    if ( _speed < (_frictionMagnitude * tick.scale()) ) {
      if ( DEBUG_MOTION )
        fprintf( stderr, "Friction has stopped all movement.\n" );
      _velocity = vec3( 0, 0, 0 );
      _speed = 0;
      _speed_cap = 0;
    } else {
      // Friction is a vector that is the opposite of velocity.
      vec3 frictionVec = -_velocity;
      /* By dividing friction by (speed/FrictionMagnitude), 
       we guarantee that the magnitude is FrictionMagnitude. */
      frictionVec = frictionVec / (_speed / _frictionMagnitude);
      frictionVec *= tick.scale();
      
      if ( DEBUG_MOTION )
        fprintf( stderr, "Applying friction to Velocity: + (%f,%f,%f)\n",
                 frictionVec.x, frictionVec.y, frictionVec.z );
      _velocity += frictionVec;
      _speed = length( _velocity );
      _speed_cap = _speed / _maxSpeed;
    }
  }
}

float Camera::x( void ) const {
  return -_ctm._offset.matrix()[0][3];
}

float Camera::y( void ) const {
  return -_ctm._offset.matrix()[1][3];
}

float Camera::z( void ) const {
  return -_ctm._offset.matrix()[2][3];
}

vec4 Camera::pos( void ) const {
  return -1.0*vec4( x(), y(), z(), 1.0 );
}

float Camera::fieldOfView( void ) const {
  return _fovy;
}

void Camera::fieldOfView( const float &in ) {
  _fovy = in;
  if ( _currentView == Camera::PERSPECTIVE ) refreshPerspective();
}

void Camera::changePerspective( const ViewType &vType ) {
  
  _currentView = vType;
  refreshPerspective();
  
}

void Camera::refreshPerspective( void ) {
  
  // Some constants. For your pleasure.
  static const GLfloat zNear = 0.001;
  static const GLfloat zFar = 100.0;
  
  switch ( _currentView ) {
  case PERSPECTIVE:
    _view = Perspective( _fovy, _aspectRatio, zNear, zFar );
    break;
  case ORTHO:
    _view = Ortho( -1.0, 1.0, -1.0, 1.0, zNear, zFar );
    break;
  case ORTHO2D:
    _view = Ortho2D( -1.0, 1.0, -1.0, 1.0 );
    break;
  case FRUSTUM:
    _view = Frustum( -1.0, 1.0, -1.0, 1.0, zNear, zFar );
    break;
  case IDENTITY:
  default:
    _view = mat4( GLuint( 1.0 ) );
    break;
  }
}

void Camera::adjustFieldOfView( const float &by ) {
  fieldOfView( fieldOfView() + by );
}

void Camera::viewport( size_t _X, size_t _Y, size_t _Width, size_t _Height ) {
  this->_viewportPosition = Angel::vec2( _X, _Y );
  this->_viewportSize = Angel::vec2( _Width, _Height );
  this->_aspectRatio = (double) _Width / (double) _Height;
  refreshPerspective();
}

void Camera::send( Object::UniformEnum which ) {
  
  switch ( which ) {
  case TRANSLATION:
    if ( _handles[which] != -1 )
    glUniformMatrix4fv( _handles[which], 1, GL_TRUE, _ctm._offset.matrix() );
    send( CTM );
    break;
  case ROTATION:
    if ( _handles[which] != -1 )
    glUniformMatrix4fv( _handles[which], 1, GL_TRUE, _ctm._orbit.matrix() );
    send( CTM );
    break;
  case VIEW:
    if ( _handles[which] != -1 )
    glUniformMatrix4fv( _handles[which], 1, GL_TRUE, _view );
    send( CTM );
    break;
  case CTM:
    _ctm.calcCTM( POSTMULT );
    if ( _handles[which] != -1 )
    glUniformMatrix4fv( _handles[which], 1, GL_TRUE, _ctm.otm() );
    break;
  default:
    // If we don't know which variable this is,
    // See if our parent method knows.
    Object::send( which );
    break;
  }

  if (glGetError()) {
    gprint( PRINT_ERROR, "Camera::send() failed for %s[%u], handle was %u\n",
	    name().c_str(), which, _handles[which] );
  }
}

void Camera::view( void ) {
  
  glViewport( _viewportPosition.x, _viewportPosition.y, _viewportSize.x, _viewportSize.y );
  /* send all of our matrices, who knows what the shader's gonna do with 'em */
  send( TRANSLATION );
  send( ROTATION );
  send( VIEW );
  send( CTM );
  
}

void Camera::resetRotation( void ) {
  
  // The transpose of any _rotation is its inverse.
  // Thus, this resets the rotational matrix.
  this->_ctm._orbit.adjust( transpose( this->_ctm._orbit.matrix() ) );
  
}


/**
   dPos returns the camera's velocity.
   @return The current velocity of the camera.
**/
vec3 Camera::dPos( void ) const { return _velocity; }


/**
   forward returns a vector pointing in the same direction as the camera
   @return a vector pointing forwards, relative to the current camera rotation
 **/
vec4 Camera::forward ( void ) const { return vec4( 0.0, 0.0, 1.0, 1.0 ) * this->_ctm._orbit.matrix() ; }


/**
   up returns a vector pointing in the same direction as the camera
   @return a vector pointing up, relative to the current camera rotation
 **/
vec4 Camera::up ( void ) const { return vec4( 0.0, 1.0, 0.0, 1.0 ) * this->_ctm._orbit.matrix() ; }


void Camera::resetPosition( void ) {

  // This function is here to reset position back to (0,0,0)
  // Before doing so, it stops movement in order to prevent weirdness, then reenables it

  // This part of the function would reset the rotation, but it is broken and does nothing.
  //resetRotation();

  stopAll();
  pos(0,0,0);

}

void Camera::debug( void ) {

  std::cerr << "T{\n" << _ctm._offset.matrix() << "} " << _handles[TRANSLATION] << "\n";
  std::cerr << "R{\n" << _ctm._orbit.matrix() << "} " << _handles[ROTATION] << "\n";
  std::cerr << "P{\n" << _view << "}\n";
  std::cerr << "CTM{\n" << _ctm.otm() << "}\n";

}

void Camera::setSpeed(float num)
{
	_speed = (GLfloat) num;
}
void Camera::setMaxAcceleration(float num)
{
	_maxAccel = (GLfloat) num;
}
void Camera::setFriction(float num)
{
	_frictionMagnitude = (GLfloat) num;
}
	float Camera::getSpeed()
{
	return (float) _speed;
}
float Camera::getMaxAcceleration()
{
	return (float) _maxAccel;
}
float Camera::getFriction()
{
	return (float) _frictionMagnitude;
}

void Camera::setMaxSpeed(float num)
{
    _maxSpeed = (GLfloat)num;
}

float Camera::getMaxSpeed()
{
    return _maxSpeed;
}

void Camera::setCurrentView(int num)
{
    switch(num)
    {
        case 0:
            changePerspective(PERSPECTIVE);
            break;
        case 1:
            changePerspective(ORTHO);
            break;
        case 2:
            changePerspective(ORTHO2D);
            break;
        case 3:
            changePerspective(IDENTITY);
            break;
        case 4:
            changePerspective(FRUSTUM);
            break;
   }
}
