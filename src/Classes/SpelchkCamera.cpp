/**
 * @file SpelchkCamera.cpp
 * @author Hoanh Nguyen
 * @date 2012-12-01
 * @brief Tiem Spelchk Camera Implementation
 * @details FIXME Documentation needed from Eric.
 */

#include "SpelchkCamera.hpp"

SpelchkCamera::SpelchkCamera( vec4 initialTranslationVector ) {
  _timeRef = 0;
  _initialTranslationVector = initialTranslationVector;
  reset();
}

SpelchkCamera::~SpelchkCamera() {
}

void SpelchkCamera::reset() {
  _projectionType = 0;
  _fovy = 45;
  
  _left = -1.0;
  _right = 1.0;
  _bottom = -1.0;
  _top = 1.0;
  _zNear = 0.1;
  _zFar = 20.0;
  
  _xDepth = 0.0;
  _yDepth = 0.0;
  _zDepth = 0.0;
  
  _xAngle = 0.0;
  _yAngle = 0.0;
  _zAngle = 0.0;
  
  _xHead = 0.0;
  _yHead = 0.0;
  _zHead = 0.0;
  
  _xHeadAngle = 0.0;
  _yHeadAngle = 0.0;
  _zHeadAngle = 0.0;
  
  _oldTranslationVector = _initialTranslationVector;
  _translationVector = _oldTranslationVector;

  _oldCameraPosition = _initialTranslationVector;
  _cameraPosition = _oldCameraPosition;

  calculateTranslationVector();
  calculateCameraPosition();
}

void SpelchkCamera::copyCamera(SpelchkCamera *camera) {
  camera->_projectionType = _projectionType;

  _xDepth = camera->_xDepth;
  _yDepth = camera->_yDepth;
  _zDepth = camera->_zDepth;

  _xAngle = camera->_xAngle;
  _yAngle = camera->_yAngle;
  _zAngle = camera->_zAngle;

  _oldTranslationVector = camera->_oldTranslationVector;
  _translationVector = camera->_translationVector;

  _oldCameraPosition = camera->_oldCameraPosition;
  _cameraPosition = camera->_cameraPosition;
}

mat4 SpelchkCamera::getProjectionMatrix() {
  switch ( _projectionType ) {
  case 1:
    return Ortho( _left, _right, _bottom, _top, _zNear, _zFar );
  case 2:
    return Frustum( _left, _right, _bottom, _top, _zNear, _zFar );
  default:
    return Perspective( _fovy, _aspect, _zNear, _zFar );
  }
}

mat4 SpelchkCamera::getModelViewMatrix() {
  _modelViewMatrix = RotateX( _xAngle ) * RotateY( _yAngle )
                     * RotateZ( _zAngle ) * RotateX( _xHeadAngle )
                     * RotateY( _yHeadAngle ) * Translate( _translationVector )
                     * RotateX( -_xHeadAngle ) * RotateY( -_yHeadAngle );
  return _modelViewMatrix;
}

vec4 SpelchkCamera::getTranslationVector() {
  return _translationVector;
}

mat4 SpelchkCamera::getRotationMatrix() {
  return RotateZ(-_zAngle) * RotateY(-_yAngle) * RotateX(-_xAngle);
}

vec4 SpelchkCamera::getCameraPosition() {
  return _cameraPosition;
}

void SpelchkCamera::calculateCameraPosition() {
  vec4 cameraDisplacement = getRotationMatrix() * vec4( _xDepth, _yDepth, _zDepth, 0.0 );
  _cameraPosition = (_oldCameraPosition + cameraDisplacement);
}

void SpelchkCamera::calculateTranslationVector() {
  // calculate displacement based on current angles (note rotations done in reverse order and negative to move model in opposite direction)
  vec4 calculateDisplacement = getRotationMatrix() * vec4( -_xDepth, -_yDepth, -_zDepth, 0.0 );
  _translationVector = (_oldTranslationVector + calculateDisplacement);
}

void SpelchkCamera::moveCamera( float xDepth, float yDepth, float zDepth ) {
  _oldTranslationVector = _translationVector;
  _oldCameraPosition = _cameraPosition;
  
  _xDepth = xDepth;
  _yDepth = yDepth;
  _zDepth = zDepth;
  
  calculateTranslationVector();
  calculateCameraPosition();
}

void SpelchkCamera::rotateCamera( float xAngle, float yAngle,
                                  float zAngle ) {
  _xAngle += xAngle;
  _yAngle += yAngle;
  _zAngle += zAngle;
  
  // Keep camera from flipping over
  if ( _xAngle > 90.0 ) {
    _xAngle = 90.0;
  } else if ( _xAngle < -90 ) {
    _xAngle = -90;
  }
  
  calculateTranslationVector();
}

void SpelchkCamera::setScreenSize( int width, int height ) {
  glViewport( 0, 0, width, height );
  _screenWidth = width;
  _screenHeight = height;
  _aspect = GLfloat( width ) / height;
}

void SpelchkCamera::setProjection( int projectionType ) {
  _projectionType = projectionType;
}

void SpelchkCamera::setLightMovementRef( GLuint ref ) {
  _timeRef = ref;
  
}

void SpelchkCamera::setLightMovementTime( float elapsed ) {
  if ( _timeRef != 0 )
  glUniform1f( _timeRef, elapsed );
}
void SpelchkCamera::headMovement( int usernum, double x, double y, double z ) {
  //mm to meters and cast to float
  
  calculateTranslationVector();
  getModelViewMatrix();
  
  vec4 originCentric = _modelViewMatrix
                       * vec4( x / 250.0, y / 250.0, z / 250.0, 1.0 );
  if ( originCentric.z != 0 ) {
    float ysin = originCentric.x / originCentric.z;
    float xcos = originCentric.y / originCentric.z;
    if ( ysin > 1 ) ysin = 1;
    else if ( ysin < -1 ) ysin = -1;
    if ( xcos > 1 ) xcos = 1;
    else if ( xcos < -1 ) xcos = -1;
    _yHeadAngle = -atan( ysin );
    _xHeadAngle = -atan( xcos );
  }
  
  moveCamera( _xHead - (float) (x / 250.0), _yHead - (float) (y / 250.0),
              _zHead + (float) (z / 250.0) );
  _xHead = (float) (x / 250.0);
  _yHead = (float) (y / 250.0);
  _zHead = -(float) (z / 250.0);
  
//	printf("%d - (%6.2f, %6.2f, %6.2f) ==> (%6.2f,%6.2f,%6.2f), yaw=%f, pitch=%f\n", usernum,x,y,z,_xHead, _yHead, _zHead, _xHeadAngle, _yHeadAngle);
}
