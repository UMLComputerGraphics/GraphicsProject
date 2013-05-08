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
#include "glut_callbacks.h"
#ifdef WII
#include "WiiUtil.h"
#endif

//This counteracts mouse rotation slugishness when screen dimensions are considered in calculating movement
#define MAGIC_MOUSE_SCALAR (45.0)

#include <stdexcept>
#include <exception>

bool warpPointer = true;

bool trapCursor(bool state)
{
  warpPointer = state;
  glutSetCursor( state ? GLUT_CURSOR_NONE : GLUT_CURSOR_LEFT_ARROW );
  gprint(PRINT_INFO, "Pointer = %s\n", state ? "trapped" : "FREE!");
  if (state)
    glutWarpPointer(Engine::instance()->mainScreen()->midpointX(), Engine::instance()->mainScreen()->midpointY());
  return state;
}

/**
 * keylift is registered as a GLUT callback for when a user
 * releases a depressed key.
 *
 * @param key The key that was lifted.
 * @param x The x coordinate of the mouse at the time the key was released.
 * @param y The y coordinate of the mouse at the time the key was released.
 */
void engineKeylift( unsigned char key, int x, int y ) {
  
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
void engineKeyboard( unsigned char key, int x, int y ) {
  
  Scene *theScene = Engine::instance()->rootScene();
  Cameras *camList = Engine::instance()->cams();
  
#ifdef WII
  // Hacky, for the wii reset, below.
  Camera *wiiCam = NULL;
  try {
    Camera *wiiCam = dynamic_cast< Camera* >((*camList)["AutoCamera2"] );
  } catch ( ... ) { }
#endif
  
  switch ( key ) {
  
  case 033: // Escape Key
    glutLeaveMainLoop();
    break;
    
  case ';': // Print Info
    // Active() will throw if there is no active object, or if the requested object doesn't exist
    try {
      gprint( PRINT_INFO, "Active Object: %s\n", theScene->active()->name().c_str() );
    } catch( std::logic_error& e ) {
      gprint( PRINT_WARNING, "There is currently no active object, or no objects in the scene. "
	      " We tried to print the name, but got: %s\n", e.what() );
    }
    break;
    
  case '~':
#ifdef WII
    calibrateGyro( Engine::instance()->_wii );
    if (wiiCam) wiiCam->resetRotation();
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
    if (camList->numCameras() > 1)
      camList->popCamera();
    else
      gprint( PRINT_WARNING, "You can't delete the last camera." );
    break;
  case ';':
    gprint( PRINT_INFO, "Camera Position: (%f,%f,%f)\n", cam.x(), cam.y(), cam.z() );
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

  case 'r':
	//testing stopAll function
    cam.pos(2.0, 5.0, 9.0);
    break;

  case 't':
    gprint( PRINT_DEBUG, "Turning on Terrain Regeneration\n" );
    Engine::instance()->opt( "terrain_regen", true );
    break;
    
  case 'p':
    trapCursor(!warpPointer);
	break;

  case 'f':
    Engine::instance()->setRaytrace(!Engine::instance()->getRaytrace());
    break;
  case 'l':
    Engine::instance()->phongSong(!Engine::instance()->wearingAPhong());
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
void engineSpecialKeyboard( int key, int x, int y ) {
  
  Scene *theScene = Engine::instance()->rootScene();
  Cameras *camList = Engine::instance()->cams();
  
  GLint mode = -1;

  switch ( key ) {
  //Cycle between active Objects ...
  case GLUT_KEY_LEFT:
    theScene->prev();
    break;
  case GLUT_KEY_RIGHT:
    theScene->next();
    break;
    
    //Change the Draw drawMode ...
  case GLUT_KEY_F1:
    mode = GL_POINTS;
    break;
  case GLUT_KEY_F2:
     mode = GL_LINE_STRIP;
     break;
  case GLUT_KEY_F3:
    mode = GL_TRIANGLE_STRIP;
    break;
  case GLUT_KEY_F4:
    mode = GL_TRIANGLES;
    break;

  //Fullscreen
  case GLUT_KEY_F7:
    Engine::instance()->setFullScreen();
    break;
  }

  if (mode != -1) {
    Object *s;
    try {
      s = theScene->active();
      s->drawMode( mode );
    } catch( std::exception &ex ) {
      gprint( PRINT_ERROR, "Attempted to switch the draw mode on an object, "
	      "but Active() threw an exception: %s\n", ex.what() );
    }
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

//these track button states
bool _leftDown, _rightDown;

/**
 * mouse is registered as a GLUT callback.
 * It handles input from, primarily, the scrollwheel.
 *
 * @param button The mouse button being pressed.
 * @param state the state of the aforementioned mouse button.
 * @param x the x coordinate of the mouse.
 * @param y the y coordinate of the mouse.
 */
void engineMouse( int button, int state, int x, int y ) {
  
  static Cameras *camList = Engine::instance()->cams();
  if ( camList->numCameras() < 1 ) return;
  
  if (state && !warpPointer)
  {
    trapCursor(true);
  }

  switch ( button ) {
  case GLUT_LEFT_BUTTON:
    _leftDown = state == GLUT_DOWN;
    break;
  case GLUT_RIGHT_BUTTON:
    _rightDown = state == GLUT_DOWN;
    break;
  case 3:
    camList->active()->adjustFieldOfView( 1 );
    break;
  case 4:
    camList->active()->adjustFieldOfView( -1 );
    break;
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
void engineMouseMotion( int x, int y ) {
  EngineMousePassive(x,y);
}

/**
 * mouselook is registered as a GLUT callback.
 * mouselook implements FPS-like controls where the camera moves
 * proportional to the direction of the mouse.
 *
 * @param x the x coordinate of the mouse pointer.
 * @param y the y coordinate of the mouse pointer.
 */
void EngineMousePassive( int x, int y ) {
  Screen *myScreen = Engine::instance()->mainScreen();

  int centerX = myScreen->midpointX();
  int centerY = myScreen->midpointY();

  const double dx = ((double) x - centerX) * MAGIC_MOUSE_SCALAR / ((double)myScreen->width());
  const double dy = ((double) centerY - y) * MAGIC_MOUSE_SCALAR / ((double)myScreen->height());

  if ((dx == 0 && dy == 0) || !warpPointer) return;
  if ( myScreen->_camList.numCameras() > 0 ) {
    if (_leftDown || _rightDown)
    {
      myScreen->_camList.active()->roll( dx );
    }
    else
    {
      myScreen->_camList.active()->pitch( dy );
      myScreen->_camList.active()->yaw(
          dx, Engine::instance()->opt( "fixed_yaw" ) );
    }
  }
  
  glutWarpPointer( centerX, centerY );
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
void engineResize( int width, int height ) {
  
  // Get a handle to the screen object
  Screen *scr = Engine::instance()->mainScreen();
  
  // Update the size, which propagates changes to cameras and viewports.
  scr->size( width, height );

  if (warpPointer) {
	// move the pointer so that there isn't a big jump next time we move it.
    glutWarpPointer( scr->midpointX(), scr->midpointY() );
  }
  
}


/**
 wiilook is an analog of mouselook, for wii remote controls.
 It takes a reference to a Camera, and two vec3s,
 and uses the information to adjust the Camera's _rotation.

 @param WiiCamera The camera to adjust the _rotation of.
 @param NewTheta  The X,Y,Z angles of the Wii Remote.
 @param MovementRates The X, Y, Z angular velocities of the Wii Remote.

 @return Void.

**/
void wiilook( Camera &WiiCamera, const Angel::vec3 &NewTheta,
              const Angel::vec3 &MovementRates ) {

  static Angel::vec3 OldTheta;
  static Angel::vec3 accumulated;

  // Rotation Order: Y-X-Z looks the best, I think.
  //WiiCamera.yaw( NewTheta.y - OldTheta.y );
  //WiiCamera.pitch( OldTheta.x - NewTheta.x );
  //WiiCamera.roll( NewTheta.z - OldTheta.z );
  float yaw = -MovementRates.y / 20;
  float pitch = -MovementRates.x / 20;
  float roll = MovementRates.z / 20;

  accumulated += vec3( yaw, pitch, roll );

  if ( abs(accumulated.x) >= 0.1 ) {
    WiiCamera.yaw( accumulated.x, Engine::instance()->opt( "fixed_yaw" ) );
    accumulated.x = 0;
  }

  if ( abs(accumulated.y) >= 0.1 ) {
    WiiCamera.pitch( accumulated.y );
    accumulated.y = 0;
  }
    
  if ( abs(accumulated.z) >= 0.1 ) {
    WiiCamera.roll( accumulated.z );
    accumulated.z = 0;
  }
  
  OldTheta = NewTheta;

}

