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
}

RayTracer::~RayTracer()
{
}



void RayTracer::setMinMax( vec3 &min, vec3 &max, triangle_t &t ) {

  MINTRIPLET( x );
  MINTRIPLET( y );
  MINTRIPLET( z );

  MAXTRIPLET( x );
  MAXTRIPLET( y );
  MAXTRIPLET( z );

  }

/**
 * Handle the re-display of the scene.
 */
void RayTracer::_display( void ) {
#ifndef __APPLE__
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

   glUniform1i( uNumOfSpheres, numSpheres );
   glUniform3fv( uSphereCenterPoints, numSpheres, sphereCenterPoints );
   glUniform1fv( uSphereRadius, numSpheres, sphereRadius );
   glUniform3fv( uSphereDiffuse, numSpheres, sphereDiffuse );
   glUniform3fv( uSphereAmbient, numSpheres, sphereAmbient );
   glUniform3fv( uSphereSpecular, numSpheres, sphereSpecular );
   glUniform1fv( uSphereShininess, numSpheres, sphereShininess );
   glUniform1fv( uSphereReflect, numSpheres, sphereReflect );
   glUniform1fv( uSphereRefract, numSpheres, sphereRefract );

   glUniform1i( uNumOfTriangle, numTriangles );
   glUniform1i( uNumOfTriangleVectors, numOfTriangleVectors );

   glUniform1i( uNumOfL2BoundingBoxes, numOfL2BoundingBoxes );
   glUniform1i( uNumOfL1BoundingBoxes, numOfL1BoundingBoxes );

   glUniform1i( uNumOfL2TrianglesBounded, numOfL2TrianglesBounded );

   glUniform1i( uNumberOfLights, numberOfLights );
   glUniform3fv( uLightPositions, numberOfLights, lightPositions );
   glUniform3fv( uLightDiffuse, numberOfLights, lightDiffuse );
   glUniform3fv( uLightSpecular, numberOfLights, lightSpecular );

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

   Engine::instance()->cams()->active()->relinkUniforms();
   Engine::instance()->cams()->active()->view();

   glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );

   glutSwapBuffers();
   glDisableVertexAttribArray( vRayPosition );

   frameCount++;
   if ( frameCount == 10 ) {
     float elapsedTime = glutGet( GLUT_ELAPSED_TIME );
     float fps = 10.0 / ((elapsedTime - previousTime) / 1000.0);
     gprint( PRINT_INFO, "fps: %f\n", fps );
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

  /** Load our data into the triangle slice **/
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
  newTriangle.padding = 0.0;

  triangle_tData.push_back( newTriangle );
}

void RayTracer::pushTriangleDataToBuffer( std::vector<GLfloat> &dataBuffer,
    std::vector<triangle_t> &triangleBuffer,
    size_t start, size_t count ) {
    gprint( PRINT_DEBUG, "Adding triangles from %lu to %lu, out of %lu total.\n",
      start, start + count, triangleBuffer.size() );

    size_t numFloats    = dataBuffer.size();
    size_t numTriangles = triangleBuffer.size();

    if (start > numTriangles) {
      throw std::logic_error( "start index in pushTriangleDataToBuffer cannot exceed "
            "the total size of the triangleBuffer vector.\n" );
    }

    /** Request a std::vector resize **/
    size_t numLeft = (numTriangles - start);
    if (count > numLeft) count = numLeft;
    size_t floatsPerTriangle = numOfTriangleVectors * 3;
    size_t newFloats = count * floatsPerTriangle;
    if ((sizeof(triangle_t) * count) != (newFloats * sizeof(GLfloat))) {
      throw std::logic_error( "Size of new triangles to add is different from the size "
            "we requested for new floats we requested for the buffer." );
    }

    dataBuffer.resize( numFloats + newFloats );
    void *dst = &(dataBuffer.at(numFloats));
    void *src = &(triangleBuffer.at(start));
    memcpy( dst, src, sizeof(triangle_t) * count );
}

void RayTracer::pushDataToBuffer( std::vector<GLfloat> &dataBuffer ) {
#ifndef __APPLE__
  vec3 min, max;
    min = vec3( INFINITY, INFINITY, INFINITY );
    max = vec3( -INFINITY, -INFINITY, -INFINITY );

    std::vector<triangle_t>::iterator it;
    size_t count = 1;
    numTriangles = triangle_tData.size();

    for (it = triangle_tData.begin(); it != triangle_tData.end(); ++it, ++count ) {
      setMinMax( min, max, *it );

      // If we've added some amount of triangles (20, currently)
      // Or if this is the LAST triangle, add the bounding box and triangles.
      if ((count % numOfL2TrianglesBounded == 0) ||
    (count == triangle_tData.size())) {

        // Push bounding box into the buffer.
        addVec3ToVector(&bufferData, min);
        addVec3ToVector(&bufferData, max);

        // ????
        dataBuffer.push_back((GLfloat)numOfL2TrianglesBounded);
        dataBuffer.push_back(0.0);
        dataBuffer.push_back(0.0);

        // Push the triangles this box bounds into the buffer.
        // pTDTB( destination, source, start_index, num_to_push )
        pushTriangleDataToBuffer( dataBuffer, triangle_tData,
          // Below: (0 * 20), (1 * 20), etc.
          (numOfL2BoundingBoxes * numOfL2TrianglesBounded),
          numOfL2TrianglesBounded );

        // Increment how many Bounding Boxes we've generated.
        ++numOfL2BoundingBoxes;

        // Reset our Bounding Box.
        min = vec3( INFINITY, INFINITY, INFINITY );
        max = vec3( -INFINITY, -INFINITY, -INFINITY );
      }
    }

    gprint( PRINT_DEBUG, "numTriangles %d\n", numTriangles );
    gprint( PRINT_DEBUG, "numOfL2BoundingBoxes %d\n", numOfL2BoundingBoxes );


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

    Engine::instance()->switchShader( program );
    Engine::instance()->rootScene()->shader( program );
    Engine::instance()->cams()->shader( program );
    Engine::instance()->cams()->active()->shader( program );

    vRayPosition = glGetAttribLocation( program, "vRayPosition" );

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

    uNumOfL2BoundingBoxes = glGetUniformLocation( program, "uNumOfL2BoundingBoxes" );
    uNumOfL1BoundingBoxes = glGetUniformLocation( program, "uNumOfL1BoundingBoxes" );

    uNumOfL2TrianglesBounded = glGetUniformLocation( program, "uNumOfL2TrianglesBounded" );

    uNumberOfLights = glGetUniformLocation( program, "uNumberOfLights" );
    uLightPositions = glGetUniformLocation( program, "uLightPositions" );
    uLightDiffuse = glGetUniformLocation( program, "uLightDiffuse" );
    uLightSpecular = glGetUniformLocation( program, "uLightSpecular" );

    tick.setTimeUniform( glGetUniformLocation( program, "ftime" ) );

    glShadeModel( GL_FLAT );
    glEnable( GL_DEPTH_TEST );
    glClearColor( 0.1, 0.1, 0.1, 1.0 );
  }
}

