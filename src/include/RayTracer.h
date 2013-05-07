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
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/thread/mutex.hpp>
#include "RaytraceBuffer.hpp"
#include "mat.hpp"
#include "Util.hpp"

class RayTracer {
  /* These need to be declared prior to the boost-threading so they can be initialized before the boost thread forks off. */
 private:
  GLfloat *_lightPositions;
  GLfloat *_lightDiffuse;
  GLfloat *_lightSpecular;
  
 public:
  RayTracer();
  boost::function<void(void)> display;
  void init(GLuint shader);
  void generateScene(void);
  void addVec3ToVector(std::vector<GLfloat> *_vector, vec3 _vec3);
  void setMinMax( vec3 &min, vec3 &max, triangle_t &t );
  void addTriangle( const vec3& a, const vec3& b, const vec3& c,
		    const vec3& diffuse, const vec3& ambient, const vec3& specular,
		    float shininess, float reflect, float refract,
		    vec3 normal = vec3(-INFINITY,-INFINITY,-INFINITY) );
  void addTransformation( const Angel::mat4 &mat );

  void pushTriangleDataToBuffer( std::vector<GLfloat> &dataBuffer,
				 std::vector<triangle_t> &triangleBuffer,
				 size_t start, size_t count );
  void pushDataToBuffer();
  void legacySceneGen( void );
  virtual ~RayTracer();
  void _display(void);
  void thisDateIsOver(void);
  void lightFlicker();
  void idleHandsSpendTimeWithTheTextureBuffer();

 private:
  
  GLuint _program;
  GLint _vRayPosition;
  //
  GLint _uNumOfSpheres;    
  GLint _uNumOfTriangle;
  GLint _uNumOfTriangleVectors;
  GLint _uNumOfL2BoundingBoxes;
  GLint _uNumOfL1BoundingBoxes;
  GLint _uNumOfL2TrianglesBounded;
  GLint _uNumberOfLights;
  GLint _uLightPositions;
  GLint _uLightDiffuse;
  GLint _uLightSpecular;
  GLint _uNumSGTransformations;
  GLint _uSceneGraphTransformations;
  //
  int _numberOfLights;
  int _numTriangles;
  int _numSpheres;
  int _numOfL2BoundingBoxes;
  int _numOfL1BoundingBoxes;
  int _numOfL2TrianglesBounded;
  
  std::vector<GLfloat> _bufferData;
  std::vector<triangle_t> _triangleData;
  std::vector<Angel::mat4> _sceneData;
  
  int _numOfTriangleVectors;

  bool _extinguish;

  //rebuffering magic
  bool _readytorebuffer;
};

#endif /* RAYTRACER_H_ */
