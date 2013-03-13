/*
 * Camera.h
 *
 *  Created on: Dec 1, 2012
 *      Author: Hoanh Nguyen
 */

#ifndef CAMERA_H_
#define CAMERA_H_

#include <cmath>
#include "mat.hpp"
#include "vec.hpp"
using namespace Angel;

class Camera2 {
  private:
    int projectionType;

    // Projection transformation parameters
    GLfloat fovy;  // Field-of-view in Y direction angle (in degrees)
    GLfloat aspect;       // Viewport aspect ratio

    GLfloat left, right;
    GLfloat bottom, top;
    GLfloat zNear, zFar;

    GLuint timeRef;

    int screenWidth, screenHeight;

    GLfloat xDepth, yDepth, zDepth;
    GLfloat xAngle, yAngle, zAngle;
    GLfloat xHead, yHead, zHead;
    float xHeadStart, yHeadStart, zHeadStart;
	GLfloat xHeadAngle, yHeadAngle, zHeadAngle;

    vec4 initialTranslationVector;
    vec4 translationVector;
    vec4 oldTranslationVector;

    mat4 modelViewMatrix;

    int inboundHeadData;
    vec4 initialHeadPosition;

    void calculateTranslationVector();

  public:
    Camera2(vec4 initialTranslationVector);
    virtual ~Camera2();

    mat4 getProjectionMatrix();
    mat4 getModelViewMatrix();
    vec4 getTranslationVector();

    // Displaces camera from its current location
    void moveCamera(float xDepth, float yDepth, float zDepth);

    // Rotate camera from its current orientation
    void rotateCamera(float xAngle, float yAngle, float zAngle);

    void setScreenSize(int width, int height);
    void setProjection(int projectionType);

    void reset();

    void setLightMovementRef(GLuint ref);
    void setLightMovementTime(float elapsed);

    void getReadyForZero(int usernum);
    void headMovement(int usernum, double x, double y, double z);
};

#endif /* CAMERA_H_ */
