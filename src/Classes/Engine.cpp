/**
 @file Engine.cpp
 @author John Huston
 @authors https://github.com/UMLComputerGraphics
 @date 2013-03-13
 @brief Implementation for the Engine class.
 **/
#include <stdexcept>
#include <string>
#include <map>

#include "Engine.hpp"
#include "Cameras.hpp"
#include "Scene.hpp"
#include "Screen.hpp"

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
  // Nihil. How's the weather?
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
