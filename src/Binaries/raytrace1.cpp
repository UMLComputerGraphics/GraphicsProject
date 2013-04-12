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

#include "SpelchkCamera.hpp"
#include "InitShader.hpp"
#include "Timer.hpp"
#include "Util.hpp"

#include "ObjLoader.hpp"

/** Global shader object **/
GLuint program;

GLint vRayPosition = -1;
GLint uDisplay;

/** Rotation matrix uniform shader variable location **/
GLuint uRotationMatrix = -1;
/** Handle to uniform that will contain the position of the Camera. **/
GLint uCameraPosition = -1;

/** Logical camera object. **/
SpelchkCamera camera( vec4( 0.0, 0.0, 10.0, 0.0 ) );
SpelchkCamera cameraLeft( vec4( 0.0, 0.0, 10.0, 0.0 ) );
SpelchkCamera cameraRight( vec4( 0.0, 0.0, 10.0, 0.0 ) );

GLint uSphereCenterPoints = -1;
GLint uSphereRadius = -1;
GLint uSphereDiffuse = -1;
GLint uSphereAmbient = -1;
GLint uSphereSpecular = -1;
GLint uSphereShininess = -1;
GLint uNumOfSpheres = -1;

GLint uNumOfTriangle = -1;
GLint uNumOfTriangleVectors = -1;

GLint uNumOfBoundingSpheres = - 1;
GLint uNumOfTrianglesBounded = -1;

GLint uLightPositions = -1;
GLint uLightDiffuse = -1;
GLint uLightSpecular = -1;

//----------------------------------------------------------------------------

GLfloat sphereCenterPoints[] = { 0.0, 0.0, 5.0,
                                 1.0, -1.0, -2.0,
                                 -1.0, 1.0, -2.0,
                                 1.0, 1.0, -2.0 };

GLfloat sphereRadius[] = { 0.5,
                           0.6,
                           0.7,
                           0.8 };

GLfloat sphereDiffuse[] = { 1.0, 1.0, 1.0,
                           0.3, 1.0, 0.3,
                           0.3, 0.3, 1.0,
                           1.0, 0.3, 0.3 };

GLfloat sphereAmbient[] = { 0.1, 0.1, 0.1,
                            0.0, 0.0, 0.0,
                            0.0, 0.0, 0.0,
                            0.0, 0.0, 0.0 };

GLfloat sphereSpecular[] = { 0.0, 0.0, 0.0,
                             0.0, 0.0, 0.0,
                             0.0, 0.0, 0.0,
                             0.0, 0.0, 0.0 };

GLfloat sphereShininess[] = { 1.0,
                              1.0,
                              1.0,
                              1.0 };

GLfloat lightPositions[] = { 1.0, 1.0, 10.0 };
GLfloat lightDiffuse[] = { 1.0, 1.0, 1.0 };
GLfloat lightSpecular[] = { 1.0, 1.0, 1.0 };

int numTriangles = 0;
std::vector<vec3> trianglePoints;

int numOfBoundingSpheres = 0;
std::vector<GLfloat> bufferData;

int numOfTriangleVectors = 10;
int numOfTrianglesBounded = 12;

/**
 * Redraw the scene.
 */
void idle( void ) {
  glutPostRedisplay();
}

/** current Height of the screen. **/
int screenHeight;
/** current Width of the screen. **/
int screenWidth;

/**
 * Screen resize handler.
 * @param width New screen width.
 * @param height New screen height.
 */
void reshape( int width, int height ) {
  screenHeight = height;
  screenWidth = width;
}

/**
 * Keyboard Conrol Handler.
 * @param key The key that was pressed.
 * @param x X coordinate of the mouse when a key was pressed.
 * @param y Y coordinate of the mouse when a key was pressed.
 */
