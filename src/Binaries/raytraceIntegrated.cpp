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

/**
 * Not-So-Simple raytracer demo by Hoanh Nguyen.
 * @param argc Not used.
 * @param argv Not used.
 * @return 0.
 */
int main( int argc, char **argv ) {
  RayTracer rt;
  Engine::init( &argc, argv, "Raytracer" );
  Engine::instance()->cams()->active()->z(10);

  GLCHECK();

  GLuint program = Angel::InitShader( "shaders/vRaytracer.glsl",
             "shaders/fRaytracer.glsl" );

  Scene *rootScene = Engine::instance()->rootScene();
  Screen *primScreen = Engine::instance()->mainScreen();

  Object *bottle = rootScene->addObject( "bottle", program );
  ObjLoader::loadModelFromFile( bottle, "../models/bottle_wine_high.obj" );
  ObjLoader::loadMaterialFromFile( bottle, "../models/bottle_wine_high.mtl" );

  Engine::instance()->registerDisplayFunc(rt.display);
  rt.init(program);
  std::vector<Object *> objs;
  //ITERATE OVER ALL OBJS IN SCENE!

  Engine::instance()->rootScene()->bufferToRaytracer( rt );
  rt.pushDataToBuffer();
  
  glutMainLoop();

  return 0;
}
