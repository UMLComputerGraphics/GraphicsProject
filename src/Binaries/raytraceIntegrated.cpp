/**
 * @file raytrace1.cpp
 * @author Hoanh Nguyen
 * @date 2013-03-15
 * @brief Geometric Raytracing Demo
 */

#include <cmath>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <vector>
#include <boost/thread.hpp>

#include "Engine.hpp"
#include "SpelchkCamera.hpp"


#define GLCHECK(); if (glGetError()) { gprint( PRINT_ERROR, "glGetError() is true on line %d\n", __LINE__ ); exit(255); }

/** Global shader object **/
GLuint program = -1;
GLint vRayPosition = -1;
/** Rotation matrix uniform shader variable location **/
GLuint uRotationMatrix = -1;
/** Handle to uniform that will contain the position of the Camera. **/
GLint uCameraPosition = -1;

GLint uSphereCenterPoints = -1;
GLint uSphereRadius = -1;
GLint uSphereDiffuse = -1;
GLint uSphereAmbient = -1;
GLint uSphereSpecular = -1;
GLint uSphereShininess = -1;
GLint uSphereReflect = -1;
GLint uSphereRefract = -1;
GLint uNumOfSpheres = -1;

GLint uNumOfTriangle = -1;
GLint uNumOfTriangleVectors = -1;

GLint uNumOfL2BoundingBoxes = -1;
GLint uNumOfL1BoundingBoxes = -1;

GLint uNumOfL2TrianglesBounded = -1;

GLint uNumberOfLights = -1;
GLint uLightPositions = -1;
GLint uLightDiffuse = -1;
GLint uLightSpecular = -1;

//----------------------------------------------------------------------------

GLfloat sphereCenterPoints[] = { 0.0, 0.0, 5.0,
                                 5.0, 0.0, 0.0,
                                 5.0, 0.0, 5.0,
                                 1.0, 1.0, -2.0 };

GLfloat sphereRadius[] = { 0.5,
                           0.5,
                           0.5,
                           0.8 };

GLfloat sphereDiffuse[] = { 0.05, 0.05, 0.05,
                            0.05, 0.05, 0.05,
                            0.05, 0.05, 0.05,
                           1.0, 0.3, 0.3 };

GLfloat sphereAmbient[] = { 0.05, 0.05, 0.05,
                            0.05, 0.05, 0.05,
                            0.05, 0.05, 0.05,
                            0.0, 0.0, 0.0 };

GLfloat sphereSpecular[] = { 1.0, 1.0, 1.0,
                             1.0, 1.0, 1.0,
                             1.0, 1.0, 1.0,
                             0.0, 0.0, 0.0 };

GLfloat sphereShininess[] = { 1000.0,
                              1000.0,
                              1000.0,
                              1.0 };

GLfloat sphereReflect[] = { 0.5,
                            1.0,
                            1.0,
                            1.0 };

GLfloat sphereRefract[] = { 1.0,
                            1.0,
                            1.0,
                            1.0 };

int numberOfLights = 2;

GLfloat lightPositions[] = { -2.0, 4.0, 10.0,
                             10.0, 4.0, -2.0};
GLfloat lightDiffuse[] = { 0.8, 0.8, 0.8,
                           0.8, 0.8, 0.8};
GLfloat lightSpecular[] = { 1.0, 1.0, 1.0,
                            1.0, 1.0, 1.0};

int numTriangles = 0;
std::vector<vec3> trianglePoints;

int numOfL2BoundingBoxes = 0;
int numOfL1BoundingBoxes = 0;

int numOfL2TrianglesBounded = 20;

std::vector<GLfloat> bufferData;
std::vector<GLfloat> triangleData;

int numOfTriangleVectors = (sizeof(triangle_t) / ((sizeof(GLfloat) * 3)));

bool stereo = false;

float eps = 0.0001;