void customkeyboard( unsigned char key, int x, int y ) {
  switch ( key ) {
  case 033: // Escape Key
  case 'q':
  case 'Q':
    exit( EXIT_SUCCESS );
    break;
  case 'w': // move up
    camera.moveCamera( 0.0, 0.2, 0.0 );
    break;
  case 's': // move down
    camera.moveCamera( 0.0, -0.2, 0.0 );
    break;
  case 'a': // move left
    camera.moveCamera( -0.2, 0.0, 0.0 );
    break;
  case 'd': // move right
    camera.moveCamera( 0.2, 0.0, 0.0 );
    break;
  case 'z': //move back
    camera.moveCamera( 0.0, 0.0, 0.2 );
    break;
  case 'x': //move forward
    camera.moveCamera( 0.0, 0.0, -0.2 );
    break;
  case '1': //move somewhere
    camera.reset();
    camera.moveCamera( 0.0, 0.0, -4.0 );
    //camera.rotateCamera(0.0, 90.0, 0.0);
    break;
  case ' ': // reset values to their defaults
    camera.reset();
    break;
  }
}

/** Is the left mouse button being pressed? **/
bool leftDown = false;
/** Is the right mouse button being pressed? **/
bool rightDown = false;

int mouseDownX, mouseDownY;
/** @var mouseDownX
 * Last known x coordinate of the mouse pointer.
 */
/** @var mouseDownY
 * Last known y coordinate of the mouse pointer.
 */

/**
 * Mouse click handler.
 * @param button The mouse button that was pressed.
 * @param state The current state of that button.
 * @param x X coordinate of the mouse.
 * @param y Y coordinate of the mouse.
 */
void custommouse( int button, int state, int x, int y ) {
  if ( state == GLUT_DOWN ) {
    switch ( button ) {
    case GLUT_LEFT_BUTTON:
      leftDown = true;
      mouseDownX = x;
      mouseDownY = y;
      break;
    case GLUT_MIDDLE_BUTTON:
      break;
    case GLUT_RIGHT_BUTTON:
      rightDown = true;
      mouseDownX = x;
      mouseDownY = y;
      break;
    }
  } else if ( state == GLUT_UP ) {
    switch ( button ) {
    case GLUT_LEFT_BUTTON:
      leftDown = false;
      break;
    case GLUT_MIDDLE_BUTTON:
      break;
    case GLUT_RIGHT_BUTTON:
      rightDown = false;
      break;
    }
  }
}

/** Magic constant to adjust the rate of rotation when re-orienting the camera. **/
float rotationFactor = 45.0;
/** Magic constant to adjust the rate of translation when re-orienting the camera. **/
float translationFactor = 5.0;

/**
 * Handle mouse and camera movement.
 * @param x X coordinate of the mouse.
 * @param y Y coordinate of the mouse.
 */
void motion( int x, int y ) {
  
  float xAngle = 0, yAngle = 0;
  float temp;
  float depth;
  
  if ( leftDown ) {
    temp = (x - mouseDownX);
    mouseDownX = x;
    yAngle = (temp * rotationFactor / screenWidth);
    
    temp = (y - mouseDownY);
    mouseDownY = y;
    xAngle = (temp * rotationFactor / screenHeight);
    
    camera.rotateCamera( xAngle, yAngle, 0.0 );
    
  } else if ( rightDown ) {
    temp = (y - mouseDownY);
    mouseDownY = y;
    depth = (temp * translationFactor / screenHeight);
    
    camera.moveCamera( 0.0, 0.0, depth );
  }
}

int frameCount = 0;
float previousTime = 0.0;

/**
 * Handle the re-display of the scene.
 */
