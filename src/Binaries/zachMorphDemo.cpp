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
#include "modelFunctions.hpp"
#include "meshMapping.hpp"
#include "bipartiteGraph.hpp"

// Initialization: load and compile shaders, initialize camera(s), load models.
void init() {
  
  // Get handles to the Scene and the Screen.
  Scene *rootScene = Engine::instance()->rootScene();

  Object *bottle = rootScene->addObject( "bottle" );

  ObjLoader::loadModelFromFile( bottle, "../models/bottle-a.obj" );

  bottle->genMorphTarget() ;
  Object *bottleMorphTarget = bottle->morphTarget();
  ObjLoader::loadModelFromFile( bottleMorphTarget, "../models/bottle-b.obj" );

  printf("Number Vertices Model1: %lu\n",bottle->numberOfPoints());
  printf("Number Vertices Model2: %lu\n\n",bottleMorphTarget->numberOfPoints());

  /*
  Angel::vec3 OriginalLowBoundSrc = bottle->getMin();
  Angel::vec3 OriginalMaxBoundSrc = bottle->getMax();
  Angel::vec3 OriginalLowBoundDst = bottleMorphTarget->getMin();
  Angel::vec3 OriginalMaxBoundDst = bottleMorphTarget->getMax();
  */
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
  printf("Number Vertices Model1: %lu\n",bottle->numberOfPoints());
  printf("Number Vertices Model2: %lu\n\n",bottleMorphTarget->numberOfPoints());
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

  // Generic OpenGL setup: Enable the depth buffer and set a nice background color.
  glClearColor( 0.3, 0.5, 0.9, 1.0 );

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

  /* PULL THE TRIGGER */
  glutMainLoop();
  return EXIT_SUCCESS;

}