//flicker at constant rate, regardless of update loop
bool extinguish = false;
void aRomanticEvening() {
  while ( !extinguish ) {
    // random number between 0 and 1
    float lightness = (float) rand() / (float) RAND_MAX;
    // between 0 and .3
    lightness = lightness * 3.0 / 10.0;
    
    lightness += .7;
    lightDiffuse[0] = lightness;
    lightDiffuse[1] = lightness;
    lightDiffuse[2] = lightness;
    
    sleep( 0.01 );
  }
}

int frameCount = 0;
float previousTime = 0.0;

/**
 * Handle the re-display of the scene.
 */
void display( void ) {
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  
  int numSpheres = 3;
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
    printf("fps: %f\n", fps );
    previousTime = elapsedTime;
    frameCount = 0;
  }
}

void addVec3ToVector(std::vector<GLfloat> *_vector, vec3 _vec3) {
  _vector->push_back(_vec3.x);
  _vector->push_back(_vec3.y);
  _vector->push_back(_vec3.z);
}

void setMinMax(vec3 *min, vec3 *max, vec3 v) {

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

void addTriangle( const vec3& a, const vec3& b, const vec3& c, 
		  const vec3& diffuse, const vec3& ambient, const vec3& specular, 
		  float shininess, float reflect, float refract) {

  trianglePoints.push_back(a);
  trianglePoints.push_back(b);
  trianglePoints.push_back(c);

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
  newTriangle.padding = 0.0;

  GLfloat *ptr = NULL;
  for ( ptr = (GLfloat *)&newTriangle;
	(void *)ptr < (void *)(&newTriangle + 1);
	ptr++ ) {
    triangleData.push_back( *ptr );
  }

  numTriangles++;

}

void addCube(vec3 position, vec3 diffuse, vec3 ambient, vec3 specular, float shininess, float reflect, float refract) {
  vec3 vertices[8] = {
    vec3(-0.5, -0.5,  0.5), // 0 left bottom front
    vec3(-0.5,  0.5,  0.5), // 1 left top front
    vec3( 0.5,  0.5,  0.5), // 2 right top front
    vec3( 0.5, -0.5,  0.5), // 3 right bottom front
    vec3(-0.5, -0.5, -0.5), // 4 left bottom back
    vec3(-0.5,  0.5, -0.5), // 5 left top back
    vec3( 0.5,  0.5, -0.5), // 6 right top back
    vec3( 0.5, -0.5, -0.5)  // 7 right bottom back
  };

  //front
  addTriangle(vertices[1] + position, vertices[0] + position, vertices[3] + position, diffuse, ambient, specular, shininess, reflect, refract);
  addTriangle(vertices[1] + position, vertices[3] + position, vertices[2] + position, diffuse, ambient, specular, shininess, reflect, refract);

  //back
  addTriangle(vertices[6] + position, vertices[7] + position, vertices[4] + position, diffuse, ambient, specular, shininess, reflect, refract);
  addTriangle(vertices[6] + position, vertices[4] + position, vertices[5] + position, diffuse, ambient, specular, shininess, reflect, refract);

  //right
  addTriangle(vertices[2] + position, vertices[3] + position, vertices[7] + position, diffuse, ambient, specular, shininess, reflect, refract);
  addTriangle(vertices[2] + position, vertices[7] + position, vertices[6] + position, diffuse, ambient, specular, shininess, reflect, refract);

  //left
  addTriangle(vertices[5] + position, vertices[4] + position, vertices[0] + position, diffuse, ambient, specular, shininess, reflect, refract);
  addTriangle(vertices[5] + position, vertices[0] + position, vertices[1] + position, diffuse, ambient, specular, shininess, reflect, refract);

  //top
  addTriangle(vertices[5] + position, vertices[1] + position, vertices[2] + position, diffuse, ambient, specular, shininess, reflect, refract);
  addTriangle(vertices[5] + position, vertices[2] + position, vertices[6] + position, diffuse, ambient, specular, shininess, reflect, refract);

  //bottom
  addTriangle(vertices[0] + position, vertices[4] + position, vertices[7] + position, diffuse, ambient, specular, shininess, reflect, refract);
  addTriangle(vertices[0] + position, vertices[7] + position, vertices[3] + position, diffuse, ambient, specular, shininess, reflect, refract);
}

void pushTriangleDataToBuffer(int start, int end) {
  unsigned long int index = start * 3 * numOfTriangleVectors;

  GLfloat *data = triangleData.data();
  for(int i = start; i < end && index < triangleData.size(); i++) {

    for(int j = 0; j < 3 * numOfTriangleVectors; j++) {
      bufferData.push_back(data[index++]);
    }
  }
}

void pushDataToBuffer() {

  vec3 min, max;
  vec3 *vertices = trianglePoints.data();
  min.x = vertices[0].x;
  max.x = vertices[0].x;
  min.y = vertices[0].y;
  max.y = vertices[0].y;
  min.z = vertices[0].z;
  max.z = vertices[0].z;

  int start;

  unsigned long int count = 0;
  int triangleCount = 0;
  while(count < trianglePoints.size()) {
    vec4 vertex = vertices[count++];
    vec3 a = vec3(vertex.x, vertex.y, vertex.z);
    vertex = vertices[count++];
    vec3 b = vec3(vertex.x, vertex.y, vertex.z);
    vertex = vertices[count++];
    vec3 c = vec3(vertex.x, vertex.y, vertex.z);

    setMinMax(&min, &max, a);
    setMinMax(&min, &max, b);
    setMinMax(&min, &max, c);
    triangleCount++;

    if(count > 0 && triangleCount % numOfL2TrianglesBounded == 0) {

      addVec3ToVector(&bufferData, min);
      addVec3ToVector(&bufferData, max);

      start = triangleCount - numOfL2TrianglesBounded;

      bufferData.push_back((float)numOfL2TrianglesBounded);
      bufferData.push_back(0.0);
      bufferData.push_back(0.0);
      numOfL2BoundingBoxes += 1;

      pushTriangleDataToBuffer(start, triangleCount);

      min.x = vertices[count + 1].x;
      max.x = vertices[count + 1].x;
      min.y = vertices[count + 1].y;
      max.y = vertices[count + 1].y;
      min.z = vertices[count + 1].z;
      max.z = vertices[count + 1].z;
    }
  }

  start = triangleCount - (triangleCount % numOfL2TrianglesBounded);

  addVec3ToVector(&bufferData, min);
  addVec3ToVector(&bufferData, max);
  bufferData.push_back(float(triangleCount % numOfL2TrianglesBounded));
  bufferData.push_back(0.0);
  bufferData.push_back(0.0);

  pushTriangleDataToBuffer(start, triangleCount);
  numOfL2BoundingBoxes += 1;

  gprint( PRINT_DEBUG, "numTriangles %d\n", numTriangles );
  gprint( PRINT_DEBUG, "numOfL2BoundingBoxes %d\n", numOfL2BoundingBoxes );


  GLuint bufObj;
  glActiveTexture(GL_TEXTURE0);
  glGenBuffers(1, &bufObj);
  glBindBuffer(GL_TEXTURE_BUFFER, bufObj);
  glBufferData(GL_TEXTURE_BUFFER, sizeof(GLfloat) * bufferData.size(), bufferData.data(), GL_STATIC_DRAW);
  glTexBuffer(GL_TEXTURE_BUFFER, GL_RGB32F, bufObj);
}

void genereateScene() {

  //TODO: raytrace1 use class/Raytrace

  Object *bottle = new Object("", program);
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


  addCube(vec3(-2.0, 1.0, -3.0), vec3(0.8, 0.8, 0.8), vec3(0.05, 0.05, 0.05), vec3(0.0, 0.0, 0.0), 1000.0, 0.5, 0.0);
  addCube(vec3(2.0, 1.0, -3.0), vec3(0.8, 0.8, 0.8), vec3(0.05, 0.05, 0.05), vec3(0.0, 0.0, 0.0), 1000.0, 0.5, 0.0);


  addTriangle(vec3(-5.0, -3.0, -5.0), vec3(5.0, -3.0, -5.0), vec3(5.0, 7.0, -5.0), vec3(1.0, 1.0, 1.0), vec3(0.05, 0.05, 0.05), vec3(1.0, 1.0, 1.0), 100.0, 0.0, 0.0);
  addTriangle(vec3(-5.0, -3.0, -5.0), vec3(5.0, 7.0, -5.0), vec3(-5.0, 7.0, -5.0), vec3(0.0, 0.0, 1.0), vec3(0.0, 0.0, 0.05), vec3(0.0, 0.0, 1.0), 100.0, 0.0, 0.0);

  addTriangle(vec3(-5.0, -3.0, -5.0), vec3(-5.0, -3.0, 5.0), vec3(5.0, -3.0, 5.0), vec3(0.0, 1.0, 0.0), vec3(0.0, 0.05, 0.0), vec3(0.0, 1.0, 0.0), 100.0, 0.0, 0.0);
  addTriangle(vec3(-5.0, -3.0, -5.0), vec3(5.0, -3.0, 5.0), vec3(5.0, -3.0, -5.0), vec3(1.0, 0.0, 0.0), vec3(0.05, 0.0, 0.0), vec3(1.0, 0.0, 0.0), 100.0, 0.0, 0.0);


  addTriangle(vec3(-5.0, -3.0, -5.0), vec3(-5.0, 7.0, 5.0), vec3(-5.0, -3.0, 5.0), vec3(1.0, 0.0, 1.0), vec3(0.05, 0.0, 0.05), vec3(1.0, 0.0, 1.0), 100.0, 0.0, 0.0);
  addTriangle(vec3(-5.0, -3.0, -5.0), vec3(-5.0, 7.0, -5.0), vec3(-5.0, 7.0, 5.0), vec3(1.0, 1.0, 0.0), vec3(0.05, 0.05, 0.0), vec3(1.0, 1.0, 0.0), 100.0, 0.0, 0.0);

  pushDataToBuffer();
}

/**
 * Initialization of objects and OpenGL state.
 */
void init( void ) {
  
  // Create a vertex array object
  GLuint vao;
  glGenVertexArrays( 1, &vao );
  glBindVertexArray( vao );
  
  // Load shaders and use the resulting shader program
  program = Angel::InitShader( "shaders/vRaytracer.glsl",
			       "shaders/fShaderSpheres2.glsl" );
  Engine::instance()->switchShader( program );
  Engine::instance()->rootScene()->shader( program );
  Engine::instance()->cams()->shader( program );
  Engine::instance()->cams()->active()->shader( program );
  glUseProgram( program );

  vRayPosition = glGetAttribLocation( program, "vRayPosition" );
  uRotationMatrix = glGetUniformLocation( program, "R" );
  uCameraPosition = glGetUniformLocation( program, "T" );

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

  //glShadeModel( GL_FLAT );
  //glEnable( GL_DEPTH_TEST );
  //glClearColor( 0.1, 0.1, 0.1, 1.0 );
  genereateScene();
}

/**
 * Simple raytracer demo by Hoanh Nguyen.
 * @param argc Not used.
 * @param argv Not used.
 * @return 0.
 */
int main( int argc, char **argv ) {

  Engine::init( &argc, argv, "Raytracer" );
  init();

  boost::thread zipo( aRomanticEvening );
  
  glutDisplayFunc( display ); // register callback w/Window System

  GLCHECK();
  Engine::run();
  
  extinguish = true;
  zipo.join();
  
  return 0;
}
