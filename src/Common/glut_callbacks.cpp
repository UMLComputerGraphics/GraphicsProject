// OpenGL and System-specific macros
#include "globals.h"

// Engine manipulation.
#include "Camera.hpp"
#include "Scene.hpp"
#include "Screen.hpp"
#include "Settings.hpp"
#include "Grinstein.hpp"
#include <sstream>

/**
   keylift is registered as a GLUT callback for when a user releases a depressed key.
   
   @param key The key that was lifted.
   @param x The x coordinate of the mouse at the time the key was released.
   @param y The y coordinate of the mouse at the time the key was released.

   @return void.
**/
void keylift( unsigned char key, int x, int y ) {

  Cameras *camList = Grinstein::GetCameras();

  if (camList->NumCameras() < 1) return;
  Camera &cam = *(camList->Active());

  switch( key ) {
  case 'w':
    cam.Stop( Camera::Forward );
    break;
  case 's':
    cam.Stop( Camera::Backward );
    break;
  case 'a':
    cam.Stop( Camera::Left );
    break;
  case 'd':
    cam.Stop( Camera::Right );
    break;
  case 'q':
    cam.Stop( Camera::Up );
    break;
  case 'e':
    cam.Stop( Camera::Down );
    break;
  }
}


/**
   keyboard is a callback registered with GLUT.
   It handles keyboard input.

   @param key The key pressed by the user.
   @param x The X coordinate of the mouse when the key was pressed.
   @param y The Y coordinate of the mouse when the key was pressed.
   
   @return void.
**/
void keyboard( unsigned char key, int x, int y ) {

  static Scene *theScene = Grinstein::GetScene();
  static Cameras *camList = Grinstein::GetCameras();

#ifdef WII
  // Hacky, for the wii reset, below.
  Camera *camptr = dynamic_cast< Camera* >( (*camList)["AutoCamera2"] );
#endif

  switch( key ) {

  case 033: // Escape Key	  
    /*
      cleanup();
      Disabled for now; not crucial. 
      Intend to fix later when I profile a bit more with valgrind.
    */
    glutLeaveMainLoop();
    break;

  case ';': // Print Info
    fprintf( stderr, "Active Object: %s\n",
	     theScene->Active()->Name().c_str() );
    break;

  case '~':
#ifdef WII
    CalibrateGyro( Wii );
    if (camptr) camptr->resetRotation();
#endif
    break;
    
    
  case '+':
    std::stringstream camName;
    camName << "AutoCamera" << camList->NumCameras() + 1;
    camList->AddCamera( camName.str() );
    break;
  }

  if (camList->NumCameras() < 1) return;

  /* A shorthand variable with local scope that refers to "The Active Camera." */
  Camera &cam = *(camList->Active());
  
  switch( key ) {
  case '-':
    camList->PopCamera();
    break;
  case ';':
    fprintf( stderr, "Camera Position: (%f,%f,%f)\n", cam.X(), cam.Y(), cam.Z() );
    break;
    
  case 'w':
    cam.Move( Camera::Forward );
    break;
  case 's':
    cam.Move( Camera::Backward );
    break;
  case 'a':
    cam.Move( Camera::Left );
    break;
  case 'd':
    cam.Move( Camera::Right );
    break;
  case 'q':
    cam.Move( Camera::Up );
    break;
  case 'e':
    cam.Move( Camera::Down );
    break;
    
    //Perspectives
  case 'z': cam.changePerspective( Camera::PERSPECTIVE ); break;
  case 'x': cam.changePerspective( Camera::ORTHO ); break;
  case 'c': cam.changePerspective( Camera::ORTHO2D ); break;
  case 'v': cam.changePerspective( Camera::FRUSTUM ); break;
  case 'b': cam.changePerspective( Camera::IDENTITY ); break;

  }
}


/**
   keyboard_ctrl is registered as a GLUT callback.
   It is responsible for catching when special keys are pressed.

   @param key The key pressed.
   @param x The X coordinate of the mouse when the key was pressed.
   @param y The Y coordinate of the mouse when the key was pressed.

   @return void.
**/
void keyboard_ctrl( int key, int x, int y ) {

  static Scene *theScene = Grinstein::GetScene();
  static Cameras *camList = Grinstein::GetCameras();
  
  switch (key) {
    //Cycle between Active Objects ...
  case GLUT_KEY_LEFT:
    theScene->Prev();
    break;
  case GLUT_KEY_RIGHT:
    theScene->Next();
    break;

    //Change the Draw Mode ...
  case GLUT_KEY_F1:
    theScene->Active()->Mode( GL_POINTS );
    break;
  case GLUT_KEY_F2:
    theScene->Active()->Mode( GL_LINE_STRIP );
    break;
  case GLUT_KEY_F3:
    theScene->Active()->Mode( GL_TRIANGLE_STRIP );
    break;
  case GLUT_KEY_F4:
    theScene->Active()->Mode( GL_TRIANGLES );
    break;
  }

  // If there are no Cameras, don't muck around with this section.
  if (camList->NumCameras() < 1) return;

  switch( key ) {
  case GLUT_KEY_PAGE_UP:
    camList->Prev();
    break;

  case GLUT_KEY_PAGE_DOWN:
    camList->Next();
    break;
  }
}

void mouse( int button, int state, int x, int y ) {

  static Cameras *camList = Grinstein::GetCameras();
  if (camList->NumCameras() < 1) return;

  if ( state == GLUT_DOWN ) {
    switch( button ) {
    case 3: camList->Active()->dFOV( 1 ); break;
    case 4: camList->Active()->dFOV( -1 ); break;
    }
  }

}


void mouseroll( int x, int y ) {

  static Screen *myScreen = Grinstein::GetScreen();
  if ((x != myScreen->MidpointX()) || (y != myScreen->MidpointY())) {
    if (myScreen->camList.NumCameras() > 0)
      myScreen->camList.Active()->roll( x - myScreen->MidpointX() );
    glutWarpPointer( myScreen->MidpointX(), myScreen->MidpointY() );
  }

}

void mouselook( int x, int y ) {

  static Screen *myScreen = Grinstein::GetScreen();
  if ((x != myScreen->MidpointX()) || (y != myScreen->MidpointY())) {
    const double dx = ((double)x - myScreen->MidpointX());
    const double dy = ((double)y - myScreen->MidpointY());

    if ((abs(dx) > 100) || (abs(dy) > 100)) return;

    if (myScreen->camList.NumCameras() > 0) {
      myScreen->camList.Active()->pitch( dy );
      myScreen->camList.Active()->yaw( dx, Grinstein::GetSettings()->Get("fixed_yaw") );
    }

    glutWarpPointer( myScreen->MidpointX(), myScreen->MidpointY() );
  }
  
}

/**
   resizeEvent is registered as a glut callback for when
   the screen is resized. It instructs the screen object
   of the new size, which informs all of the children cameras
   to recompute their aspect ratios, viewport positions, and so on.

   We also warp the pointer to the center of the screen, for
   compatibility with mouselook( void ).

   @param width The new width of the window.
   @param height The new height of the window.

   @return void.
**/
void resizeEvent( int width, int height ) {

  // Get a handle to the screen object
  Screen *scr = Grinstein::GetScreen();

  // Update the size, which propagates changes to cameras and viewports.
  scr->Size( width, height );

  // Move the pointer so that there isn't a big jump next time we move it.
  glutWarpPointer( scr->MidpointX(), scr->MidpointY() );

}
