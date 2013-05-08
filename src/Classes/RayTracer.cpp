/*
 * RayTracer.cpp
 *
 *  Created on: Apr 29, 2013
 *      Author: eric
 */

#include "RayTracer.h"
#include "Engine.hpp"

RayTracer::RayTracer() : 
  _lightPositions( NULL ),
  _lightDiffuse( NULL ),
  _lightSpecular( NULL ),
  display(boost::bind(&RayTracer::_display,this)),
  _extinguish( false ),
  _readytorebuffer(false) {

  // Handles
  _program = 0;
  _vRayPosition = -1;
  // Uniform Handles
  _uNumOfSpheres = -1;
  _uNumOfTriangle = -1;
  _uNumOfTriangleVectors = -1;
  _uNumOfL2BoundingBoxes = -1;
  _uNumOfL1BoundingBoxes = -1;
  _uNumOfL2TrianglesBounded = -1;
  _uNumberOfLights = -1;
  _uLightPositions = -1;
  _uLightDiffuse = -1;
  _uLightSpecular = -1;
  _uNumSGTransformations = -1;
  _uSceneGraphTransformations = -1;
  // State
  _numberOfLights = 2;
  _numTriangles = 0;
  _numSpheres = 0;
  _numOfL2BoundingBoxes = 0;
  _numOfL1BoundingBoxes = 0;
  _numOfL2TrianglesBounded = 20;
  _numOfTriangleVectors = (sizeof(triangle_t))/(sizeof(GLfloat)*3);

  // Turn on lights, THEN enable lighting-thead.
  _lightPositions = (GLfloat*)malloc(sizeof(GLfloat)*6);
  _lightPositions[0] = _lightPositions[5] = -2.0;
  _lightPositions[1] = _lightPositions[4] = 4.0;
  _lightPositions[2] = _lightPositions[3] = 10.0;  
  _lightDiffuse = (GLfloat*)malloc(sizeof(GLfloat)*6);
  for ( size_t i = 0; i < 6; ++i ) { _lightDiffuse[i] = 0.8; }
  _lightSpecular = (GLfloat*)malloc(sizeof(GLfloat)*6);
  for ( size_t i = 0; i < 6; ++i ) { _lightSpecular[i] = 1.0; }
  _extinguish = false;

  gprint( PRINT_DEBUG, "RayTrace() Constructor Exiting\n" );
}

RayTracer::~RayTracer()
{
  thisDateIsOver();
  if (_lightPositions) free(_lightPositions);
  if (_lightDiffuse)   free(_lightDiffuse);
  if (_lightSpecular)  free(_lightSpecular);
}

/**
 * Handle the re-display of the scene.
 */
