/**
 @file zachMorphDemo.cpp
 @author Zach Maybury
 @authors Nicholas St.Pierre, John Huston, Nicholas VerVoort, Chris Compton
 @date 2013-03-28
 @brief Zach's Bottle Morphing Demo.
 @details This is a tech demo for morphing two objects back and forth.
 The main driver code is based on Ed Angel's code from his book.
 **/

#include "Engine.hpp"
/* System Headers */
#include <cmath>
#include <cstdio>
#include <sstream>
#include <cstdlib>
#include <time.h>
/* Multi-platform support and OpenGL headers. */
/* Utilities and Common */
#include "model.hpp"
#include "modelFunctions.hpp"
#include "meshMapping.hpp"
#include "bipartiteGraph.hpp"
#include "InitShader.hpp"
#include "glut_callbacks.h"
#include "ObjLoader.hpp"

// Initialization: load and compile shaders, initialize camera(s), load models.
void init() {
  
  // Get handles to the Scene and the Screen.
  Scene *rootScene = Engine::instance()->rootScene();
  Screen *primScreen = Engine::instance()->mainScreen();

  // Load shaders and use the resulting shader program. 
  GLuint gShader = Angel::InitShader("shaders/vmorph.glsl","shaders/fmorph.glsl");

  // Let the other objects know which shader to use by default.
  rootScene->shader( gShader );
  primScreen->_camList.shader( gShader );

  // We start with no cameras, by default. Add one and set it "active" by using Next().
  primScreen->_camList.addCamera( "Camera1" );
  primScreen->_camList.next();

  // Create an object and add it to the scene with the name "bottle".
  Object *bottle = rootScene->addObject( "bottle" );

  // Use the object loader to actually fill out the vertices and-so-on of the bottle.
  ObjLoader::loadModelFromFile( bottle, "../models/bottle-a.obj" );

  // Objects has-a pointer to an object which is their "morph target."
  // they are created and buffered as follows:

  bottle->genMorphTarget( gShader ) ; // this makes a new object and links it to the source object. it returns the addr of the new obj..
  Object *bottleMorphTarget = bottle->morphTarget() ; // we can get the addr of the morph object like this, also.
  ObjLoader::loadModelFromFile( bottleMorphTarget, "../models/bottle-b.obj" ); // with this model, we can use all the preexisting Object class functionality

  printf("Number Vertices Model1: %d\n",bottle->numberOfPoints());
  printf("Number Vertices Model2: %d\n\n",bottleMorphTarget->numberOfPoints());

  Angel::vec3 OriginalLowBoundSrc = bottle->getMin();
  Angel::vec3 OriginalMaxBoundSrc = bottle->getMax();
  Angel::vec3 OriginalLowBoundDst = bottleMorphTarget->getMin();
  Angel::vec3 OriginalMaxBoundDst = bottleMorphTarget->getMax();
  std::vector< Angel::vec3> originalBounds;
  //Scale source and destination height to unit 0-1
  //scaleObjectsToUnitHeight(bottle, bottleMorphTarget);
  
  
  Angel::vec3 lowBoundSrc = bottle->getMin();
  Angel::vec3 maxBoundSrc = bottle->getMax();
  Angel::vec3 lowBoundDst = bottleMorphTarget->getMin();
  Angel::vec3 maxBoundDst = bottleMorphTarget->getMax();
  
  //std::cout << "Model1 Bounds: " << lowBoundSrc << " " << maxBoundSrc << std::endl;
  //std::cout << "Model2 Bounds: " << lowBoundDst << " " << maxBoundDst << std::endl;
  
/*
  SquareMap* squareMap = createSquareMap(fminf(float(lowBoundSrc.x),
					       float(lowBoundDst.x)),
					 fmaxf(float(maxBoundSrc.x),
					       float(maxBoundDst.x)),
					 fminf(float(lowBoundSrc.y),
					       float(lowBoundDst.y)),
					 fmaxf(float(maxBoundSrc.y),
					       float(maxBoundDst.y)),
					 fminf(float(lowBoundSrc.z),
						   float(lowBoundDst.z)),
					 fmaxf(float(maxBoundSrc.z),
						   float(maxBoundDst.z)));
	*/
	
	//create Bipartite Graph
	//BipartiteGraph * bipartiteGraph = new BipartiteGraph(bottle, bottleMorphTarget);
	//segment parts of bottle to determine problem areas
	//splitProblemTriangles(bottle, bottleMorphTarget);
	segmentModels(bottle, lowBoundSrc, maxBoundSrc, bottleMorphTarget, lowBoundDst, maxBoundDst);
  //matchInitialPoints(bottle, bottleMorphTarget);
  printf("Number Vertices Model1: %d\n",bottle->numberOfPoints());
  printf("Number Vertices Model2: %d\n\n",bottleMorphTarget->numberOfPoints());
  //makeModelsSameSize(bottle, bottleMorphTarget);

  //populateSrcSquare(squareMap,bottle->_vertices);
  //populateDestSquare(squareMap,bottleMorphTarget->_vertices);
  //expandSquareMap(squareMap);
/*
  printf("Front Vertices Src:  %d\n",int(squareMap->front->srcTriangles.size()));
  printf("Back  Vertices Src:  %d\n",int(squareMap->back->srcTriangles.size()));
  printf("Left  Vertices Src:  %d\n",int(squareMap->left->srcTriangles.size()));
  printf("Right Vertices Src:  %d\n\n",int(squareMap->right->srcTriangles.size()));

  printf("Front Vertices Dest: %d\n",int(squareMap->front->destTriangles.size()));
  printf("Back  Vertices Dest: %d\n",int(squareMap->back->destTriangles.size()));
  printf("Left  Vertices Dest: %d\n",int(squareMap->left->destTriangles.size()));
  printf("Right Vertices Dest: %d\n",int(squareMap->right->destTriangles.size()));
*/
  //printf("Number Vertices Model1: %d\n",bottle->numberOfPoints());
  //printf("Number Vertices Model2: %d\n",bottleMorphTarget->numberOfPoints());

  bottle->_trans._scale.set( 0.01 );
  bottleMorphTarget->_trans._scale.set( 0.01 );
  bottle->buffer();
  bottle->bufferMorphOnly(); // YES THIS IS THE REAL OBJECT, NOT THE TARGET. IT SENDS THE MORPH VERTICES TO THE SHADER, NOT TO THE DRAW LIST TO BE DRAWN!


  // Generic OpenGL setup: Enable the depth buffer and set a nice background color.
  glEnable( GL_DEPTH_TEST );
  glClearColor( 0.3, 0.5, 0.9, 1.0 );

}

