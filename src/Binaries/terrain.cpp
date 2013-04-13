/**
 @file terrain.cpp
 @authors John Huston, Nicholas StPierre, Chris Compton
 @date 2013-02-23
 @brief This is a trimmed version of our Fall 2012 project.
 @details This is a tech demo for terrain generation using an udpated
 engine derived from Ed Angel's code from his book.
 **/

// System Headers
#include <cmath>
#include <cstdio>
#include <sstream>
#include <cstdlib>
#include <time.h>
// Multi-platform support and OpenGL headers
#include "Engine.hpp"
// Utilities
#include "model.hpp"
#include "InitShader.hpp"
#include "glut_callbacks.h"
#include "ObjLoader.hpp"

// Type Aliases
using Angel::vec3;
using Angel::vec4;

// Wii Connectivity 
#ifdef WII
#include <wiicpp.h>  // WiiC++ API
#include "WiiUtil.h" // Wii Controller Handler Util
CWii Wii;
bool usingWii = false;
#endif

/**
 * Here's a gross blob of texture constants used for terrain generation!
 */
const char* terrainTex[] = { "../Textures/GoodTextures_0013423.jpg", // Dirt/Mud
    "../Textures/GoodTextures_0013779.jpg",  // Sand
    "../Textures/GrassGreenTexture0002.jpg", // Grass (who'da thunk?)
    "../Textures/GoodTextures_0013418.jpg",  // Rock
    "../Textures/GoodTextures_0013291.jpg"   // Snow
    };

/**
 randomize_terrain is called to regenerate the terrain in this application.
 It assumes the terrain is named "terrain" and is located as a direct
 child of the main scene graph.

 @return void.
 **/
void randomize_terrain() {
  
  // Final project demo used the equivalent of terrain_size = 10.
  const size_t terrain_size = 8;
  float H = fmod( (float) random(), 20 * terrain_size ) + 5 * terrain_size;
  
  srand( time( NULL ) );
  
  Object *Terrain = (*Engine::instance()->rootScene())["terrain"];
  double magnitude = landGen( Terrain, terrain_size, H );
  Terrain->buffer();
  GLint handle = glGetUniformLocation( Terrain->shader(), "terrainMag" );
  if ( handle != -1 ) glUniform1f( handle, magnitude );
  
}

/**
 init will initialize this particular flythrough,
 by creating and instantiating a shader, a camera,
 and a number of initial objects.

 @return void.
 **/
