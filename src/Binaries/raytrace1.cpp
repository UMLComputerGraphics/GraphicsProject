#include <cmath>
#include <iostream>
#include <cstdio>
#include <SOIL/SOIL.h>
#include <cstring>

#include "SpelchkCamera.hpp"
#include "InitShader.hpp"

GLuint program;

typedef Angel::vec4 color4;
typedef Angel::vec4 point4;

GLuint modelView = -1;  // model-view matrix uniform shader variable location

GLint vRayPosition = -1;
GLint uCameraPosition = -1;

Camera2 camera(vec4(0.0, 0.0, 0.0, 0.0));
vec3 cameraPositionVec = vec3(0.0, 0.0, 0.0);

//----------------------------------------------------------------------------

void idle(void) {
  glutPostRedisplay();
}

int screenHeight;
int screenWidth;
void reshape(int width, int height) {
  screenHeight = height;
  screenWidth = width;
}

void customkeyboard(unsigned char key, int x, int y) {
  switch (key) {
    case 033: // Escape Key
    case 'q':
    case 'Q':
      exit(EXIT_SUCCESS);
      break;
    case 'w': // move up
      camera.moveCamera(0.0, -0.2, 0.0);
      cameraPositionVec.y += 0.1;
      break;
    case 's': // move down
      camera.moveCamera(0.0, 0.2, 0.0);
      cameraPositionVec.y -= 0.1;
      break;
    case 'a': // move left
      camera.moveCamera(0.2, 0.0, 0.0);
      cameraPositionVec.x -= 0.1;
      break;
    case 'd': // move right
      camera.moveCamera(-0.2, 0.0, 0.0);
      cameraPositionVec.x += 0.1;
      break;
    case 'z': //move back
      camera.moveCamera(0.0,0.0,-0.2);
      cameraPositionVec.z += 0.1;
      break;
    case 'x': //move forward
      camera.moveCamera(0.0,0.0,0.2);
      cameraPositionVec.z -= 0.1;
      break;
    case ' ': // reset values to their defaults
      camera.reset();
      break;
  }
}

bool leftDown = false;
bool rightDown = false;

int mouseDownX, mouseDownY;

void custommouse(int button, int state, int x, int y) {
  if (state == GLUT_DOWN) {
    switch (button) {
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
  } else if (state == GLUT_UP) {
    switch (button) {
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

float rotationFactor = 45.0;
float translationFactor = 5.0;
void motion(int x, int y) {

  float xAngle = 0, yAngle = 0;
  float temp;
  float depth;

  if (leftDown) {
    temp = (x - mouseDownX);
    mouseDownX = x;
    yAngle = (temp * rotationFactor / screenWidth);

    temp = (y - mouseDownY);
    mouseDownY = y;
    xAngle = (temp * rotationFactor / screenHeight);

    camera.rotateCamera(xAngle, yAngle, 0.0);

  } else if (rightDown) {
    temp = (y - mouseDownY);
    mouseDownY = y;
    depth = (temp * translationFactor / screenHeight);

    camera.moveCamera(0.0, 0.0, depth);
  }
}

void display(void) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  mat4 mv = camera.getModelViewMatrix();
  glUniformMatrix4fv(modelView, 1, GL_TRUE, mv);

  float elapsedTime = glutGet(GLUT_ELAPSED_TIME);
  //printf("%f\n", elapsedTime);

  glUniform3fv(uCameraPosition, 1, cameraPositionVec);

  GLfloat vertices[] = {
      1.0,  1.0,
     -1.0,  1.0,
      1.0, -1.0,
     -1.0, -1.0,
  };
  GLuint vbo_vertices;
  glGenBuffers(1, &vbo_vertices);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
  glEnableVertexAttribArray(vRayPosition);
  glVertexAttribPointer(
    vRayPosition,  // attribute
    2,                  // number of elements per vertex, here (x,)
    GL_FLOAT,           // the type of each element
    GL_FALSE,           // take our values as-is
    0,                  // no extra data between each position
    0                   // offset of first element
  );

  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  glutSwapBuffers();

  glDisableVertexAttribArray(vRayPosition);
}

void init(void) {

  // Create a vertex array object
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // Load shaders and use the resulting shader program
  GLuint program = Angel::InitShader("shaders/vShaderOrgAndDir.glsl", "shaders/fShaderSpheres2.glsl");
  glUseProgram(program);

  modelView = glGetUniformLocation(program, "ModelView");

  vRayPosition = glGetAttribLocation(program, "vRayPosition");
  uCameraPosition = glGetUniformLocation(program, "uCameraPosition");

  glShadeModel(GL_FLAT);
  glEnable(GL_DEPTH_TEST);
  glClearColor(0.1, 0.1, 0.1, 1.0);
}

int main(int argc, char **argv) {
  glutInit(&argc, argv);

  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE); // set rendering context
  glutInitWindowSize(768, 768);
  glutCreateWindow("Project"); // title

  glewInit(); // set OpenGL state and initialize shaders
  init();

  glutDisplayFunc(display); // register callback w/Window System
  glutKeyboardFunc(customkeyboard);
  glutMouseFunc(custommouse);
  glutMotionFunc(motion);
  glutIdleFunc(idle);
  glutReshapeFunc(reshape);

  glutMainLoop();
  return 0;
}