void display( void ) {
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  tick.sendTime();
  
  cameraLeft.copyCamera(&camera);
  cameraLeft.moveCamera(-0.1, 0.0, 0.0);

  cameraRight.copyCamera(&camera);
  cameraRight.moveCamera(0.1, 0.0, 0.0);

  int numSpheres = 1;
  glUniform1i( uNumOfSpheres, numSpheres );
  glUniform3fv( uSphereCenterPoints, numSpheres, sphereCenterPoints );
  glUniform1fv( uSphereRadius, numSpheres, sphereRadius );
  glUniform3fv( uSphereDiffuse, numSpheres, sphereDiffuse );
  glUniform3fv( uSphereAmbient, numSpheres, sphereAmbient );
  glUniform3fv( uSphereSpecular, numSpheres, sphereSpecular );
  glUniform1fv( uSphereShininess, numSpheres, sphereShininess );
  
  glUniform1i( uNumOfTriangle, numTriangles );
  glUniform1i( uNumOfTriangleVectors, numOfTriangleVectors );

  glUniform1i( uNumOfBoundingSpheres, numOfBoundingSpheres );
  glUniform1i( uNumOfTrianglesBounded, numOfTrianglesBounded );

  // random number between 0 and 1
  float lightness = (float)rand()/(float)RAND_MAX;
  // between 0 and .3
  lightness = lightness * 3.0 / 10.0;

  lightness += .7;
  lightDiffuse[0] = lightness;
  lightDiffuse[1] = lightness;
  lightDiffuse[2] = lightness;

  glUniform3fv( uLightPositions, 1, lightPositions );
  glUniform3fv( uLightDiffuse, 1, lightDiffuse );
  glUniform3fv( uLightSpecular, 1, lightSpecular );

  GLfloat vertices[] = { 1.0, 1.0,
                         -1.0, 1.0,
                         1.0, -1.0,
                         -1.0, -1.0, };
  
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
  
  glUniform1i( uDisplay, -1 );
  glUniformMatrix4fv( uRotationMatrix, 1, GL_TRUE, cameraLeft.getRotationMatrix() );
  glUniform4fv( uCameraPosition, 1, cameraLeft.getCameraPosition() );
  glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );

  glUniform1i( uDisplay, 1 );
  glUniformMatrix4fv( uRotationMatrix, 1, GL_TRUE, cameraRight.getRotationMatrix() );
  glUniform4fv( uCameraPosition, 1, cameraRight.getCameraPosition() );
  glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );

  glutSwapBuffers();
  glDisableVertexAttribArray( vRayPosition );

  frameCount++;
  if(frameCount == 10) {
    float elapsedTime = glutGet( GLUT_ELAPSED_TIME );
    float fps = 10.0 / ((elapsedTime - previousTime) / 1000.0);
    printf("fps: %f\n", fps);
    previousTime = elapsedTime;
    frameCount = 0;
  }
}

void addTriangle(const vec3& a, const vec3& b, const vec3& c, const vec3& diffuse, const vec3& ambient, const vec3& specular, float shininess) {
  trianglePoints.push_back(a);
  trianglePoints.push_back(b);
  trianglePoints.push_back(c);

  bufferData.push_back(a.x);
  bufferData.push_back(a.y);
  bufferData.push_back(a.z);

  bufferData.push_back(b.x);
  bufferData.push_back(b.y);
  bufferData.push_back(b.z);

  bufferData.push_back(c.x);
  bufferData.push_back(c.y);
  bufferData.push_back(c.z);

  bufferData.push_back(diffuse.x);
  bufferData.push_back(diffuse.y);
  bufferData.push_back(diffuse.z);

  bufferData.push_back(ambient.x);
  bufferData.push_back(ambient.y);
  bufferData.push_back(ambient.z);

  bufferData.push_back(specular.x);
  bufferData.push_back(specular.y);
  bufferData.push_back(specular.z);

  bufferData.push_back(shininess);
  bufferData.push_back(0.0);
  bufferData.push_back(0.0);

  vec3 normal = normalize(cross(b - a, c - b));
  bufferData.push_back(normal.x);
  bufferData.push_back(normal.y);
  bufferData.push_back(normal.z);

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

  bufferData.push_back(centerX);
  bufferData.push_back(centerY);
  bufferData.push_back(centerZ);
  bufferData.push_back(distance);
  bufferData.push_back(distance * distance);
  bufferData.push_back(0.0);

  numTriangles++;
}

