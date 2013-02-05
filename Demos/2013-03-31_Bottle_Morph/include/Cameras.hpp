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
   for a model view. Each camera posesses its own current viewing
   angle, and an absolute position in space.

   Each Camera posesses its own CTM which can be resent to the GPU at will.

**/

class Cameras : public Scene {

public:

  Cameras( void );
  ~Cameras( void );

  /* Proxies */
  Camera *AddCamera( const std::string &name );
  void DelCamera( const std::string &name );
  void DelCamera( void );
  void PopCamera( void );
  void DestroyCamera( void );
 
  /* Inherited and Overloaded */
  Camera *Next( void );
  Camera *Prev( void );
  Camera *Active( void );

  /* New Definitions */
  size_t NumCameras( void ) const;
  void IdleMotion( void );
  void Resize( int width, int height );
  void CalculateViewports( void );
  void View(void (*draw_func)(void));

  /* Utility */
  Camera *Obj2Cam( std::list<Object*>::iterator &it );
  void TellMeYourSecrets( void );

private:  

  /* The size of our screen. */
  Angel::vec2 Size;

};

#endif