void RayTracer::_display( void ) {

#ifdef __APPLE__
  gprint( PRINT_INFO, "Raytracer::display() unsupported on Apple OSX at this time.\n" );
  return;
#else
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  glUniform1i( _uNumOfSpheres, _numSpheres );

  glUniform1i( _uNumOfTriangle, _numTriangles );
  glUniform1i( _uNumOfTriangleVectors, _numOfTriangleVectors );

  glUniform1i( _uNumOfL2BoundingBoxes, _numOfL2BoundingBoxes );
  glUniform1i( _uNumOfL1BoundingBoxes, _numOfL1BoundingBoxes );

  glUniform1i( _uNumOfL2TrianglesBounded, _numOfL2TrianglesBounded );

  glUniform1i( _uNumberOfLights, *(Engine::instance()->getNumLights()) );
  glUniform3fv( _uLightPositions, _numberOfLights, Engine::instance()->getLightPositions() );
  glUniform3fv( _uLightDiffuse, _numberOfLights, Engine::instance()->getLightDiffuses() );
  glUniform3fv( _uLightSpecular, _numberOfLights, Engine::instance()->getLightSpeculars() );

  glUniform1i( _uNumSGTransformations, _sceneData.size() );
  if (_sceneData.size() > 0)
    glUniformMatrix4fv( _uSceneGraphTransformations, _sceneData.size(),
			GL_TRUE, (GLfloat*)&(_sceneData.at(0)) );

  static const GLfloat vertices[] = { 1.0, 1.0, -1.0, 1.0, 1.0, -1.0, -1.0, -1.0, };

  GLuint vbo_vertices;
  glGenBuffers( 1, &vbo_vertices );
  glBindBuffer( GL_ARRAY_BUFFER, vbo_vertices );
  glBufferData( GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW );
  glBindBuffer( GL_ARRAY_BUFFER, 0 );

  glBindBuffer( GL_ARRAY_BUFFER, vbo_vertices );
  glEnableVertexAttribArray( _vRayPosition );
  glVertexAttribPointer( _vRayPosition,  // attribute
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
  glDisableVertexAttribArray( _vRayPosition );

  static unsigned frameCount = 0;
  static float previousTime = 0.0;
  if ( frameCount++ == 10 ) {
    float elapsedTime = glutGet( GLUT_ELAPSED_TIME );
    float fps = 10.0 / ((elapsedTime - previousTime) / 1000.0);
    gprint( PRINT_INFO, "fps: %f\n", fps );
    previousTime = elapsedTime;
    frameCount = 0;
  }

  _readytorebuffer = true;

  // Draw particles-only
  // Broken >_<
  // Engine::instance()->rootScene()->draw( false );
#endif
}

void RayTracer::addVec3ToVector(std::vector<GLfloat> *_vector, vec3 _vec3) {
  _vector->push_back(_vec3.x);
  _vector->push_back(_vec3.y);
  _vector->push_back(_vec3.z);
}

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

void RayTracer::setMinMax( vec3 &min, vec3 &max, triangle_t &t ) {

  MINTRIPLET( x );
  MINTRIPLET( y );
  MINTRIPLET( z );

  MAXTRIPLET( x );
  MAXTRIPLET( y );
  MAXTRIPLET( z );
}

void RayTracer::addTriangle( const vec3 &a, const vec3 &b, const vec3 &c,
			     const vec3 &diffuse, const vec3 &ambient, const vec3 &specular,
			     float shininess, float reflect, float refract,
			     vec3 normal ) {

  triangle_t newTriangle;

  if (normal.x == -INFINITY) {
    normal = normalize(cross(b - a, c - b));
  }
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

  _triangleData.push_back( newTriangle );
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
  size_t floatsPerTriangle = _numOfTriangleVectors * 3;
  size_t newFloats = count * floatsPerTriangle;
  if ((sizeof(triangle_t) * count) != (newFloats * sizeof(GLfloat))) {
    throw std::logic_error( "Size of new triangles to add is different from the size "
          "we requested for new floats we requested for the buffer." );
  }

  dataBuffer.resize( numFloats + newFloats );
  void *src = &(dataBuffer.at(numFloats));
  void *dst = &(triangleBuffer.at(start));
  memcpy( src, dst, sizeof(triangle_t) * count );

}

void RayTracer::pushDataToBuffer() {

#ifdef __APPLE__
  gprint( PRINT_INFO, "Non-supported on Apple OSX with glsl 1.2 at this time." );
  return;
#else
  //boost::mutex::scoped_lock l(_framelock);

  _bufferData.clear();
  _bufferData.resize(0);
  _triangleData.resize(0);
  _numOfL2BoundingBoxes = 0;
  _numTriangles = 0;
  _numOfL1BoundingBoxes = 0;
  Engine::instance()->rootScene()->bufferToRaytracer( *this );
  Engine::instance()->rootScene()->sceneToRaytracer( *this );

  vec3 min, max;
  min = vec3( INFINITY, INFINITY, INFINITY );
  max = vec3( -INFINITY, -INFINITY, -INFINITY );

  std::vector<triangle_t>::iterator it;
  size_t count = 1;
  size_t numTriangles = _triangleData.size();

  for (it = _triangleData.begin(); it != _triangleData.end(); ++it, ++count ) {
    setMinMax( min, max, *it );
    
    // If we've added some amount of triangles (20, currently)
    // Or if this is the LAST triangle, add the bounding box and triangles.
    if ((count % _numOfL2TrianglesBounded == 0) ||
	(count == _triangleData.size())) {

      // Push bounding box into the buffer.
      addVec3ToVector(&_bufferData, min);
      addVec3ToVector(&_bufferData, max);

      // ????
      _bufferData.push_back((GLfloat)_numOfL2TrianglesBounded);
      _bufferData.push_back(0.0);
      _bufferData.push_back(0.0);

      // Push the triangles this box bounds into the buffer.
      // pTDTB( destination, source, start_index, num_to_push )
      pushTriangleDataToBuffer( _bufferData, _triangleData,
        // Below: (0 * 20), (1 * 20), etc.
				(_numOfL2BoundingBoxes * _numOfL2TrianglesBounded),
				_numOfL2TrianglesBounded );

      // Increment how many Bounding Boxes we've generated.
      ++_numOfL2BoundingBoxes;

      // Reset our Bounding Box.
      min = vec3( INFINITY, INFINITY, INFINITY );
      max = vec3( -INFINITY, -INFINITY, -INFINITY );
    }
  }

  gprint( PRINT_DEBUG, "numTriangles %d\n", numTriangles );
  gprint( PRINT_DEBUG, "numOfL2BoundingBoxes %d\n", _numOfL2BoundingBoxes );
#endif
}

/**
 * Initialization of objects and OpenGL state.
 */
void RayTracer::init( GLuint shader ) {

  _program = shader;
  Engine::instance()->switchShader( _program );

  // Create a vertex array object
  GLuint vao;
  glGenVertexArrays( 1, &vao );
  glBindVertexArray( vao );

  _vRayPosition = glGetAttribLocation( _program, "vRayPosition" );
  _uNumOfSpheres = glGetUniformLocation( _program, "uNumOfSpheres" );
  _uNumOfTriangle = glGetUniformLocation( _program, "uNumOfTriangle" );
  _uNumOfTriangleVectors = glGetUniformLocation( _program, "uNumOfTriangleVectors" );
  _uNumOfL2BoundingBoxes = glGetUniformLocation( _program, "uNumOfL2BoundingBoxes" );
  _uNumOfL1BoundingBoxes = glGetUniformLocation( _program, "uNumOfL1BoundingBoxes" );
  _uNumOfL2TrianglesBounded = glGetUniformLocation( _program, "uNumOfL2TrianglesBounded" );
  _uNumberOfLights = glGetUniformLocation( _program, "uNumberOfLights" );
  _uLightPositions = glGetUniformLocation( _program, "uLightPositions" );
  _uLightDiffuse = glGetUniformLocation( _program, "uLightDiffuse" );
  _uLightSpecular = glGetUniformLocation( _program, "uLightSpecular" );
  _uNumSGTransformations = glGetUniformLocation( _program, "uNumSGTransformations" );
  _uSceneGraphTransformations = glGetUniformLocation( _program, "uSceneGraphTransformations" );
  tick.setTimeUniform( glGetUniformLocation( _program, "ftime" ) );
}

void RayTracer::legacySceneGen( void ) {

  Object *bottle = new Object("", _program);
  ObjLoader::loadModelFromFile( bottle, "../models/bottle_wine_med.obj" );
  vec4 *vertices = bottle->_vertices.data();

  unsigned long int count = 0;
  while(count < bottle->_vertices.size()) {
    vec4 vertex = vertices[count++];
    vec3 a = vec3(vertex.x, vertex.y, vertex.z);
    vertex = vertices[count++];
    vec3 b = vec3(vertex.x, vertex.y, vertex.z);
    vertex = vertices[count++];
    vec3 c = vec3(vertex.x, vertex.y, vertex.z);
    
    addTriangle(a, b, c, vec3(0.5, 0.5, 0.5), vec3(0.05, 0.05, 0.05), vec3(1.0, 1.0, 1.0), 100.0, 0.3, 0.0);
  }
  /*
  addTriangle(vec3(-5.0, -3.0, -5.0), vec3(5.0, -3.0, -5.0), vec3(5.0, 7.0, -5.0), vec3(1.0, 1.0, 1.0), vec3(0.05, 0.05, 0.05), vec3(1.0, 1.0, 1.0), 100.0, 0.0, 0.0);
  addTriangle(vec3(-5.0, -3.0, -5.0), vec3(5.0, 7.0, -5.0), vec3(-5.0, 7.0, -5.0), vec3(0.0, 0.0, 1.0), vec3(0.0, 0.0, 0.05), vec3(0.0, 0.0, 1.0), 100.0, 0.0, 0.0);

  addTriangle(vec3(-5.0, -3.0, -5.0), vec3(-5.0, -3.0, 5.0), vec3(5.0, -3.0, 5.0), vec3(0.0, 1.0, 0.0), vec3(0.0, 0.05, 0.0), vec3(0.0, 1.0, 0.0), 100.0, 0.0, 0.0);
  addTriangle(vec3(-5.0, -3.0, -5.0), vec3(5.0, -3.0, 5.0), vec3(5.0, -3.0, -5.0), vec3(1.0, 0.0, 0.0), vec3(0.05, 0.0, 0.0), vec3(1.0, 0.0, 0.0), 100.0, 0.0, 0.0);

  addTriangle(vec3(-5.0, -3.0, -5.0), vec3(-5.0, 7.0, 5.0), vec3(-5.0, -3.0, 5.0), vec3(1.0, 0.0, 1.0), vec3(0.05, 0.0, 0.05), vec3(1.0, 0.0, 1.0), 100.0, 0.0, 0.0);
  addTriangle(vec3(-5.0, -3.0, -5.0), vec3(-5.0, 7.0, -5.0), vec3(-5.0, 7.0, 5.0), vec3(1.0, 1.0, 0.0), vec3(0.05, 0.05, 0.0), vec3(1.0, 1.0, 0.0), 100.0, 0.0, 0.0);
  */

  pushDataToBuffer();
}

// Artist formerly known as ARomanticEvening.
// Well, STILL known as that, if you look in Classes/MONOLITH.cpp.
void RayTracer::lightFlicker( void ) {
  while ( !_extinguish ) {
    float lightness = (float)rand() / (float)RAND_MAX;
    lightness = lightness * 3.0 / 10.0;

    lightness += .7;
    _lightDiffuse[0] = _lightDiffuse[1] = _lightDiffuse[2] = lightness;

    sleep(0.01); //yielding is for sissies. I exist, therefore I have the right of way.
  }
}

void RayTracer::addTransformation( const Angel::mat4 &mat ) {

  _sceneData.push_back( mat );

}


void RayTracer::idleHandsSpendTimeWithTheTextureBuffer()
{
#ifdef __APPLE__
  return;
#else
  if (_readytorebuffer)
  {
    printf("buffering...\n");
    pushDataToBuffer();

    GLuint bufObj;
    glGenBuffers(1, &bufObj);
    glBindBuffer(GL_TEXTURE_BUFFER, bufObj);
    glBufferData(GL_TEXTURE_BUFFER, sizeof(GLfloat) * _bufferData.size(), _bufferData.data(), GL_STATIC_DRAW);
    glTexBuffer(GL_TEXTURE_BUFFER, GL_RGB32F, bufObj);
    glActiveTexture(GL_TEXTURE0);
    _readytorebuffer = false;
  }
#endif
}

void RayTracer::thisDateIsOver( void ) {
  _extinguish = true;
}
