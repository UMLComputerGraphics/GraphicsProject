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

#define GLCHECK(); if (glGetError()) { gprint( PRINT_ERROR, "glGetError() is true on line %d\n", __LINE__ ); exit(255); }

RayTracer rt;

void aRomanticEvening() {
  rt.lightFlicker();
}

void RTdisplay( void ) {
  rt._display();
}

/**
 * Not-So-Simple raytracer demo by Hoanh Nguyen.
 * @param argc Not used.
 * @param argv Not used.
 * @return 0.
 */
int main( int argc, char **argv ) {

  Engine::init( &argc, argv, "Raytracer" );

  GLint program = Angel::InitShader( "shaders/vRaytracer.glsl",
				     "shaders/fRaytracer.glsl" );
  rt.init( program );
  if (1) {
    Object *bottle = Engine::instance()->rootScene()->addObject( "bottle", program );
    ObjLoader::loadModelFromFile( bottle, "../models/bottle_wine_med.obj" );
    //ObjLoader::loadMaterialFromFile( bottle, "../models/bottle_wine_med.mtl" );
    rt.pushDataToBuffer();
  } else {
    rt.legacySceneGen();
  }

  glutDisplayFunc( RTdisplay ); // register callback w/Window System
  boost::thread zipo( aRomanticEvening );  
  GLCHECK();
  Engine::run();
  printf("This doesnt exit cleanly... woops.\n");
  rt.thisDateIsOver();
  zipo.join();
  
  return EXIT_SUCCESS;
}
