/**
   @file Cameras.cpp
   @author John Huston
   @authors John Huston, Nicholas StPierre, Chris Compton
   @date 2012-12-04
   @brief Implementation for the Cameras class, which is a container for Camera objects.
**/


#include <cmath>
#include <vector>
#include "Camera.hpp"
#include "Cameras.hpp"
#include "globals.h"

/**
   Default constructor. Nothing special.
**/
Cameras::Cameras( void ) {
  this->Size = Angel::vec2( 0, 0 );
}

/**
   Default destructor. Nothing special here, either.
**/
Cameras::~Cameras( void ) { }

/**
   AddCamera takes a name for a camera and returns a handle to a newly created camera.

   @param name The name of the new camera to create.

   @return A Pointer to a newly created Camera object.
**/
Camera *Cameras::AddCamera( const std::string &name ) {

  Camera *cam = new Camera( name, gShader );
  Scene::InsertObject( name, cam );
  CalculateViewports();
  return cam;
  //Set Active Camera?

}

size_t Cameras::NumCameras( void ) const {
  return list.size();
}

//DelCamera( const std::string &name );
//DelCamera( void );
//DestroyCamera( void );

void Cameras::PopCamera( void ) {
  Scene::PopObject();
  CalculateViewports();
}

Camera *Cameras::Obj2Cam( std::list< Object* >::iterator &it ) {
  return dynamic_cast< Camera* >( *it );
}

Camera *Cameras::Active( void ) {
  return Obj2Cam( currentObj );
}

Camera *Cameras::Prev( void ) {
  Scene::Prev();
  Active()->Send( Camera::CTM );
  return Active();
}

Camera *Cameras::Next( void ) {
  Scene::Next();
  Active()->Send( Camera::CTM );
  return Active();
}

void Cameras::IdleMotion( void ) {
  std::list<Object*>::iterator it;
  for (it = list.begin(); it != list.end(); ++it) {
    Obj2Cam(it)->Idle();
  }
}

void Cameras::View(void (*draw_func)(void)) {
  std::list< Object* >::iterator it;
  for (it = list.begin(); it != list.end(); ++it) {
    Obj2Cam(it)->View();
    (*draw_func)();
  }
}

void Cameras::Resize( int width, int height ) {
  this->Size = Angel::vec2( width, height );
  if (DEBUG)
    std::cerr << "Setting Cameras WxH: " << width << "x" << height << "\n";
  CalculateViewports();
}

void Cameras::CalculateViewports( void ) {

  unsigned Width = Size.x;
  unsigned Height = Size.y;

  /* Let's not try to distribute Viewports if the window system
     hasn't been initialized yet, i.e, if Resize() has not yet
     been called. */
  if (!Size.x || !Size.y) return;
  
  // How many cameras do we have?
  size_t numCameras = NumCameras();

  // Let's not try to resize zero cameras.
  if (numCameras == 0) return;

  // How many virtual rows will we need to display this many?
  size_t numRows = ceil(sqrt(numCameras));

  // How many columns will we need (average) in general to display this many?
  double numCols = (double)numCameras/(double)numRows;
  size_t numMaxCols = ceil(numCols);
  size_t numMinCols = floor(numCols);

  // How many rows do we need to draw with MinCols?
  // (By extension: drawMaxRows = (numRows-drawMinRows))
  size_t drawMinRows = (numCameras % numRows) ?
    ((int)(numCameras - (numRows * numMaxCols)) / (int)(numMinCols - numMaxCols)) :
    0;

  std::list< Object * >::iterator it = list.begin();

  /* This is confusing as hell. Good luck! */
  for (size_t allocHeight = 0, row = 0;
       (row < numRows) || (it != list.end()); //Terminate on either cond. Just in case.
       ++row) {
    size_t myWidth;
    size_t myHeight;
    size_t allocWidth = 0;
    size_t colsThisRow = (row < drawMinRows) ? numMinCols : numMaxCols;
    if (0) fprintf( stderr, "Row: %lu; Columns this row: %lu\n", row, colsThisRow );

    for (size_t col = 0; col < colsThisRow; ++col, ++it) {
      // Is this the last column? Use the remaining width.
      if (col + 1 == colsThisRow) myWidth = (Width) - allocWidth;
      else myWidth = (Width)/colsThisRow;
      // Is this the last row? Use the remaining height.
      if (row + 1 == numRows) myHeight = (Height) - allocHeight;
      else myHeight = (Height)/numRows;

      // Tell this camera his new viewport.
      // height looks a little goofy because we are allocating height
      // from the top of the coordinate system and working down,
      // so we have to take the complement.
      Obj2Cam(it)->viewport( allocWidth, ((Height)-(allocHeight+myHeight)), myWidth, myHeight );
      if (0) fprintf( stderr, "Camera: (%lu x %lu) @ (%lu,%lu)\n",
		      myWidth, myHeight,
		      allocWidth, ((Height)-(allocHeight+myHeight)));

      // Increment our allocated width counter.
      allocWidth += myWidth;
    }
    // Increment our allocated height counter.
    allocHeight += ((Height)/numRows);
  }
}

void Cameras::TellMeYourSecrets( void ) {
  std::list<Object *>::iterator it;
  for ( it = list.begin(); it != list.end(); ++it ) {
    fprintf( stderr, "Camera: [%s]; Ptr: [%p]\n",
	     Obj2Cam(it)->Name().c_str(),
	     (void*)(*it) );
  }

  std::map<std::string, Object*>::iterator it2;
  for (it2 = map.begin(); it2 != map.end(); ++it2 ) {
    fprintf( stderr, "[M]Camera: {%p}[%s]-->[%s]\n",
	     (void*)(it2->second),
	     it2->first.c_str(),
	     "..."
	     /*((*it2).second)->Name().c_str()*/ );
  }
}
