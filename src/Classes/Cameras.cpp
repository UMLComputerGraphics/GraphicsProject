/**
 @file Cameras.cpp
 @author John Huston
 @authors John Huston, Nicholas StPierre, Chris Compton
 @date 2012-12-04
 @brief Implementation for the Cameras class, which is a container for Camera objects.
 **/

#include <cmath>
#include <vector>
#include <stdexcept>
#include "Camera.hpp"
#include "Cameras.hpp"
#include "globals.h"

/**
 * Default constructor. Nothing special.
 */
Cameras::Cameras( void ) {
  this->_size = Angel::vec2( 0, 0 );
}

/**
 * Default destructor. Nothing special here, either.
 */
Cameras::~Cameras( void ) {
}

/**
 * addCamera takes a name for a camera and returns a handle to a newly created camera.
 *
 * @param name The name of the new camera to create.
 * @return A Pointer to a newly created Camera object.
 */
Camera *Cameras::addCamera( const std::string &name ) {
  
  Camera *cam = new Camera( name, _gShader );

  Scene::insertObject( cam );

  calculateViewports();
  return cam;
  //Set active Camera?
  
}

/**
 * popCamera removes the most recently added Camera from the scene.
 *
 * @return void.
 */
void Cameras::popCamera( void ) {
  Scene::popObject();
  calculateViewports();
}

//DelCamera( const std::string &name );
//DelCamera( void );
//DestroyCamera( void );

/**
 * Sets the active Camera to the next available one in the collection.
 *
 * @return A pointer to the newly active Camera.
 */
Camera *Cameras::next( void ) {
  Scene::next();
  active()->send( Camera::CTM );
  return active();
}

/**
 * Sets the active Camera to the previous available one in the collection.
 *
 * @return A pointer to the newly active Camera.
 */
Camera *Cameras::prev( void ) {
  Scene::prev();
  active()->send( Camera::CTM );
  return active();
}

/**
 * active returns the Camera in the collection that is considered 'active'.
 *
 * @return A pointer to the currently selected, active Camera.
 */
Camera *Cameras::active( void ) const {
  
  // Use the base method to retrieve the active object.
  Object *camptr = Scene::active();
  
  // Convert it to a Camera pointer.
  return dynamic_cast< Camera* >( camptr );
  
}

/**
 * numCameras fetches the number of Cameras in the collection.
 *
 * @return an unsigned integer, the number of Cameras in the collection.
 */
size_t Cameras::numCameras( void ) const {
  return _list.size();
}

/**
 * idleMotion calls the idle method on all child cameras.
 * Intended to be called during the idle() loop in GLUT.
 *
 * @return void.
 */
void Cameras::idleMotion( void ) {
  std::list< Object* >::iterator it;
  for ( it = _list.begin(); it != _list.end(); ++it ) {
    obj2Cam( it )->idle();
  }
}

/**
 * resize informs the Cameras collection of the new window size.
 * Intended to be called from the GLUT main loop.
 * This method also invokes Cameras::calculateViewports.
 *
 * @param width The new window width.
 * @param height The new window height.
 *
 * @return void.
 */
void Cameras::resize( int width, int height ) {
  this->_size = Angel::vec2( width, height );
  if ( DEBUG )
    std::cerr << "Setting Cameras WxH: " << width << "x" << height << "\n";
  calculateViewports();
}

/**
 * For each Camera in the collection, computes the position and size of that
 * Camera's viewport in a split-screen, single-window configuration.
 *
 * @return void.
 */
void Cameras::calculateViewports( void ) {
  
  unsigned Width = _size.x;
  unsigned Height = _size.y;
  
  /*
   * This function is a little confusing.
   * Ultimately, we see how many virtual cameras we have,
   * then we compute how many "rows" and "columns" of virtual viewports
   * we will need to accommodate that many.
   *
   * After those calculations, we run through a loop that goes
   * row-by-row and then column-by-column computing the actual
   * x,y offsets, height and widths of each viewport.
   */

  /* Let's not try to distribute Viewports if the window system
   hasn't been initialized yet, i.e, if resize() has not yet
   been called. */
  if ( !_size.x || !_size.y ) return;
  
  // How many cameras do we have?
  size_t numCameras = Cameras::numCameras();
  
  // Let's not try to resize zero cameras.
  if ( numCameras == 0 ) return;
  
  // How many virtual rows will we need to display this many?
  size_t numRows = ceil( sqrt( numCameras ) );
  
  // How many columns will we need (average) in general to display this many?
  double numCols = (double) numCameras / (double) numRows;
  size_t numMaxCols = ceil( numCols );
  size_t numMinCols = floor( numCols );
  
  // How many rows do we need to draw with MinCols?
  // (By extension: drawMaxRows = (numRows-drawMinRows))
  size_t drawMinRows =
      (numCameras % numRows) ?
          ((int) (numCameras - (numRows * numMaxCols)) / (int) (numMinCols
              - numMaxCols)) :
          0;
  
  std::list< Object * >::iterator it = _list.begin();
  
  /* This is confusing as hell. Good luck! */
  for ( size_t allocHeight = 0, row = 0; (row < numRows) || (it != _list.end());
      ++row ) {
    size_t myWidth;
    size_t myHeight;
    size_t allocWidth = 0;
    size_t colsThisRow = (row < drawMinRows) ? numMinCols : numMaxCols;
    if ( 0 )
      fprintf( stderr, "Row: %lu; Columns this row: %lu\n", row, colsThisRow );
    
    for ( size_t col = 0; col < colsThisRow; ++col, ++it ) {
      // Is this the last column? Use the remaining width.
      if ( col + 1 == colsThisRow ) myWidth = (Width) - allocWidth;
      else myWidth = (Width) / colsThisRow;
      // Is this the last row? Use the remaining height.
      if ( row + 1 == numRows ) myHeight = (Height) - allocHeight;
      else myHeight = (Height) / numRows;
      
      // Tell this camera his new viewport.
      // height looks a little goofy because we are allocating height
      // from the top of the coordinate system and working down,
      // so we have to take the complement.
      obj2Cam( it )->viewport( allocWidth,
                               ((Height) - (allocHeight + myHeight)), myWidth,
                               myHeight );
      if ( 0 )
        fprintf( stderr, "Camera: (%lu x %lu) @ (%lu,%lu)\n", myWidth, myHeight,
                 allocWidth, ((Height) - (allocHeight + myHeight)) );
      
      // Increment our allocated width counter.
      allocWidth += myWidth;
    }
    // Increment our allocated height counter.
    allocHeight += ((Height) / numRows);
  }
}

/**
 * view calls the view method on all child cameras, followed by
 * the provided draw function. Intended to be called during the
 * display() portion of the GLUT main loop.
 *
 * view() is intended to "set up" the object, but not actually draw it.
 *
 * @param draw_func A pointer to a function that will actually draw the object.
 */
void Cameras::view( void (*draw_func)( void ) ) {
  std::list< Object* >::iterator it;
  for ( it = _list.begin(); it != _list.end(); ++it ) {
    obj2Cam( it )->view();
    (*draw_func)();
  }
}

/**
 * obj2Cam is a gross hack; the function is used as a utility to
 * convert Object pointers to Camera pointers safely.
 * FIXME: Refactor the inheritance here to make this less hacky.
 *
 * @param it A list<Object*> iterator that points to the Object.
 *
 * @return A pointer to a Camera object.
 */
Camera *Cameras::obj2Cam( std::list< Object* >::iterator &it ) {
  return dynamic_cast< Camera* >( *it );
}
