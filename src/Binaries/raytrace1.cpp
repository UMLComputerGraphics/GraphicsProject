/**
 * @file raytrace1.cpp
 * @author Hoanh Nguyen
 * @date 2013-03-15
 * @brief Geometric Raytracing Demo
 */

#include <cmath>
#include <iostream>
#include <cstdio>
#include <SOIL/SOIL.h>
#include <cstring>

#include "SpelchkCamera.hpp"
#include "InitShader.hpp"

/** Global shader object **/
GLuint program;

/** model-view matrix uniform shader variable location **/
GLuint modelView = -1;

GLint vRayPosition = -1;
/** Handle to uniform that will contain the position of the Camera. **/
GLint uCameraPosition = -1;

/** Logical camera object. **/
SpelchkCamera camera( vec4( 0.0, 0.0, 0.0, 0.0 ) );
/** Spatial coordinate of camera. **/
vec3 cameraPositionVec = vec3( 0.0, 0.0, 0.0 );

//----------------------------------------------------------------------------

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
    camera.moveCamera( 0.0, -0.2, 0.0 );
    cameraPositionVec.y += 0.1;
    break;
  case 's': // move down
    camera.moveCamera( 0.0, 0.2, 0.0 );
    cameraPositionVec.y -= 0.1;
    break;
  case 'a': // move left
    camera.moveCamera( 0.2, 0.0, 0.0 );
    cameraPositionVec.x -= 0.1;
    break;
  case 'd': // move right
    camera.moveCamera( -0.2, 0.0, 0.0 );
    cameraPositionVec.x += 0.1;
    break;
  case 'z': //move back
    camera.moveCamera( 0.0, 0.0, -0.2 );
    cameraPositionVec.z += 0.1;
    break;
  case 'x': //move forward
    camera.moveCamera( 0.0, 0.0, 0.2 );
    cameraPositionVec.z -= 0.1;
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

/**
 * Handle the re-display of the scene.
 */
void display( void ) {
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  mat4 mv = camera.getModelViewMatrix();
  glUniformMatrix4fv( modelView, 1, GL_TRUE, mv );

  float elapsedTime = glutGet( GLUT_ELAPSED_TIME );
  //printf("%f\n", elapsedTime);

  glUniform3fv( uCameraPosition, 1, cameraPositionVec );

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

  glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );

  glutSwapBuffers();

  glDisableVertexAttribArray( vRayPosition );
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

  modelView = glGetUniformLocation( program, "ModelView" );

  vRayPosition = glGetAttribLocation( program, "vRayPosition" );
  uCameraPosition = glGetUniformLocation( program, "uCameraPosition" );

  glShadeModel( GL_FLAT );
  glEnable( GL_DEPTH_TEST );
  glClearColor( 0.1, 0.1, 0.1, 1.0 );
}

/**
 * Simple raytracer demo by Hoanh Nguyen.
 * @param argc Not used.
 * @param argv Not used.
 * @return 0.
 */
int main( int argc, char **argv ) {
  glutInit( &argc, argv );

  glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE ); // set rendering context
  glutInitWindowSize( 768, 768 );
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
