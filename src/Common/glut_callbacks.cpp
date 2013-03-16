/**
 * @file glut_callbacks.cpp
 * @author John Huston
 * @authors John Huston, Nick St.Pierre, Chris Compton
 * @date 2013-03-13
 * @brief glut_callbacks provides function declarations for
 * a set of functions commonly used across multiple binaries
 * for keyboard, mouse and other GLUT callback functions.
 **/

// OpenGL and System-specific macros
#include "globals.h"

// Engine manipulation.
#include "Camera.hpp"
#include "Scene.hpp"
#include "Screen.hpp"
#include "Engine.hpp"
#include <sstream>
#include <stdexcept>
#include <exception>

/**
 * keylift is registered as a GLUT callback for when a user
 * releases a depressed key.
 *
 * @param key The key that was lifted.
 * @param x The x coordinate of the mouse at the time the key was released.
 * @param y The y coordinate of the mouse at the time the key was released.
 */
void keylift( unsigned char key, int x, int y ) {

  Cameras *camList = Engine::instance()->cams();

  if ( camList->numCameras() < 1 ) return;
  Camera &cam = *(camList->active());

  switch ( key ) {
  case 'w':
    cam.stop( Camera::DIR_FORWARD );
    break;
  case 's':
    cam.stop( Camera::DIR_BACKWARD );
    break;
  case 'a':
    cam.stop( Camera::DIR_LEFT );
    break;
  case 'd':
    cam.stop( Camera::DIR_RIGHT );
    break;
  case 'q':
    cam.stop( Camera::DIR_UP );
    break;
  case 'e':
    cam.stop( Camera::DIR_DOWN );
    break;
  }
}

/**
 * keyboard is a callback registered with GLUT.
 * It handles (surprise!) keyboard input.
 *
 * @param key The key pressed by the user.
 * @param x The x coordinate of the mouse when the key was pressed.
 * @param y The y coordinate of the mouse when the key was pressed.
 */
void keyboard( unsigned char key, int x, int y ) {

  Scene *theScene = Engine::instance()->rootScene();
  Cameras *camList = Engine::instance()->cams();

#ifdef WII
  // Hacky, for the wii reset, below.
  Camera *camptr = dynamic_cast< Camera* >( (*_camList)["AutoCamera2"] );
#endif

  switch ( key ) {

  case 033: // Escape Key	  
    /*
     cleanup();
     Disabled for now; not crucial.
     Intend to fix later when I profile a bit more with valgrind.
     */
    glutLeaveMainLoop();
    break;

  case ';': // Print Info
<<<<<<< HEAD

    // Active() will throw if there is no active object, or if the requested object doesn't exist
    try 
    {
      fprintf( stderr, "Active Object: %s\n",
	       theScene->Active()->Name().c_str() );
    }

    catch( std::logic_error& e ) 
    {
      fprintf( stderr, "There is currently no Active Object, or no more Objects in the scene\n");
    }

    break;

  case '~':
#ifdef WII
    CalibrateGyro( Wii );
    if (camptr) camptr->resetRotation();
#endif
    break;
    
  case '+':
    std::stringstream camName;
    camName << "AutoCamera" << camList->numCameras() + 1;
    camList->addCamera( camName.str() );
    break;
  }

  if ( camList->numCameras() < 1 ) return;

  /* A shorthand variable with local scope that refers to "The active Camera." */
  Camera &cam = *(camList->active());
  
  switch ( key ) {
  case '-':
    camList->popCamera();
    break;
  case ';':
    fprintf( stderr, "Camera Position: (%f,%f,%f)\n", cam.x(), cam.y(),
             cam.z() );
    break;
    
  case 'w':
    cam.move( Camera::DIR_FORWARD );
    break;
  case 's':
    cam.move( Camera::DIR_BACKWARD );
    break;
  case 'a':
    cam.move( Camera::DIR_LEFT );
    break;
  case 'd':
    cam.move( Camera::DIR_RIGHT );
    break;
  case 'q':
    cam.move( Camera::DIR_UP );
    break;
  case 'e':
    cam.move( Camera::DIR_DOWN );
    break;
    
    //Perspectives
  case 'z':
    cam.changePerspective( Camera::PERSPECTIVE );
    break;
  case 'x':
    cam.changePerspective( Camera::ORTHO );
    break;
  case 'c':
    cam.changePerspective( Camera::ORTHO2D );
    break;
  case 'v':
    cam.changePerspective( Camera::FRUSTUM );
    break;
  case 'b':
    cam.changePerspective( Camera::IDENTITY );
    break;

  }
}

/**
 * keyboard_ctrl is registered as a GLUT callback.
 * It is responsible for catching when special keys are pressed.
 *
 * @param key The key pressed.
 * @param x The x coordinate of the mouse when the key was pressed.
 * @param y The y coordinate of the mouse when the key was pressed.
 */