void addCube(vec3 position, vec3 diffuse) {
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

  vec3 ambient = vec3(0.0, 0.0, 0.0);
  vec3 specular = vec3(0.0, 0.0, 0.0);

  //front
  addTriangle(vertices[1] + position, vertices[0] + position, vertices[3] + position, diffuse, ambient, specular, 1.0);
  addTriangle(vertices[1] + position, vertices[3] + position, vertices[2] + position, diffuse, ambient, specular, 1.0);

  //back
  addTriangle(vertices[6] + position, vertices[7] + position, vertices[4] + position, diffuse, ambient, specular, 1.0);
  addTriangle(vertices[6] + position, vertices[4] + position, vertices[5] + position, diffuse, ambient, specular, 1.0);

  //right
  addTriangle(vertices[2] + position, vertices[3] + position, vertices[7] + position, diffuse, ambient, specular, 1.0);
  addTriangle(vertices[2] + position, vertices[7] + position, vertices[6] + position, diffuse, ambient, specular, 1.0);

  //left
  addTriangle(vertices[5] + position, vertices[4] + position, vertices[0] + position, diffuse, ambient, specular, 1.0);
  addTriangle(vertices[5] + position, vertices[0] + position, vertices[1] + position, diffuse, ambient, specular, 1.0);

  //top
  addTriangle(vertices[5] + position, vertices[1] + position, vertices[2] + position, diffuse, ambient, specular, 1.0);
  addTriangle(vertices[5] + position, vertices[2] + position, vertices[6] + position, diffuse, ambient, specular, 1.0);

  //bottom
  addTriangle(vertices[0] + position, vertices[4] + position, vertices[7] + position, diffuse, ambient, specular, 1.0);
  addTriangle(vertices[0] + position, vertices[7] + position, vertices[3] + position, diffuse, ambient, specular, 1.0);
}

void pushDataToBuffer() {
  unsigned int i = 0;
  int groupSize = numOfTrianglesBounded * 3;
  while(i < trianglePoints.size()) {

    vec3 centerPoint;

    int j;
    for(j = 0; j < groupSize && i < trianglePoints.size(); j++) {
      centerPoint += trianglePoints.data()[i++];
    }

    centerPoint.x /= j;
    centerPoint.y /= j;
    centerPoint.z /= j;

    bufferData.push_back(centerPoint.x);
    bufferData.push_back(centerPoint.y);
    bufferData.push_back(centerPoint.z);

    //printf("%f, %f, %f\n", centerPoint.x, centerPoint.y, centerPoint.z);

    i -= j;

    float distance = 0.0;
    for(j = 0; j < groupSize && i < trianglePoints.size(); j++) {

      vec3 point = trianglePoints.data()[i++];
      float x = centerPoint.x - point.x;
      float y = centerPoint.y - point.y;
      float z = centerPoint.z - point.z;

      float tempDistance = sqrtf((x * x) + (y * y) + (z * z));

      if(tempDistance > distance) distance = tempDistance;
    }
    distance += 0.0001;

    bufferData.push_back(distance);
    bufferData.push_back(distance * distance);
    bufferData.push_back(0.0);

    //printf("distance %f\n", distance);
    numOfBoundingSpheres++;
  }

  printf("numTriangles %d\n", numTriangles);
  printf("numOfBoundingSpheres %d\n", numOfBoundingSpheres);

  GLuint bufObj;
  glActiveTexture(GL_TEXTURE0);
  glGenBuffers(1, &bufObj);
  glBindBuffer(GL_TEXTURE_BUFFER, bufObj);
  // Size of float * number of floats in a vector * number of vectors in a triangle (a, b, c, color) * number of triangles
  GLsizeiptr triangleSize = 3 * numOfTriangleVectors * numTriangles;
  GLsizeiptr boundingSphereSize = 3 * 2 * numOfBoundingSpheres;
  glBufferData(GL_TEXTURE_BUFFER, sizeof(GLfloat) * (triangleSize + boundingSphereSize), bufferData.data(), GL_STATIC_DRAW);
  glTexBuffer(GL_TEXTURE_BUFFER, GL_RGB32F, bufObj);
}

