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
#include "RayTracer.h"

#define GLCHECK(); if (glGetError()) { gprint( PRINT_ERROR, "glGetError() is true on line %d\n", __LINE__ ); exit(255); }
#define WTFGL ;GLCHECK();

RayTracer rt;

/** Global shader object **/
GLuint program = -1;

//----------------------------------------------------------------------------

int numSpheres = 3;


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

int numOfL2TrianglesBounded = 20;

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
    if (rt.lightDiffuse)
    {
      rt.lightDiffuse[0] = lightness;
      rt.lightDiffuse[1] = lightness;
      rt.lightDiffuse[2] = lightness;
    }
    
    sleep( 0.01 );
  }
}

/**
 * Handle the re-display of the scene.
 */
void display( void ) {
  rt._display();
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
  rt.addTriangle(vertices[1] + position, vertices[0] + position, vertices[3] + position, diffuse, ambient, specular, shininess, reflect, refract);
  rt.addTriangle(vertices[1] + position, vertices[3] + position, vertices[2] + position, diffuse, ambient, specular, shininess, reflect, refract);

  //back
  rt.addTriangle(vertices[6] + position, vertices[7] + position, vertices[4] + position, diffuse, ambient, specular, shininess, reflect, refract);
  rt.addTriangle(vertices[6] + position, vertices[4] + position, vertices[5] + position, diffuse, ambient, specular, shininess, reflect, refract);

  //right
  rt.addTriangle(vertices[2] + position, vertices[3] + position, vertices[7] + position, diffuse, ambient, specular, shininess, reflect, refract);
  rt.addTriangle(vertices[2] + position, vertices[7] + position, vertices[6] + position, diffuse, ambient, specular, shininess, reflect, refract);

  //left
  rt.addTriangle(vertices[5] + position, vertices[4] + position, vertices[0] + position, diffuse, ambient, specular, shininess, reflect, refract);
  rt.addTriangle(vertices[5] + position, vertices[0] + position, vertices[1] + position, diffuse, ambient, specular, shininess, reflect, refract);

  //top
  rt.addTriangle(vertices[5] + position, vertices[1] + position, vertices[2] + position, diffuse, ambient, specular, shininess, reflect, refract);
  rt.addTriangle(vertices[5] + position, vertices[2] + position, vertices[6] + position, diffuse, ambient, specular, shininess, reflect, refract);

  //bottom
  rt.addTriangle(vertices[0] + position, vertices[4] + position, vertices[7] + position, diffuse, ambient, specular, shininess, reflect, refract);
  rt.addTriangle(vertices[0] + position, vertices[7] + position, vertices[3] + position, diffuse, ambient, specular, shininess, reflect, refract);
}

void genereateScene() {

  //TODO: raytrace1 use class/Raytrace
  Object *bottle = new Object("", program);
  ObjLoader::loadModelFromFile( bottle, "../models/bottle_wine_med.obj" );
  bottle ->bufferToRaytracer(rt);

  addCube(vec3(-2.0, 1.0, -3.0), vec3(0.8, 0.8, 0.8), vec3(0.05, 0.05, 0.05), vec3(0.0, 0.0, 0.0), 1000.0, 0.5, 0.0);
  addCube(vec3(2.0, 1.0, -3.0), vec3(0.8, 0.8, 0.8), vec3(0.05, 0.05, 0.05), vec3(0.0, 0.0, 0.0), 1000.0, 0.5, 0.0);


  rt.addTriangle(vec3(-5.0, -3.0, -5.0), vec3(5.0, -3.0, -5.0), vec3(5.0, 7.0, -5.0), vec3(1.0, 1.0, 1.0), vec3(0.05, 0.05, 0.05), vec3(1.0, 1.0, 1.0), 100.0, 0.0, 0.0);
  rt.addTriangle(vec3(-5.0, -3.0, -5.0), vec3(5.0, 7.0, -5.0), vec3(-5.0, 7.0, -5.0), vec3(0.0, 0.0, 1.0), vec3(0.0, 0.0, 0.05), vec3(0.0, 0.0, 1.0), 100.0, 0.0, 0.0);

  rt.addTriangle(vec3(-5.0, -3.0, -5.0), vec3(-5.0, -3.0, 5.0), vec3(5.0, -3.0, 5.0), vec3(0.0, 1.0, 0.0), vec3(0.0, 0.05, 0.0), vec3(0.0, 1.0, 0.0), 100.0, 0.0, 0.0);
  rt.addTriangle(vec3(-5.0, -3.0, -5.0), vec3(5.0, -3.0, 5.0), vec3(5.0, -3.0, -5.0), vec3(1.0, 0.0, 0.0), vec3(0.05, 0.0, 0.0), vec3(1.0, 0.0, 0.0), 100.0, 0.0, 0.0);


  rt.addTriangle(vec3(-5.0, -3.0, -5.0), vec3(-5.0, 7.0, 5.0), vec3(-5.0, -3.0, 5.0), vec3(1.0, 0.0, 1.0), vec3(0.05, 0.0, 0.05), vec3(1.0, 0.0, 1.0), 100.0, 0.0, 0.0);
  rt.addTriangle(vec3(-5.0, -3.0, -5.0), vec3(-5.0, 7.0, -5.0), vec3(-5.0, 7.0, 5.0), vec3(1.0, 1.0, 0.0), vec3(0.05, 0.05, 0.0), vec3(1.0, 1.0, 0.0), 100.0, 0.0, 0.0);

  rt.pushDataToBuffer(rt.bufferData);
}

/**
 * Initialization of objects and OpenGL state.
 */
void init( void ) {
  // Load shaders and use the resulting shader program
  program = Angel::InitShader( "shaders/vRaytracer.glsl",
             "shaders/fRaytracer.glsl" );

  rt.numOfL2TrianglesBounded = numOfL2TrianglesBounded;
  rt.numOfTriangleVectors = numOfTriangleVectors;
  rt.numberOfLights = numberOfLights;
  rt.lightDiffuse = lightDiffuse;
  rt.lightPositions = lightPositions;
  rt.lightSpecular = lightSpecular;
  rt.numSpheres = numSpheres;
  rt.sphereAmbient = sphereAmbient;
  rt.sphereCenterPoints = sphereCenterPoints;
  rt.sphereDiffuse = sphereDiffuse;
  rt.sphereRadius = sphereRadius;
  rt.sphereReflect = sphereReflect;
  rt.sphereRefract = sphereRefract;
  rt.sphereShininess = sphereShininess;
  rt.sphereSpecular = sphereSpecular;
  rt.init(program);

  genereateScene();
}

/**
 * Not-So-Simple raytracer demo by Hoanh Nguyen.
 * @param argc Not used.
 * @param argv Not used.
 * @return 0.
 */
int main( int argc, char **argv ) {

  Engine::init( &argc, argv, "Raytracer" );
  init();
  glutDisplayFunc( display ); // register callback w/Window System

  boost::thread zipo( aRomanticEvening );
  
  GLCHECK();
  Engine::run();
  
  extinguish = true;
  zipo.join();
  
  return 0;
}