void keyboard_ctrl( int key, int x, int y ) {

  Scene *theScene = Engine::instance()->rootScene();
  Cameras *camList = Engine::instance()->cams();
  
  switch ( key ) {
  //Cycle between active Objects ...
  case GLUT_KEY_LEFT:
    theScene->prev();
    break;
  case GLUT_KEY_RIGHT:
    theScene->next();
    break;

    //Change the Draw Mode ...
  case GLUT_KEY_F1:
    try
    {
	 theScene->Active()->Mode( GL_POINTS );
    }
    catch( std::logic_error& e ) 
     {
       fprintf(stderr, "Error: Attempt to change active object draw mode failed\nReason: %s\n", e.what() ) ;
     }
    break;
  case GLUT_KEY_F2:
    try
    {
	 theScene->Active()->Mode( GL_LINE_STRIP );
    }
    catch( std::logic_error& e ) 
     {
       fprintf(stderr, "Error: Attempt to change active object draw mode failed\nReason: %s\n", e.what() ) ;
     }
    break;
  case GLUT_KEY_F3:
    try
    {
	 theScene->Active()->Mode( GL_TRIANGLE_STRIP );
    }
    catch( std::logic_error& e ) 
     {
       fprintf(stderr, "Error: Attempt to change active object draw mode failed\nReason: %s\n", e.what() ) ;
     }
    break;
  case GLUT_KEY_F4:
    try
    {
	 theScene->Active()->Mode( GL_TRIANGLES );
    }
    catch( std::logic_error& e ) 
     {
       fprintf(stderr, "Error: Attempt to change active object draw mode failed\nReason: %s\n", e.what() ) ;
     }
    break;
  }

  // If there are no Cameras, don't muck around with this section.
  if ( camList->numCameras() < 1 ) return;

  switch ( key ) {
  case GLUT_KEY_PAGE_UP:
    camList->prev();
    break;

  case GLUT_KEY_PAGE_DOWN:
    camList->next();
    break;
  }
}

/**
 * mouse is registered as a GLUT callback.
 * It handles input from, primarily, the scrollwheel.
 *
 * @param button The mouse button being pressed.
 * @param state the state of the aforementioned mouse button.
 * @param x the x coordinate of the mouse.
 * @param y the y coordinate of the mouse.
 */
void mouse( int button, int state, int x, int y ) {

  static Cameras *camList = Engine::instance()->cams();
  if ( camList->numCameras() < 1 ) return;

  if ( state == GLUT_DOWN ) {
    switch ( button ) {
    case 3:
      camList->active()->adjustFieldOfView( 1 );
      break;
    case 4:
      camList->active()->adjustFieldOfView( -1 );
      break;
    }
  }

}

/**
 * mouseroll is registered as a GLUT callback.
 * mouseroll is called when the mouse is moved while a button is depressed.
 * It is used here to implement barrel-rolls while left-clicking.
 *
 * @param x the x coordinate of the mouse pointer.
 * @param y the y coordinate of the mouse pointer.
 */
void mouseroll( int x, int y ) {

  static Screen *myScreen = Engine::instance()->mainScreen();

  if ( (x != myScreen->MidpointX()) || (y != myScreen->MidpointY()) ) {
    if ( myScreen->_camList.numCameras() > 0 )
      myScreen->_camList.active()->roll( x - myScreen->MidpointX() );
    glutWarpPointer( myScreen->MidpointX(), myScreen->MidpointY() );
  }

}

/**
 * mouselook is registered as a GLUT callback.
 * mouselook implements FPS-like controls where the camera moves
 * proportional to the direction of the mouse.
 *
 * @param x the x coordinate of the mouse pointer.
 * @param y the y coordinate of the mouse pointer.
 */
void mouselook( int x, int y ) {

  static Screen *myScreen = Engine::instance()->mainScreen();
  if ( (x != myScreen->MidpointX()) || (y != myScreen->MidpointY()) ) {
    const double dx = ((double) x - myScreen->MidpointX());
    const double dy = ((double) y - myScreen->MidpointY());

    if ( (abs( dx ) > 100) || (abs( dy ) > 100) ) return;

    if ( myScreen->_camList.numCameras() > 0 ) {
      myScreen->_camList.active()->pitch( dy );
      myScreen->_camList.active()->yaw(
          dx, Engine::instance()->opt( "fixed_yaw" ) );
    }

    glutWarpPointer( myScreen->MidpointX(), myScreen->MidpointY() );
  }
  
}

/**
 * resizeEvent is registered as a glut callback for when
 * the screen is resized. It instructs the screen object
 * of the new size, which informs all of the children cameras
 * to recompute their aspect ratios, viewport positions, and so on.
 *
 * We also warp the pointer to the center of the screen, for
 * compatibility with mouselook( void ).
 *
 * @param width The new width of the window.
 * @param height The new height of the window.
 *
 * @return void.
 */
void resizeEvent( int width, int height ) {

  // Get a handle to the screen object
  Screen *scr = Engine::instance()->mainScreen();

  // Update the size, which propagates changes to cameras and viewports.
  scr->Size( width, height );

  // move the pointer so that there isn't a big jump next time we move it.
  glutWarpPointer( scr->MidpointX(), scr->MidpointY() );

}
