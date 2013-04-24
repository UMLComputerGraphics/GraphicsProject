/**
 @file Engine.cpp
 @author John Huston
 @authors https://github.com/UMLComputerGraphics
 @date 2013-03-13
 @brief Implementation for the Engine class.
 **/
#include <stdexcept>
#include <string>
#include <cstring>
#include <map>

#include "Engine.hpp"
#include "Cameras.hpp"
#include "Scene.hpp"
#include "Screen.hpp"
#include "glut_callbacks.h"

/**
 * static, stateful variable that is our singleton pointer.
 */
Engine *Engine::_engineSingleton = NULL;

/**
 * instance returns, or creates and then returns,
 * a pointer to the Engine object.
 * All hail the singleton!
 *
 * @return A pointer to the Engine object.
 */
Engine *Engine::instance( void ) {
  
  if ( _engineSingleton == NULL ) _engineSingleton = new Engine();
  
  return _engineSingleton;
  
}

/**
 * Default constructor. Cannot be called, this is a singleton class.
 */
Engine::Engine( void ) {
  
  _idleFunc = NULL;
  // Reminder: 0 is never a valid program.
  _currentShader = 0;
  _renderingCamera = NULL;
  
  opt( "fixed_yaw", true );
  opt( "trap_pointer", true );
  
}

/**
 * Default, non-virtual destructor.
 */
Engine::~Engine( void ) {
  // Nihil.
}

/**
 * Copy constructor. Cannot be called.
 * @param copy Not used.
 * @return Will always throw an exception.
 * @throw Will always throw std::logic_error.
 */
Engine::Engine( const Engine &copy ) {
  throw std::logic_error( "Engine is a Singleton. You can't make more!" );
  // To quiet Eclipse warnings:
  _idleFunc = NULL;
  _currentShader = 0;
  _renderingCamera = NULL;
}

/**
 * Assignment operator. Cannot be used. This is a singleton class.
 * @param assign Not used.
 * @return Will always throw an exception.
 * @throw Will always throw std::logic_error.
 */
Engine &Engine::operator=( Engine &assign ) {
  throw std::logic_error(
      "Cannot assign a Engine to another; You can seriously only have one!" );
}

/**
 * Retrieves a pointer to the Camera List.
 * @return A pointer to the Camera List.
 */
Cameras *Engine::cams( void ) {
  
  return &(_screen._camList);
  
}

/**
 * Retrieves a pointer to the Root, top-level Scene graph.
 * @return A pointer to the Root-level Scene graph.
 */
Scene *Engine::rootScene( void ) {
  
  return &_scene;
  
}

/**
 * Retrieves a pointer to the core 'Screen' object.
 * @return A pointer to the core 'Screen' object.
 */
Screen *Engine::mainScreen( void ) {
  
  return &_screen;
  
}

/**
 * Retrieves a pointer to the Texture Management object.
 * @return A pointer to the Texture Management object.
 */
TextureManagement *Engine::texMan( void ) {
  return &_texMan;
}

/**
 * opt retrieves the current setting of an option in the Engine.
 * @param Option The name of the option to access.
 * @return A bool: the current value of the setting.
 */
bool Engine::opt( const std::string &Option ) {
  return _engineSettings[Option];
}

/**
 * opt, with a second parameter, sets an Engine option.
 * @param Option The name of the option to set.
 * @param setting The value to give the option.
 * @return void.
 */
void Engine::opt( const std::string &Option, bool setting ) {
  _engineSettings[Option] = setting;
}

/**
 * set checks to see if an option has been explicitly set
 * to either True/False.
 * @param Option The option to check the existence of
 * @return A boolean: True if the option has been set, False otherwise.
 */
bool Engine::set( const std::string &Option ) {
  SettingsMap::iterator it;
  it = _engineSettings.find( Option );
  if ( it == _engineSettings.end() ) return false;
  return true;
}

/**
 * flip changes a setting from its current value to its negated form.
 * @param Option The option to toggle.
 * @return The new, current value of the option.
 */
bool Engine::flip( const std::string &Option ) {
  return _engineSettings[Option] = (!_engineSettings[Option]);
}

/**
 * Initialize GLEW, GLUT and our Engine.
 */
