/*
 * Camera.cpp
 *
 *  Created on: Dec 1, 2012
 *      Author: Hoanh Nguyen
 */

#include "SpelchkCamera.hpp"

SpelchkCamera::SpelchkCamera(vec4 _initialTranslationVector) {
  timeRef = 0;
  initialTranslationVector = _initialTranslationVector;
  reset();
}

SpelchkCamera::~SpelchkCamera() {
}

void SpelchkCamera::reset() {
  projectionType = 0;
  fovy = 45;

  left = -1.0;
  right = 1.0;
  bottom = -1.0;
  top = 1.0;
  zNear = 0.1;
  zFar = 20.0;

  xDepth = 0.0;
  yDepth = 0.0;
  zDepth = 0.0;

  xAngle = 0.0;
  yAngle = 0.0;
  zAngle = 0.0;

  xHead = 0.0;
  yHead = 0.0;
  zHead = 0.0;

  xHeadAngle = 0.0;
  yHeadAngle = 0.0;
  zHeadAngle = 0.0;

  oldTranslationVector = initialTranslationVector;
  translationVector = oldTranslationVector;
  calculateTranslationVector();
}

mat4 SpelchkCamera::getProjectionMatrix() {
  switch (projectionType) {
    case 1:
      return Ortho(left, right, bottom, top, zNear, zFar);
    case 2:
      return Frustum(left, right, bottom, top, zNear, zFar);
    default:
      return Perspective(fovy, aspect, zNear, zFar);
  }
}

mat4 SpelchkCamera::getModelViewMatrix() {
  modelViewMatrix = RotateX(xAngle) * RotateY(yAngle) * RotateZ(zAngle) * RotateX(xHeadAngle) * RotateY(yHeadAngle) * Translate(translationVector) * RotateX(-xHeadAngle) * RotateY(-yHeadAngle);
  return modelViewMatrix;
}

vec4 SpelchkCamera::getTranslationVector() {
  return translationVector;
}

void SpelchkCamera::calculateTranslationVector() {
  // calculate displacement based on current angles (note rotations done in reverse order and negative to move model in opposite direction)

  vec4 calculateDisplacement =  RotateZ(-zAngle) * RotateY(-yAngle) * RotateX(-xAngle) * vec4(xDepth, yDepth, -zDepth, 0.0);
  translationVector = (oldTranslationVector + calculateDisplacement);
}

void SpelchkCamera::moveCamera(float _xDepth, float _yDepth, float _zDepth) {
  oldTranslationVector = translationVector;

  xDepth = _xDepth;
  yDepth = _yDepth;
  zDepth = _zDepth;

  calculateTranslationVector();
}

void SpelchkCamera::rotateCamera(float _xAngle, float _yAngle, float _zAngle) {
  xAngle += _xAngle;
  yAngle += _yAngle;
  zAngle += _zAngle;

  // Keep camera from flipping over
  if(xAngle > 90.0) {
    xAngle = 90.0;
  } else if(xAngle < -90) {
    xAngle = -90;
  }

  calculateTranslationVector();
}

void SpelchkCamera::setScreenSize(int width, int height) {
  glViewport(0, 0, width, height);
  screenWidth = width;
  screenHeight = height;
  aspect = GLfloat(width) / height;
}

void SpelchkCamera::setProjection(int _projectionType) {
  projectionType = _projectionType;
}

void SpelchkCamera::setLightMovementRef(GLuint ref)
{
	timeRef = ref;

}

void SpelchkCamera::setLightMovementTime(float elapsed)
{
	if (timeRef != 0)
	  glUniform1f(timeRef, elapsed);
}
void SpelchkCamera::headMovement(int usernum, double x, double y, double z)
{
	//mm to meters and cast to float

	calculateTranslationVector();
	getModelViewMatrix();

	vec4 originCentric = modelViewMatrix * vec4(x/250.0,y/250.0,z/250.0,1.0);
	if (originCentric.z != 0)
	{
		float ysin = originCentric.x/originCentric.z;
		float xcos = originCentric.y/originCentric.z;
		if (ysin > 1)
			ysin = 1;
		else if (ysin < -1)
			ysin = -1;
		if (xcos > 1)
			xcos = 1;
		else if (xcos < -1)
			xcos = -1;
		yHeadAngle = -atan(ysin);
		xHeadAngle = -atan(xcos);
	}

	moveCamera(xHead-(float)(x/250.0), yHead -(float)(y/250.0), zHead +(float)(z/250.0));
	xHead = (float)(x/250.0);
	yHead = (float)(y/250.0);
	zHead = -(float)(z/250.0);

//	printf("%d - (%6.2f, %6.2f, %6.2f) ==> (%6.2f,%6.2f,%6.2f), yaw=%f, pitch=%f\n", usernum,x,y,z,xHead, yHead, zHead, xHeadAngle, yHeadAngle);
}