void cleanup( void ) {
  //Engine::instance()->rootScene()->DestroyObject();
}

//--------------------------------------------------------------------

void simpleRotateAnim( TransCache &obj ) {

  obj._rotation.rotateY( tick.scale() * 1.5 );
  obj._offset.set( 1.5, 0, 0 );
  obj._orbit.rotateY( tick.scale() * -1.0 );

}

void zach_idle( void ) {

  static Scene *rootScene = Engine::instance()->rootScene();

  //zach m  - in order to eventually allow for user specified equations,
  //          we need to think of equations between 0 and 1.
  //          The current line "percent = timer" would be the line where
  //          we would handle a user specified equation in the form of
  //          y = x, where y is the percent and x is the timer. We may
  //          want to specify that the y should equal 1 when x equals 1,
  //          otherwise we can get a percent greater than 100.
  static double timer = 0.0 ;
  static bool trigger = false;
  if(trigger == false){
	  if( (timer += 0.0001) >1 ){
		  timer = 1.0;
		  trigger = true;
	  }
  }else{
	  if( (timer -= 0.0001) < 0){
		  timer = 0.0;
		  trigger = false;
	  }
  }
  float percent = timer;

  //if ( (timer += 0.05 ) > 360.0 ) timer = 0.0 ;
  //float percent = ( sin(timer) + 1 ) / 2 ;
  (*rootScene)["bottle"]->morphPercentage(percent);
  
}

int main( int argc, char **argv ) {

  
  Engine::instance()->init( &argc, argv, "Zach's Morphing Demo" );
  Engine::instance()->registerIdle( zach_idle );
  init();

  if (DEBUG) {
    fprintf( stderr, "GL_VENDOR: %s\n", glGetString( GL_VENDOR ));
    fprintf( stderr, "GL_RENDERER: %s\n", glGetString( GL_RENDERER ));
    fprintf( stderr, "GL_VERSION: %s\n", glGetString( GL_VERSION ));
    fprintf( stderr, "GL_SHADING_LANGUAGE_VERSION: %s\n", 
	     glGetString( GL_SHADING_LANGUAGE_VERSION ));
  }

  /* PULL THE TRIGGER */
  glutMainLoop();
  return EXIT_SUCCESS;

}
