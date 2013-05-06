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
//#include "Engine.hpp"
//#include "Cameras.hpp"
//#include "Scene.hpp"
//#include "Screen.hpp"
//#include "glut_callbacks.h"
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include "RaytraceBuffer.hpp"

#define MINMACRO( XYZ, ABC ) if (min.XYZ > t.ABC.XYZ) min.XYZ = t.ABC.XYZ;
#define MAXMACRO( XYZ, ABC ) if (max.XYZ < t.ABC.XYZ) max.XYZ = t.ABC.XYZ;
#define MINTRIPLET( XYZ ) \
  MINMACRO( XYZ, a )    \
  MINMACRO( XYZ, b )    \
  MINMACRO( XYZ, c )

#define MAXTRIPLET( XYZ ) \
  MAXMACRO( XYZ, a )    \
  MAXMACRO( XYZ, b )    \
  MAXMACRO( XYZ, c )

class RayTracer
{
  public:
    RayTracer();
    boost::function<void(void)> display;
    void init(GLint shader);
    void generateScene(void);
    void addVec3ToVector(std::vector<GLfloat> *_vector, vec3 _vec3);
    void addTriangle( const vec3& a, const vec3& b, const vec3& c,
          const vec3& diffuse, const vec3& ambient, const vec3& specular,
          float shininess, float reflect, float refract);
    void pushTriangleDataToBuffer( std::vector<GLfloat> &dataBuffer,
        std::vector<triangle_t> &triangleBuffer,
        size_t start, size_t count );
    void pushDataToBuffer( std::vector<GLfloat> &dataBuffer );
    //void genereateScene(std::vector<Object*> objects);
    virtual ~RayTracer();
    void _display(void);
    void setMinMax( vec3 &min, vec3 &max, triangle_t &t );

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
    GLfloat *sphereCenterPoints, *sphereRadius, *sphereAmbient, *sphereDiffuse, *sphereSpecular, *sphereShininess, *sphereReflect, *sphereRefract;
    GLint numSpheres;

    GLint uNumOfTriangle;
    GLint uNumOfTriangleVectors;

    GLint uNumOfL2BoundingBoxes;
    GLint uNumOfL1BoundingBoxes;

    GLint uNumOfL2TrianglesBounded;

    GLint uNumberOfLights;

    GLint uNumOfBoundingBoxes;
    GLint uNumOfBoundingSpheres;
    GLint uNumOfTrianglesBounded;

    GLint uLightPositions;
    GLint uLightDiffuse;
    GLint uLightSpecular;

    int numberOfLights;
    GLfloat *lightPositions;
    GLfloat *lightDiffuse;
    GLfloat *lightSpecular;

    int numTriangles;

    int numOfL2BoundingBoxes;
    int numOfL1BoundingBoxes;
    int numOfBoundingBoxes;

    int numOfL2TrianglesBounded;

    std::vector<GLfloat> bufferData;
    std::vector<triangle_t> triangle_tData;

    int numOfTriangleVectors;

    GLint uDisplay;

    /** Rotation matrix uniform shader variable location **/
    GLuint uRotationMatrix;
    /** Handle to uniform that will contain the position of the Camera. **/
    GLint uCameraPosition;

};

#endif /* RAYTRACER_H_ */
