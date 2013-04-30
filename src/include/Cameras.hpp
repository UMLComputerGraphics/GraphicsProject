/**
 @file Cameras.hpp
 @author John Huston
 @authors John Huston, Nicholas StPierre, Chris Compton
 @date 2012-12-04
 @brief Header for the 'Cameras' class, a collection of Camera objects.
 **/

#ifndef __CAMERAS_HPP
#define __CAMERAS_HPP

#include <vector>
#include "Camera.hpp"
#include "Scene.hpp"
using std::vector;

/**
 @author John Huston, jhuston@cs.uml.edu
 @since 28 Nov 2012

 @brief The Cameras class represents a group of logical cameras
 for a model view. Each camera possesses its own current viewing
 angle, and an absolute position in space.

 Each Camera possesses its own CTM which can be resent to the GPU at will.

 **/

class Cameras : public Scene {
  
public:
  
  /**
   * Default constructor. Nothing special.
   */
  Cameras( void );
  /**
   * Default destructor. Nothing special here, either.
   */
  ~Cameras( void );

  /* Proxies */
  /**
   * addCamera takes a name for a camera and returns a handle to a newly created camera.
   *
   * @param name The name of the new camera to create.
   * @return A Pointer to a newly created Camera object.
   */
  Camera *addCamera( const std::string &name );

  /**
   * popCamera removes the most recently added Camera from the scene.
   *
   * @return void.
   */
  void popCamera( void );
  //void DelCamera( const std::string &name );
  //void DelCamera( void );
  //void DestroyCamera( void );
  
  /* Inherited and Overloaded */
  /**
   * Sets the active camera to the next available one in the collection.
   *
   * @return A pointer to the newly active Camera.
   */
  Camera *next( void );

  /**
   * Sets the active Camera to the previous available one in the collection.
   *
   * @return A pointer to the newly active Camera.
   */
  Camera *prev( void );

  /**
   * active returns the Camera in the collection that is considered 'active'.
   *
   * @return A pointer to the currently selected, active Camera.
   */
  Camera *active( void ) const;

  /* New Definitions */
  /**
   * numCameras fetches the number of Cameras in the collection.
   *
   * @return an unsigned integer, the number of Cameras in the collection.
   */
  size_t numCameras( void ) const;

  /**
   * idleMotion calls the idle method on all child cameras.
   * Intended to be called during the idle() loop in GLUT.
   *
   * @return void.
   */
  void idleMotion( void );

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
  void resize( int width, int height );

  /**
   * For each Camera in the collection, computes the position and size of that
   * Camera's viewport in a split-screen, single-window configuration.
   * The Camera object is updated with the new information.
   *
   * @return void.
   */
  void calculateViewports( void );

  /**
   * view calls the view method on all child cameras, followed by
   * the provided draw function. Intended to be called during the
   * display() portion of the GLUT main loop.
   *
   * view() is intended to "set up" the object, but not actually draw it.
   *
   * @param draw_func A pointer to a function that will actually draw the object.
   */
  void view( void (*draw_func)( void ) );

  /* Utility */
  /**
   * obj2Cam is a gross hack; the function is used as a utility to
   * convert Object pointers to Camera pointers safely.
   * FIXME: Refactor the inheritance here to make this less hacky.
   *
   * @param it A list<Object*> iterator that points to the Object.
   *
   * @return A pointer to a Camera object.
   */
  Camera *obj2Cam( std::list< Object* >::iterator &it );

  void toggleDivision( void );

private:
  
  /**
   * _size is a simple vec2 (x,y) that contains the size of the screen.
   */
  Angel::vec2 _size;

  /**
   * _horizontalDivision controls whether or not we apply a primarily
   * horizontal (true) or vertical (false) division strategy.
   */
  bool _horizDivision;
  
};

#endif
