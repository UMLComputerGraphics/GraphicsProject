/*
 * RayTracer.h
 *
 *  Created on: Apr 29, 2013
 *      Author: eric
 */
#ifndef RAYTRACER_H_
#define RAYTRACER_H_

#include <string>
#include <cmath>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <vector>
#include "Util.hpp"
//#include "Cameras.hpp"
//#include "Scene.hpp"
//#include "Screen.hpp"
//#include "glut_callbacks.h"
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include "RaytraceBuffer.hpp"

class RayTracer
{
  public:
    RayTracer();
    void _display(void);
    void init(GLint shader);
    void generateScene(void);
    void addVec3ToVector(std::vector<GLfloat> *_vector, vec3 _vec3);
    void setMinMax(vec3 *min, vec3 *max, vec3 v);
    void addTriangle( const vec3& a, const vec3& b, const vec3& c,
          const vec3& diffuse, const vec3& ambient, const vec3& specular,
          float shininess, float reflect, float refract);
    void pushDataToBuffer();
    //void genereateScene(std::vector<Object*> objects);
    virtual
    ~RayTracer();
  private:

    int frameCount;
    float previousTime;

    GLint program;

    GLint vRayPosition;

    GLint uSphereCenterPoints;
    GLint uSphereRadius;
    GLint uSphereDiffuse;
    GLint uSphereAmbient;
    GLint uSphereSpecular;
    GLint uSphereShininess;
    GLint uSphereReflect;
    GLint uSphereRefract;
    GLint uNumOfSpheres;

    GLint uNumOfTriangle;
    GLint uNumOfTriangleVectors;

    GLint uNumOfBoundingBoxes;
    GLint uNumOfBoundingSpheres;
    GLint uNumOfTrianglesBounded;

    GLint uLightPositions;
    GLint uLightDiffuse;
    GLint uLightSpecular;

    GLfloat *lightPositions;
    GLfloat *lightDiffuse;
    GLfloat *lightSpecular;

    int numTriangles;

    int numOfBoundingSpheres;
    std::vector< GLfloat > bufferData;

    int numOfTriangleVectors;
    int numOfTrianglesBounded;

    int numOfBoundingBoxes;

    GLint uDisplay;

    /** Rotation matrix uniform shader variable location **/
    GLuint uRotationMatrix;
    /** Handle to uniform that will contain the position of the Camera. **/
    GLint uCameraPosition;

};

#endif /* RAYTRACER_H_ */