void Engine::init( int *argc, char *argv[], const char *title ) {
  
  // OS X suppresses events after mouse warp.  This resets the suppression
  // interval to 0 so that events will not be suppressed. This also found
  // at http://stackoverflow.com/questions/728049/
  // glutpassivemotionfunc-and-glutwarpmousepointer
#ifdef __APPLE__
  CGSetLocalEventsSuppressionInterval( 0.0 );
#endif
  Util::InitRelativePaths( *argc, argv );
  glutInit( argc, argv );
  glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
  glutInitWindowSize( 500, 500 );
  glutCreateWindow( title );
  
  char *gtfs = getenv( "GLUT_FULLSCREEN" );
  if ( gtfs && (strncmp( gtfs, "TRUE", 4 ) == 0) ) glutFullScreen();
  
  glutSetCursor( GLUT_CURSOR_NONE );
  
  GLEW_INIT()
  ;
  
  /* Register our Callbacks */
  glutDisplayFunc( Engine::displayScreen );
  glutKeyboardFunc( engineKeyboard );
  glutKeyboardUpFunc( engineKeylift );
  glutSpecialFunc( engineSpecialKeyboard );
  glutMouseFunc( engineMouse );
  glutMotionFunc( engineMouseMotion );
  glutPassiveMotionFunc( EngineMousePassive );
  glutIdleFunc( Engine::idle );
  glutReshapeFunc( engineResize );
  
  gprint( PRINT_DEBUG, "GL_VENDOR: %s\n", glGetString( GL_VENDOR ) );
  gprint( PRINT_DEBUG, "GL_RENDERER: %s\n", glGetString( GL_RENDERER ) );
  gprint( PRINT_DEBUG, "GL_VERSION: %s\n", glGetString( GL_VERSION ) );
  gprint( PRINT_DEBUG, "GL_SHADING_LANGUAGE_VERSION: %s\n",
          glGetString( GL_SHADING_LANGUAGE_VERSION ) );
  
  glEnable( GL_DEPTH_TEST );
  glClearColor( 0.0, 0.0, 0.0, 1.0 );
  
  Engine *eng = Engine::instance();
  
  // Conjure up a default shader program to use until told otherwise.
  GLuint defaultProgram = Angel::InitShader( "./shaders/vEngine.glsl",
                                             "./shaders/fEngine.glsl" );
  eng->switchShader( defaultProgram );
  
  // Set the Camera List to use this shader,
  // And add, by default, a camera.
  eng->cams()->shader( defaultProgram );
  eng->cams()->addCamera( "defaultCamera" );
  eng->cams()->next();
  
  eng->rootScene()->shader( defaultProgram );
  
}

void Engine::registerIdle( void (idleFunc)( void ) ) {
  _idleFunc = idleFunc;
}

/**
 * What should the engine be doing every idle()?
 */
void Engine::idle( void ) {
  
  static Cameras *camList = Engine::instance()->cams();
  
  // Compute the time since last idle().
  tick.tock();
  
  // Propagate Scene Graph Changes (Maybe!)
  Engine::instance()->rootScene()->propagate();
  
  // Move all camera(s).
  camList->idleMotion();
  
  Engine::instance()->callIdle();
  
  glutPostRedisplay();
  
}

void Engine::callIdle( void ) {
  if ( _idleFunc ) (*_idleFunc)();
}

GLuint Engine::currentShader( void ) const {
  return _currentShader;
}

void Engine::switchShader( GLint program ) {
  GLint currShader;
  glGetIntegerv( GL_CURRENT_PROGRAM, &currShader );
  if ( currShader != _currentShader ) {
    gprint( PRINT_ERROR,
            "ERROR: Shader in-use (%d) was not _currentShader (%d)!\n",
            currShader, _currentShader );
  }
  if ( program == _currentShader ) return;
  
  // Update our state.
  _currentShader = program;
  // Switch to the new Shader.
  glUseProgram( program );
  gprint( PRINT_VERBOSE, "Switched from [%d] to [%d]\n", currShader, program );
  if ( _renderingCamera ) {
    // Since we're on a new shader, have the camera re-send its CTM.
    _renderingCamera->relinkUniforms();
    _renderingCamera->view();
  }
}

Camera *Engine::currentCamera( void ) const {
  return _renderingCamera;
}

void Engine::switchCamera( Camera *camera ) {
  if ( camera != _renderingCamera ) {
    _renderingCamera = camera;
    // Instruct this camera to re-send his goodies.
    _renderingCamera->view();
  }
}

/**
 * What should we be doing to display()?
 * Display/render the entire screen.
 */
void Engine::displayScreen( void ) {
  
  static Cameras *camList = Engine::instance()->cams();
  
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  
  tick.sendTime();
  
  // Tell camList to draw using our 'draw' rendering function.
  camList->view( Engine::displayViewport );
  
  glutSwapBuffers();
  
}

/**
 * Display/re-render a viewport.
 */
void Engine::displayViewport( void ) {
  
  static Scene *theScene = Engine::instance()->rootScene();
  static Cameras *camList = Engine::instance()->cams();
  
  theScene->draw();
  camList->draw();
  
}
