/*
 * RayTracer.cpp
 *
 *  Created on: Apr 29, 2013
 *      Author: eric
 */

#include "RayTracer.h"
#include "Engine.hpp"

RayTracer::RayTracer() :
  display(boost::bind(&RayTracer::_display,this))
{
   lightPositions = (GLfloat*)malloc(sizeof(GLfloat)*3);
   lightPositions[0]=lightPositions[1]=1.0;
   lightPositions[2]=10.0;
   lightDiffuse = (GLfloat*)malloc(sizeof(GLfloat)*3);
   lightSpecular = (GLfloat*)malloc(sizeof(GLfloat)*3);
   lightDiffuse[0]=lightDiffuse[1]=lightDiffuse[2]=lightSpecular[0]=lightSpecular[1]=lightSpecular[2]=1.0;
  // TODO Auto-generated constructor stub

}

RayTracer::~RayTracer()
{
  if (lightPositions)free(lightPositions);
  if(lightDiffuse)free(lightDiffuse);
  if(lightSpecular)free(lightSpecular);
}

/**
 * Handle the re-display of the scene.
 */
void RayTracer::_display( void ) {
#ifndef __APPLE__
  Engine &engie = *Engine::instance();
  Camera *cammy = engie.cams()->active();

  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  engie.switchCamera( cammy );
  engie.switchShader( program );

  tick.sendTime();

  int numSpheres = 0;
  glUniform1i( uNumOfSpheres, numSpheres );
  /*glUniform3fv( uSphereCenterPoints, numSpheres, sphereCenterPoints );
  glUniform1fv( uSphereRadius, numSpheres, sphereRadius );
  glUniform3fv( uSphereDiffuse, numSpheres, sphereDiffuse );
  glUniform3fv( uSphereAmbient, numSpheres, sphereAmbient );
  glUniform3fv( uSphereSpecular, numSpheres, sphereSpecular );
  glUniform1fv( uSphereShininess, numSpheres, sphereShininess );
  glUniform1fv( uSphereReflect, numSpheres, sphereReflect );
  glUniform1fv( uSphereRefract, numSpheres, sphereRefract );*/

  glUniform1i( uNumOfTriangle, numTriangles );
  glUniform1i( uNumOfTriangleVectors, numOfTriangleVectors );

  glUniform1i( uNumOfBoundingBoxes, numOfBoundingBoxes );

  glUniform3fv( uLightPositions, 1, lightPositions );
  glUniform3fv( uLightDiffuse, 1, lightDiffuse );
  glUniform3fv( uLightSpecular, 1, lightSpecular );

  GLfloat vertices[] = { 1.0, 1.0, -1.0, 1.0, 1.0, -1.0, -1.0, -1.0, };

  GLuint vbo_vertices;
  glGenBuffers( 1, &vbo_vertices );
  glBindBuffer( GL_ARRAY_BUFFER, vbo_vertices );
  glBufferData( GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW );
  glBindBuffer( GL_ARRAY_BUFFER, 0 );

  glBindBuffer( GL_ARRAY_BUFFER, vbo_vertices );
  glEnableVertexAttribArray( vRayPosition );
  glVertexAttribPointer( vRayPosition,  // attribute
      2,                  // number of elements per vertex, here (x,)
      GL_FLOAT,           // the type of each element
      GL_FALSE,           // take our values as-is
      0,                  // no extra data between each position
      0                   // offset of first element
      );



  glutSwapBuffers();
  glDisableVertexAttribArray( vRayPosition );

  frameCount++;
  if ( frameCount == 10 ) {
    float elapsedTime = glutGet( GLUT_ELAPSED_TIME );
    float fps = 10.0 / ((elapsedTime - previousTime) / 1000.0);
    printf("fps: %f\n", fps );
    previousTime = elapsedTime;
    frameCount = 0;
  }
#else
  gprint(PRINT_ERROR, "STEVE JOBS SAYS YOU CAN'T DO THAT!\n");
#endif
}

void RayTracer::addVec3ToVector(std::vector<GLfloat> *_vector, vec3 _vec3) {
  _vector->push_back(_vec3.x);
  _vector->push_back(_vec3.y);
  _vector->push_back(_vec3.z);
}

