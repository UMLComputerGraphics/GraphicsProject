#include <stdexcept>
#include <string>
#include <map>

#include "Engine.hpp"
#include "Cameras.hpp"
#include "Scene.hpp"
#include "Screen.hpp"

Engine *Engine::_engineSingleton = NULL;

Engine *Engine::Instance( void ) {

  if (_engineSingleton == NULL)
    _engineSingleton = new Engine();
  
  return _engineSingleton;

}

/**
   Default constructor. Cannot be called, this is a singleton class.
**/
Engine::Engine( void ) {
  // Nihil. How's the weather?
}

/**
   Default, non-virtual destructor.
**/
Engine::~Engine( void ) {
  // Nihil.
}

/**
   Assignment operator. Cannot be used. This is a singleton class.
   @param assign Not used.
   @return Will always throw an exception.
**/
Engine &Engine::operator=( Engine &copy ) {
  throw std::logic_error( "Cannot assign a Engine to another; You can seriously only have one!" );
}

/** 
    Copy constructor. Cannot be called.
    @param copy Not used.
    @return Will always throw an exception.
**/
Engine::Engine( const Engine &copy ) {
  throw std::logic_error( "Engine is a Singleton. You can't make more!" );
}

/**
   Retrieves a pointer to the Camlist.
   Creates it if it does not exist.

   @return A pointer to the CamList.
**/
Cameras *Engine::Cams( void ) {

  return &(_screen._camList);

}

/**
   Retrieves a pointer to the Root Scene.

   @return A pointer to the root-level scene.
**/
Scene *Engine::RootScene( void ) {

  return &_scene;

}

/**
   Retrieves a pointer to the root Screen object.

   @return A pointer to the root-level screen.
**/
Screen *Engine::MainScreen( void ) {

  return &_screen;

}


bool Engine::Opt( const std::string &Option ) {
  return _engineSettings[ Option ];
}

void Engine::Opt( const std::string &Option, bool setting ) {
  _engineSettings[ Option ] = setting;
}

bool Engine::Set( const std::string &Option ) {
  SettingsMap::iterator it;
  it = _engineSettings.find( Option );
  if (it == _engineSettings.end()) return false;
  return true;
}
  
bool Engine::Flip( const std::string &Option ) {
  return
    _engineSettings[ Option ] = (!_engineSettings[ Option ]);
}