void init() {
  
  Scene *theScene = Engine::instance()->rootScene();
  Screen *myScreen = Engine::instance()->mainScreen();
  
  // Load the shaders.
  GLuint gShader = Angel::InitShader( "shaders/vterrain.glsl",
                                      "shaders/fterrain.glsl" );
  // Initialize engine setting: Enable "fixed yaw"... disable free-Y _rotation.
  Engine::instance()->opt( "fixed_yaw", true );
  
  // Give the shader handle to the Scene Graph and the Camera List.
  theScene->shader( gShader );
  myScreen->_camList.shader( gShader );
  
  // Cameras must be added after setting a shader.
  myScreen->_camList.addCamera( "Camera1" );
  myScreen->_camList.next();
  
  /*
   NOTE:
   ==========================
   Please do not give the Terrain children.
   The terrain generation animation applies directly to the terrain,
   which of course propegates the animation to all child objects.

   You'll pancake any child objects.
   */

  // Let's create some objects.
  Object *terrain = theScene->addObject( "terrain" );
  glUseProgram( gShader );// Temporary hack until I refine the texturing management subsystem.
  terrain->terrainTexture( terrainTex );
  terrain->drawMode( GL_TRIANGLE_STRIP );
  randomize_terrain();// This call depends upon "terrain" existing within theScene.
  
  Object *pyramid = theScene->addObject( "pyramid" );
  
  /*
   Object *box   = theScene.addObject( "box" ) ;
   loadModelFromFile( box, "../models/box-a.obj");
   box->_trans._offset.SetY(100.0);
   box->_trans._offset.SetX(100.0);
   box->_buffer();
   */

  sierpinskiPyramid( pyramid, vec4( 0, 1, 0, 1 ), vec4( -1, -0.999, 1, 1 ),
                     vec4( 1, -0.999, 1, 1 ), vec4( 0, -0.999, -1, 1 ), 4 );
  pyramid->buffer();
  
  Object *cube_base = theScene->addObject( "basecube" );
  colorCube( cube_base, 1.0 );
  cube_base->buffer();
  
  Object *moon_cube = pyramid->addObject( "moon" );
  colorCube( moon_cube, 0.5 );
  moon_cube->buffer();
  
  // These models came from VALVE,
  // From their game "Team Fortress 2."
  // The model processing was done in Blender.
  Object *heavy = theScene->addObject( "heavy" );
  ObjLoader::loadModelFromFile( heavy, "../models/heavyT.obj" );
  heavy->buffer();
  heavy->_trans._scale.set( 0.10 );
  heavy->_trans._offset.set( 0, 2, 0 );
  
  // Valve's TF2 Medic
  Object *medic = heavy->addObject( "medic" );
  ObjLoader::loadModelFromFile( medic, "../models/medicT.obj" );
  medic->_trans._offset.set( 0, 20, 0 );
  medic->buffer();
  
  // Valve's TF2 Spy
  Object *spy = medic->addObject( "spy" );
  ObjLoader::loadModelFromFile( spy, "../models/spyT.obj" );
  spy->_trans._offset.set( 0, 20, 0 );
  spy->buffer();
  
  Object *ball = theScene->addObject( "ball" );
  sphere( ball );
  ball->_trans._scale.set( 1000 );
  ball->buffer();
  
  /*
   Object *sun = theScene.addObject ("sun");
   //loadModelFromFile( sun, "../models/heavyT.obj" );
   sphere(sun);
   sun->_trans._offset.SetX(500.0);
   sun->_trans._scale.set(16.0) ;
   sun->_buffer();
   */

  /*
   Object *actualMoon = theScene.addObject ("actualMoon");
   //loadModelFromFile( actualMoon, "../models/spyT.obj" );
   sphere(actualMoon);
   actualMoon->_trans._offset.SetX(-500.0);
   actualMoon->_trans._scale.set(12.0);
   actualMoon->_buffer();
   */

  // The water gets generated last -- In order for our fake transparency to work.
  Object *agua = theScene->addObject( "agua" );
  makeAgua( terrain, agua );
  agua->buffer();
  agua->drawMode( GL_TRIANGLES );
  
  glEnable( GL_DEPTH_TEST );
  glClearColor( 0.3, 0.5, 0.9, 1.0 );
  
  //Attach a model to the Camera.
  Object *cam = myScreen->_camList.active();
  ObjLoader::loadModelFromFile( cam, "../models/rainbow_dashT.obj" );
  // http://kp-shadowsquirrel.deviantart.com/		
  //   art/Pony-Model-Download-Center-215266264
  cam->buffer();
  cam->drawMode( GL_TRIANGLES );
  cam->_trans._scale.set( 0.05 );
  cam->_trans._preRotation.rotateY( 180 );
  cam->propagate();

  /*
  Object *rcam = cam->addObject( "right-cam" );
  ObjLoader::loadModelFromFile( rcam, "../models/rainbow_dashT.obj" );
  rcam->buffer();
  rcam->drawMode( GL_TRIANGLES );
  rcam->_trans._offset.set( 10, 0, 0 );
  cam->propagate();
  rcam->propagate();
  */
  
  // Add the propagate method to the Scene Graph directly, instead of this:
  // Note: Terrain doesn't/shouldn't have children ...
  terrain->propagate();
  
}

/**
 cleanup is a routine to call at exit time that will free up the
 resources the application is using.
 While not critical, it does aid in using debuggers to not have
 any memory leaks at exit time.
 
 @return void.
 **/
void cleanup( void ) {
  
  //Engine::instance()->rootScene()->DestroyObject();
  
}

/**
 * This global switch controls our terrain generation.
 */
bool switchingTerrain = false;

/**
 TerrainGenerationAnimation is an animation callback that will:
 (A) If triggered, begin to shrink the object into a flat plane,
 (B) Order a re-generation of the terrain data
 (C) will grow the object back into its new, final shape.

 @param obj A reference to an object's transformation state.

 @return void.
 **/