void RayTracer::setMinMax(vec3 *min, vec3 *max, vec3 v) {

  if(min->x > v.x) {
    min->x = v.x;
  } else if (max->x < v.x) {
    max->x = v.x;
  }

  if(min->y > v.y) {
    min->y = v.y;
  } else if (max->y < v.y) {
    max->y = v.y;
  }

  if(min->z > v.z) {
    min->z = v.z;
  } else if (max->z < v.z) {
    max->z = v.z;
  }
}

void RayTracer::addTriangle( const vec3& a, const vec3& b, const vec3& c,
      const vec3& diffuse, const vec3& ambient, const vec3& specular,
      float shininess, float reflect, float refract) {

  triangle_t newTriangle;

  vec3 normal = normalize(cross(b - a, c - b));
  float centerX, centerY, centerZ, x, y, z;

  centerX = (a.x + b.x + c.x) / 3;
  centerY = (a.y + b.y + c.y) / 3;
  centerZ = (a.z + b.z + c.z) / 3;

  float distance = 0.0;
  float tempDistance = 0.0;

  x = centerX - a.x;
  y = centerY - a.y;
  z = centerZ - a.z;
  tempDistance = sqrtf((x * x) + (y * y) + (z * z));
  if(tempDistance > distance) distance = tempDistance;

  x = centerX - b.x;
  y = centerY - b.y;
  z = centerZ - b.z;
  tempDistance = sqrtf((x * x) + (y * y) + (z * z));
  if(tempDistance > distance) distance = tempDistance;

  x = centerX - c.x;
  y = centerY - c.y;
  z = centerZ - c.z;
  tempDistance = sqrtf((x * x) + (y * y) + (z * z));
  if(tempDistance > distance) distance = tempDistance;
  distance += 0.0001;

  newTriangle.a = a;
  newTriangle.b = b;
  newTriangle.c = c;
  newTriangle.diffuse = diffuse;
  newTriangle.ambient = ambient;
  newTriangle.specular = specular;

  newTriangle.shininess = shininess;
  newTriangle.reflect = reflect;
  newTriangle.refract = refract;

  newTriangle.normal = normal;

  newTriangle.centerx = centerX;
  newTriangle.centery = centerY;
  newTriangle.centerz = centerZ;
  newTriangle.distance = distance;
  newTriangle.distanceSquared = (distance * distance);
  newTriangle.sentinel = 0.0;

  GLfloat *ptr = NULL;
  for ( ptr = (GLfloat *)&newTriangle;
  (void *)ptr < (void *)(&newTriangle + 1);
  ptr++ ) {
    bufferData.push_back( *ptr );
  }

  numTriangles++;
}

void RayTracer::pushDataToBuffer() {
#ifndef __APPLE__
  gprint( PRINT_DEBUG, "numTriangles %d\n", numTriangles );
  gprint( PRINT_DEBUG, "numOfBoundingBoxes %d\n", numOfBoundingBoxes );

  GLuint bufObj;
  glActiveTexture(GL_TEXTURE0);
  glGenBuffers(1, &bufObj);
  glBindBuffer(GL_TEXTURE_BUFFER, bufObj);
  glBufferData(GL_TEXTURE_BUFFER, sizeof(GLfloat) * bufferData.size(), bufferData.data(), GL_STATIC_DRAW);
  glTexBuffer(GL_TEXTURE_BUFFER, GL_RGB32F, bufObj);
#else
  gprint(PRINT_ERROR, "STEVE JOBS SAYS YOU CAN'T DO THAT!\n");
#endif
}

