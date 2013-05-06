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

/*
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
  } */

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
  rt.legacySceneGen();

  glutDisplayFunc( RTdisplay ); // register callback w/Window System

  //  boost::thread zipo( aRomanticEvening );
  
  GLCHECK();
  Engine::run();
  
  //extinguish = true;
  //zipo.join();
  
  return 0;
}