void genereateScene() {

  vec3 ambient = vec3(0.0, 0.0, 0.0);
  vec3 specular = vec3(0.0, 0.0, 0.0);

  Object *bottle = new Object("", -1);
  ObjLoader::loadModelFromFile( bottle, "../models/bottle-b.obj" );

  vec4 *vertices = bottle->_vertices.data();
  vec4 *colors = bottle->_colors.data();
  unsigned long int count = 0;
  while(count < bottle->_vertices.size()) {
    vec4 vertex = vertices[count++];
    vec3 a = vec3(vertex.x, vertex.y, vertex.z);
    vertex = vertices[count++];
    vec3 b = vec3(vertex.x, vertex.y, vertex.z);
    vertex = vertices[count++];
    vec3 c = vec3(vertex.x, vertex.y, vertex.z);

    vec4 color = colors[count - 1];

    addTriangle(a, b, c, vec3(color.x, color.y, color.z), ambient, specular, 1.0);
  }

  addTriangle(vec3(-10.0, -10.0, -6.0), vec3(10.0, -10.0, -6.0), vec3(10.0, 10.0, -6.0), vec3(1.0, 1.0, 1.0), ambient, vec3(1.0, 1.0, 1.0), 10.0);
  addTriangle(vec3(-10.0, -10.0, -6.0), vec3(10.0, 10.0, -6.0), vec3(-10.0, 10.0, -6.0), vec3(0.0, 0.0, 1.0), ambient, vec3(0.0, 0.0, 1.0), 10.0);

/*
  vec3 colors[] = {vec3(1.0, 1.0, 0.0), vec3(1.0, 0.0, 1.0), vec3(0.0, 1.0, 1.0)};

  int colorIndex = 0;
  float xPos, yPos, zPos;
  for(xPos = -6; xPos < 6.1; xPos += 2) {
    for(yPos = -6; yPos < 6.1; yPos += 2) {
      for(zPos = -6; zPos < 6.1; zPos += 2) {
        colorIndex++;
        colorIndex %= 3;
        addCube(vec3(xPos, yPos, zPos), colors[colorIndex]);
      }
    }
  }
*/
  /*
  addTriangle(vec3(-5.0, -5.0, -6.0), vec3(5.0, -5.0, -6.0), vec3(5.0, 5.0, -6.0), vec3(1.0, 0.0, 0.0));
  addTriangle(vec3(-5.0, -5.0, -6.0), vec3(5.0, 5.0, -6.0), vec3(-5.0, 5.0, -6.0), vec3(0.0, 1.0, 0.0));
   */
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
  GLuint program = Angel::InitShader( "shaders/vShaderOrgAndDir.glsl",
                                      "shaders/fShaderSpheres2.glsl" );
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
  
  uNumOfTriangle = glGetUniformLocation( program, "uNumOfTriangle" );
  uNumOfTriangleVectors = glGetUniformLocation( program, "uNumOfTriangleVectors" );

  uNumOfBoundingSpheres = glGetUniformLocation( program, "uNumOfBoundingSpheres" );
  uNumOfTrianglesBounded = glGetUniformLocation( program, "uNumOfTrianglesBounded" );

  uLightPositions = glGetUniformLocation( program, "uLightPositions" );
  uLightDiffuse = glGetUniformLocation( program, "uLightDiffuse" );
  uLightSpecular = glGetUniformLocation( program, "uLightSpecular" );

  tick.setTimeUniform(glGetUniformLocation( program, "ftime" ));
  
  glShadeModel( GL_FLAT );
  glEnable( GL_DEPTH_TEST );
  glClearColor( 0.1, 0.1, 0.1, 1.0 );

  genereateScene();
}

/**
 * Simple raytracer demo by Hoanh Nguyen.
 * @param argc Not used.
 * @param argv Not used.
 * @return 0.
 */
int main( int argc, char **argv ) {
  Util::InitRelativePaths(argc, argv);
  glutInit( &argc, argv );
  
  glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE ); // set rendering context
  glutInitWindowSize( 900, 450 );
  glutCreateWindow( "Project" ); // title
      
  glewInit(); // set OpenGL state and initialize shaders
  init();
  
  glutDisplayFunc( display ); // register callback w/Window System
  glutKeyboardFunc( customkeyboard );
  glutMouseFunc( custommouse );
  glutMotionFunc( motion );
  glutIdleFunc( idle );
  glutReshapeFunc( reshape );
  
  glutMainLoop();
  return 0;
}