/*
void RayTracer::genereateScene(std::vector<Object*> objects) {

  std::vector<GLfloat> boundingBoxes;

  vec3 diffuse = vec3(0.0, 1.0, 0.0);
  vec3 ambient = vec3(0.0, 0.1, 0.0);
  vec3 specular = vec3(0.0, 0.0, 0.0);

  for(std::vector<Object*>::iterator it = objects.begin(); it != objects.end(); it++)
  {

    vec3 min, max;

    vec4 *vertices = (*it)->_vertices.data();
    min.x = vertices[0].x;
    max.x = vertices[0].x;
    min.y = vertices[0].y;
    max.y = vertices[0].y;
    min.z = vertices[0].z;
    max.z = vertices[0].z;

    int bounding = 10;

    unsigned long int count = 0;
    while(count < (*it)->_vertices.size()) {
      vec4 vertex = vertices[count++];
      vec3 a = vec3(vertex.x, vertex.y, vertex.z);
      vertex = vertices[count++];
      vec3 b = vec3(vertex.x, vertex.y, vertex.z);
      vertex = vertices[count++];
      vec3 c = vec3(vertex.x, vertex.y, vertex.z);

      setMinMax(&min, &max, a);
      setMinMax(&min, &max, b);
      setMinMax(&min, &max, c);

      //addTriangle(a, b, c, diffuse, ambient, specular, 1.0, 0.5, 0.0);

      if(numTriangles > 0 && numTriangles % bounding == 0) {
        addVec3ToVector(&boundingBoxes, min);
        addVec3ToVector(&boundingBoxes, max);
        boundingBoxes.push_back(float(numTriangles - bounding)); // starting index
        boundingBoxes.push_back((float)numTriangles); // ending index
        boundingBoxes.push_back(0.0); // unused
        numOfBoundingBoxes += 1;

        min.x = vertices[count + 1].x;
        max.x = vertices[count + 1].x;
        min.y = vertices[count + 1].y;
        max.y = vertices[count + 1].y;
        min.z = vertices[count + 1].z;
        max.z = vertices[count + 1].z;
      }

    }

    addVec3ToVector(&boundingBoxes, min);
    addVec3ToVector(&boundingBoxes, max);
    boundingBoxes.push_back(float(numTriangles - bounding)); // starting index
    boundingBoxes.push_back((float)numTriangles); // ending index
    boundingBoxes.push_back(0.0); // unused
    numOfBoundingBoxes += 1;
  }

  for(unsigned long int i = 0; i < boundingBoxes.size(); i++) {
    bufferData.push_back(boundingBoxes.data()[i]);
  }

  pushDataToBuffer();
}
*/

/**
 * Initialization of objects and OpenGL state.
 */
void RayTracer::init( GLint shader ) {
  if (shader != program)
  {
    program = shader;

    // Create a vertex array object
    GLuint vao;
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

    glUseProgram( program );

    vRayPosition = glGetAttribLocation( program, "vRayPosition" );
    uDisplay = glGetUniformLocation( program, "uDisplay" );

    uRotationMatrix = glGetUniformLocation( program, "uRotationMatrix" );
    uCameraPosition = glGetUniformLocation( program, "uCameraPosition" );

    uNumOfSpheres = glGetUniformLocation( program, "uNumOfSpheres" );
    uSphereCenterPoints = glGetUniformLocation( program, "uSphereCenterPoints" );
    uSphereRadius = glGetUniformLocation( program, "uSphereRadius" );
    uSphereDiffuse = glGetUniformLocation( program, "uSphereDiffuse" );
    uSphereAmbient = glGetUniformLocation( program, "uSphereAmbient" );
    uSphereSpecular = glGetUniformLocation( program, "uSphereSpecular" );
    uSphereShininess = glGetUniformLocation( program, "uSphereShininess" );
    uSphereReflect = glGetUniformLocation( program, "uSphereReflect" );
    uSphereRefract = glGetUniformLocation( program, "uSphereRefract" );

    uNumOfTriangle = glGetUniformLocation( program, "uNumOfTriangle" );
    uNumOfTriangleVectors = glGetUniformLocation( program, "uNumOfTriangleVectors" );

    uNumOfBoundingBoxes = glGetUniformLocation( program, "uNumOfBoundingBoxes" );

    uLightPositions = glGetUniformLocation( program, "uLightPositions" );
    uLightDiffuse = glGetUniformLocation( program, "uLightDiffuse" );
    uLightSpecular = glGetUniformLocation( program, "uLightSpecular" );

    tick.setTimeUniform( glGetUniformLocation( program, "ftime" ) );

    glShadeModel( GL_FLAT );
    glEnable( GL_DEPTH_TEST );
    glClearColor( 0.1, 0.1, 0.1, 1.0 );
  }
}

