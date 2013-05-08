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
#include "Light.hpp"
#include "Cameras.hpp"
#include "Scene.hpp"
#include "Screen.hpp"
#include "glut_callbacks.h"

#include <boost/bind.hpp>
#include <boost/thread/mutex.hpp>

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
bool Engine::exists( void ) {
  if ( _engineSingleton == NULL ) return false;
  return true;
}

/**
 * Default constructor. Cannot be called, this is a singleton class.
 */
Engine::Engine( void ) :
  _traceFunc(boost::bind(&Engine::noop, this, _1)),
  _displayExtension(boost::bind(&Engine::banana, this))
    {

  _idleFunc = NULL;
  // Reminder: 0 is never a valid program.
  _currentShader = 0;
  _glslVersion = 0.0;

  _renderingCamera = NULL;
  _raytraceChanged = false;
  _raytraceStatus = false;
  _isFullScreen = false;

  _lights = new vector<Light*>;
  _lightsSize = 0;

  opt("fixed_yaw", true);
  opt("trap_pointer", true);

}

/**
 * Default, non-virtual destructor.
 */
Engine::~Engine( void ) {
  // Nihil.
  _engineSingleton = NULL;
  if (_lights) delete _lights;
  LifeLock.unlock();
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
TextureManagement *Engine::texMan( void ){
  return &_texMan;
}

/**
 * Gets the global light configuration
 * @return A pointer to a vector of light pointers represeting the global light configuration
 **/
vector<Light*>* Engine::getLights( void ) {
  return _lights;
}

void Engine::safeSetIntensity( int index, float intensity ) {
  boost::mutex::scoped_lock l(LifeLock);
  _lights->at(index)->intensity(intensity);
}

/**
 * Pushes a new light onto the global light configuration vector
 * @return void
 **/
void Engine::addLight( Light *newLight ) {
  _lights->push_back( newLight );
  getNumLights();
}

GLint* Engine::getNumLights( void ) {
  _lightsSize = (GLint) ( _lights->size() );
  return &_lightsSize;
}

GLfloat* Engine::getLightPositions( void ) {
  return _lightPositions;
}
GLfloat* Engine::getLightDiffuses( void ) {
  return _lightDiffuses;
}
GLfloat* Engine::getLightSpeculars( void ) {
  return _lightSpeculars;
}
GLfloat* Engine::getLightAmbient( void ) {
  return _lightAmbient;
}
GLfloat* Engine::getLightIntensities( void ) {
  return _lightIntensities;
}

void Engine::setLights( void ) {
   vector<Light*>::iterator it;

   int j = 0;

   for(it = _lights->begin(); it != _lights->end(); ++it) {
    //Ambient is one ambience for whole scene, so only grab this from light 0

    for(int i = 0; i < 4; i++) {
      if( it == _lights->begin() ) _lightAmbient[i] = (*it)->getGLAmbient()[i];
      _lightPositions[i+(j*4)] = (*it)->getGLPosition()[i];
      _lightDiffuses[i+(j*4)] = (*it)->getGLDiffuse()[i];
      _lightSpeculars[i+(j*4)] = (*it)->getGLSpecular()[i];
    } 

    _lightIntensities[j] = (GLfloat) (*it)->intensity();

    j++;
  }

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

void Engine::wiiInit( void ) {

#ifdef WII
  bool usingWii = initWii( _wii );
  if (!usingWii) {
    gprint( PRINT_WARNING, "Not using Wii controls for this runthrough.\n" );
  }
  opt( "wii", usingWii );
#endif

}


/**
 * Initialize GLEW, GLUT and our Engine.
 */
void Engine::init( int *argc, char *argv[], const char *title ) {

#ifdef WII
  Engine::instance()->wiiInit();
#endif

  // OS X suppresses events after mouse warp.  This resets the suppression
  // interval to 0 so that events will not be suppressed. This also found
  // at http://stackoverflow.com/questions/728049/
  // glutpassivemotionfunc-and-glutwarpmousepointer
#ifdef __APPLE__
  CGSetLocalEventsSuppressionInterval( 0.0 );
#endif
  Util::InitRelativePaths(*argc, argv);
  glutInit( argc, argv );
  glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
  glutInitWindowSize( 500, 500 );
  glutCreateWindow( title );

  char *gtfs = getenv( "GLUT_FULLSCREEN" );
  if (gtfs && (strncmp(gtfs, "TRUE", 4) == 0)) {
    Engine::instance()->setFullScreen();
  }

  glutSetCursor( GLUT_CURSOR_NONE );

  GLEW_INIT();

  /* Register our Callbacks */
  instance()->unregisterDisplayFunc();
  glutDisplayFunc(Engine::display);
  glutKeyboardFunc( engineKeyboard );
  glutKeyboardUpFunc( engineKeylift );
  glutSpecialFunc( engineSpecialKeyboard );
  glutMouseFunc( engineMouse );
  glutMotionFunc( engineMouseMotion );
  glutPassiveMotionFunc( EngineMousePassive );
  glutIdleFunc( Engine::idle );
  glutReshapeFunc( engineResize );

  float glsl_vers;
  sscanf( (char *)glGetString( GL_SHADING_LANGUAGE_VERSION ), "%f", &glsl_vers );

  gprint( PRINT_DEBUG, "GL_VENDOR: %s\n", glGetString( GL_VENDOR ) );
  gprint( PRINT_DEBUG, "GL_RENDERER: %s\n", glGetString( GL_RENDERER ) );
  gprint( PRINT_DEBUG, "GL_VERSION: %s\n", glGetString( GL_VERSION ) );
  gprint( PRINT_DEBUG, "GL_SHADING_LANGUAGE_VERSION: %f\n", glsl_vers );

  glEnable( GL_DEPTH_TEST );
  glClearColor( 0.0, 0.0, 0.0, 1.0 );

  Engine *eng = Engine::instance();

  eng->glslVersion( glsl_vers );

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

  // Set it up so that once the glut_mainloop exits,
  // It returns control to the application so we can cleanup ourselves.
#ifndef __APPLE__
  glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION );
#endif

}

void Engine::run( void ) {

  if (glGetError()) {
    gprint( PRINT_ERROR, "glGetError() returning true prior to Engine::run().\n" );
  }

  glutMainLoop();

  delete Engine::instance();
}

void Engine::registerIdle( boost::function<void(void)> idleFunc ) {
  _idleFunc = idleFunc;
}

void Engine::registerTraceFunc( raytracerCallback traceFunc ) {
  _traceFunc = traceFunc;
}

void Engine::registerDisplayFunc( boost::function<void(void)> displayFunc ) {
  _displayFunc = displayFunc;
}

void Engine::unregisterDisplayFunc()
{
  _displayFunc = displayScreen;
}

void Engine::display()
{
  instance()->_displayFunc();
}


/**
 * What should the engine be doing every idle()?
 */
void Engine::idle( void ) {

  static Engine *eng = Engine::instance();

  //YOU THOUGH' THA' WA' GO'N' TO BE WA'ER, BU' I' WA'N'... ROCK AND ROLLLLLLLL -Ozzy Osbourne
  if (instance()->_raytraceChanged) {
    instance()->_traceFunc(instance()->_raytraceStatus);
    instance()->_raytraceChanged = false;
  }

  // Compute the time since last idle().
  tick.tock();

  // Propagate Scene Graph Changes (Maybe!)
  eng->rootScene()->propagate();

  // Move all camera(s).
  eng->cams()->idleMotion();

#ifdef WII
  if (eng->opt("wii")) {
    static const unsigned NumPolls = 20;
    Camera *camptr = NULL;
    try {
      camptr = dynamic_cast< Camera* >( (*eng->cams())["AutoCamera2"] );
    } catch ( ... ) {
      camptr = NULL;
    }

    Angel::vec3 theta_diff;
    Angel::vec3 accel_mag;
    // Take many samples for two reasons:
    // (1) Without this, we can't poll often enough and Wii Input "lags".
    // (2) Average/Sample to "smooth" the data.
    for (size_t i = 0; i < NumPolls; ++i) {
      pollWii( eng->_wii );
      if (pollResults.resetCamera && camptr != NULL) camptr->resetRotation();
      theta_diff += pollResults.wrThetas;
      accel_mag += pollResults.bbMagnitudes;
    }
    if (camptr) {
      camptr->accel( (accel_mag / NumPolls) * 2.0 );
      wiilook( *camptr, theta_diff / NumPolls, pollResults.wrRates );
    }
  }
#endif

  Engine::instance()->callIdle();

  glutPostRedisplay();
}

void Engine::callIdle( void ) {
  if (_idleFunc) _idleFunc();
}

GLuint Engine::currentShader( void ) const {
  return _currentShader;
}

void Engine::switchShader( GLint program ) {
  if (glGetError()) { gprint( PRINT_ERROR, "Error, glGetError() true prior to switchShader()\n" ); }
  GLint currShader;
  glGetIntegerv( GL_CURRENT_PROGRAM, &currShader );
  if (currShader != _currentShader) {
    gprint( PRINT_ERROR, "ERROR: Shader in-use (%d) was not _currentShader (%d)!\n",
            currShader, _currentShader );
  }
  if (program == _currentShader) return;

  // Switch to the new Shader.
  glUseProgram( program );
  if (glGetError()) {
    gprint( PRINT_ERROR, "Could not switch from [%d] to [%d]\n", currShader, program );
  } else {
    _currentShader = program;
    gprint( PRINT_VERBOSE, "Switched from [%d] to [%d]\n", currShader, program );
    if (_renderingCamera) {
      // Since we're on a new shader, have the camera re-send its CTM.
      _renderingCamera->relinkUniforms();
      _renderingCamera->view();
    }
  }

}

Camera *Engine::currentCamera( void ) const {
  return _renderingCamera;
}


void Engine::switchCamera( Camera *camera ){
  if (camera != _renderingCamera) {
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

  if (glGetError()) { gprint( PRINT_ERROR, "true in displayViewport\n" ); }
  instance()->_displayExtension();

  //ensure all objects are lit the same
  boost::mutex::scoped_lock stopFlashingItsAFellony(instance()->LifeLock);
  theScene->draw();
  camList->draw();

}

bool Engine::getRaytrace()
{
  return _raytraceStatus;
}

/**
 * flips scene shaders
 *
 * @param enabled -- duh
 */
void Engine::setRaytrace(bool enabled)
{
  if (_raytraceStatus != enabled)
    {
      _raytraceStatus = enabled;
      _raytraceChanged = true;
    }
}

void Engine::noop(bool enabled)
{
  printf("The front line is everywhere.\nThere be no [raytracer] here.\n"); //Zack de la Rocha
}

void Engine::banana()
{
  //orange you glad this function's named banana?
}

void Engine::phongSong(bool enabled)
{
  static GLuint uni = glGetUniformLocation(currentShader(),"letMeSeeThatPhong");
  if (uni)
  {
    _floss = enabled;
    glUniform1i(uni, (enabled ? 1 : 0));
  }
}

float Engine::glslVersion( void ) {
  return _glslVersion;
}

void Engine::glslVersion( float in ) {
  _glslVersion = in;
}

void Engine::registerDisplayExtension(boost::function<void(void)> displayFunc)
{
  _displayExtension = displayFunc;
}

bool Engine::wearingAPhong()
{
  return _floss;
}

void Engine::setFullScreen() {

  if(_isFullScreen) {
    _isFullScreen = false;
    glutReshapeWindow(500, 500);
    glutPositionWindow(0, 0);
  } else {
    _isFullScreen = true;
    glutFullScreen();
  }

}