void TerrainGenerationAnimation( TransCache &obj ) {
  
  typedef enum {
    SHRINKING, TRANSITIONING, GROWING, DONE
  } AnimationState;
  static AnimationState terrainState = DONE;
  static float CurrentScale = 0.0;
  
  if ( switchingTerrain ) {
    terrainState = SHRINKING;
    switchingTerrain = false;
    CurrentScale = 0.0;
  }
  
  switch ( terrainState ) {
  
  case SHRINKING:
    obj._scale.set( 1.0,
                    ((1.0 + cos( CurrentScale * DEGREES_TO_RADIANS )) / 2.0),
                    1.0 );
    
    CurrentScale += 1.0 * tick.scale();
    
    if ( CurrentScale >= 180.0 ) {
      terrainState = TRANSITIONING;
    }
    break;
    
  case TRANSITIONING:
    // Multithread this for graphics 2 project
    randomize_terrain();
    terrainState = GROWING;
    break;
    
  case GROWING:
    obj._scale.set( 1.0,
                    ((1.0 + cos( CurrentScale * DEGREES_TO_RADIANS )) / 2.0),
                    1.0 );
    
    CurrentScale += 1.0 * tick.scale();
    
    if ( CurrentScale >= 360.0 ) terrainState = DONE;
    break;
    
  case DONE:
    return;
    
  default:
    return;
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
  
  static Angel::vec3 OldTheta; /* Defaults to 0,0,0 */
  // Rotation Order: Y-X-Z looks the best, I think.
  //WiiCamera.yaw( NewTheta.y - OldTheta.y );
  //WiiCamera.pitch( OldTheta.x - NewTheta.x );
  //WiiCamera.roll( NewTheta.z - OldTheta.z );
  
  float yaw = -MovementRates.y / 20;
  float pitch = -MovementRates.x / 20;
  float roll = MovementRates.z / 20;
  
  if ( abs( yaw ) >= 0.1 )
    WiiCamera.yaw( -MovementRates.y / 20,
                   Engine::instance()->opt( "fixed_yaw" ) );
  if ( abs( pitch ) >= 0.1 ) WiiCamera.pitch( -MovementRates.x / 20 );
  if ( abs( roll ) >= 0.1 ) WiiCamera.roll( MovementRates.z / 20 );
  
  OldTheta = NewTheta;
  
}

/**
 * A simple animation callback.
 * Rotates the object about its Y axis.
 * @param obj The object to rotate.
 */
void simpleRotateY( TransCache &obj ) {
  obj._rotation.rotateY( tick.scale() * 1.5 );
}

/**
 * A simple animation callback.
 * Rotates the object about its Y axis,
 * as it orbits the object around a point about the Y axis.
 * @param obj The object to animate.
 */
void simpleRotateAnim( TransCache &obj ) {
  obj._rotation.rotateY( tick.scale() * 1.5 );
  obj._offset.set( 1.5, 0, 0 );
  obj._orbit.rotateY( tick.scale() * -1.0 );
}

/**
 * An animation callback that tests a variety of transformations
 * in the scene graph.
 * @param obj The object to animate.
 */
void animationTest( TransCache &obj ) {
  double timeScale = tick.scale();
  double theta = timeScale * 0.1;
  if ( 0 ) fprintf( stderr, "Timescale: %f\n", timeScale );
  
  //Object increasingly grows. 
  /* Note that, Scaling Adjustment, unlike Rotation and Translation,
   is a multiplicative adjustment, which means that
   we can't just multiply by our time scaling factor,
   we have to take pow( scaleFactor, timeScale ) instead.
   This is, of course, incredibly inefficient. */
  // obj._scale.Adjust( pow( 1.001, timeScale ) );
  //Object rotates in-place.
  obj._rotation.rotateX( theta );
  obj._rotation.rotateY( theta );
  obj._rotation.rotateZ( theta );
  
  obj._offset.set( 5, 0, 0 );
  
  //Object increasingly moves away from origin, x += 0.01
  //obj._offset.delta( timeScale * 0.01, 0, 0 );
  
  //Object orbits about the origin
  obj._orbit.rotateX( timeScale * 0.2 );
  obj._orbit.rotateY( timeScale * 0.2 );
  obj._orbit.rotateZ( timeScale * 0.2 );
  
  // Object moves its focal _orbit-point, x = 5.
  //obj.displacement.set( 5, 0, 0 ); 
}

/** Scale used for terrain generation animation. **/
float heightScale = 0.0;
/** Timer used for terrain generation animation. **/
float ticker = 0.0;

/**
 * Idle function that is called from the GLUT mainloop.
 * Applies animations, camera motion, and Wii input.
 */
void terrain_idle( void ) {
  
  Scene &theScene = (*Engine::instance()->rootScene());

  Object &Terrain = *(theScene["terrain"]);
  Object &Pyramid = *(theScene["pyramid"]);
  Pyramid.animation( animationTest );
  Pyramid["moon"]->animation( simpleRotateAnim );
  
  Object &Heavy = *(theScene["heavy"]);
  Object &Medic = *(Heavy["medic"]);
  Object &Spy = *(Medic["spy"]);
  Heavy.animation( simpleRotateAnim );
  Medic.animation( simpleRotateY );
  Spy.animation( simpleRotateY );
  
  if ( Engine::instance()->opt( "terrain_regen" ) ) {
    fprintf( stderr, "terrain_regen on, turning on switchingTerrain bool\n" );
    switchingTerrain = true;
    Engine::instance()->opt( "terrain_regen", false );
  }
  
  Terrain.animation( TerrainGenerationAnimation );
  
#ifdef WII
  if (usingWii) {
    static const unsigned NumPolls = 20;
    Camera *camptr = dynamic_cast< Camera* >( myScreen._camList["AutoCamera2"] );
    Angel::vec3 theta_diff;
    Angel::vec3 accel_mag;

    // Take many samples for two reasons:
    // (1) Without this, we can't poll often enough and Wii Input "lags".
    // (2) Average/Sample to "smooth" the data.
    for (size_t i = 0; i < NumPolls; ++i) {
      pollWii( Wii );
      if (PollResults.Reset_Camera && camptr != NULL) camptr->resetRotation();
      theta_diff += PollResults.wr_thetas;
      accel_mag += PollResults.bb_magnitudes;
    }

    if (camptr) {
      camptr->Accel( (accel_mag / NumPolls) * 2.0 );
      wiilook( *camptr, theta_diff / NumPolls, PollResults.wr_rates );
    }

  }
#endif

}

//--------------------------------------------------------------------

/**
 * menufunc is registered with GLUT to handle callbacks from
 * the simple menu that it generates.
 * @param value Integer representing the option the user chose from the menu.
 */
void menufunc( int value ) {
  
  Engine *EN = Engine::instance();
  Scene *theScene = EN->rootScene();
  
  switch ( value ) {
  case 0: // Generate Terrain
    landGen( (*theScene)["terrain"], 12, 40.0 );
    (*theScene)["terrain"]->buffer();
    break;
  case 1: // Toggle fixed-yaw.
    EN->flip( "fixed_yaw" );
    break;
  }
  
}

/**
 * Terrain generation demonstration!
 * @param argc Ignored.
 * @param argv Ignored.
 * @return EXIT_SUCCESS.
 */
int main( int argc, char **argv ) {
  
#ifdef WII
  if (!(usingWii = initWii( Wii ))) {
    std::cerr << "Not using Wii controls for this runthrough.\n";
  }
#endif

  
  Engine::init( &argc, argv, "Terrain Generation Flythrough" );
  init();
    if ( DEBUG ) {
    fprintf( stderr, "GL_VENDOR: %s\n", glGetString( GL_VENDOR ) );
    fprintf( stderr, "GL_RENDERER: %s\n", glGetString( GL_RENDERER ) );
    fprintf( stderr, "GL_VERSION: %s\n", glGetString( GL_VERSION ) );
    fprintf( stderr, "GL_SHADING_LANGUAGE_VERSION: %s\n",
             glGetString( GL_SHADING_LANGUAGE_VERSION ) );
    fprintf( stderr, "GL_EXTENSIONS: %s\n", glGetString( GL_EXTENSIONS ) );
  }
  
  int menu = glutCreateMenu( menufunc );
  glutSetMenu( menu );
  glutAddMenuEntry( "Randomize Terrain", 0 );
  glutAddMenuEntry( "Toggle Free Rotation", 1 );
  glutAttachMenu( GLUT_RIGHT_BUTTON );
  

  Engine::instance()->registerIdle( terrain_idle );

  /* PULL THE TRIGGER */
  glutMainLoop();
  return EXIT_SUCCESS;
  
}
