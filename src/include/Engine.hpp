#ifndef __ENGINE_HPP
#define __ENGINE_HPP

#include "Cameras.hpp"
#include "Scene.hpp"
#include "Screen.hpp"

#include <string>
#include <map>

typedef std::map<std::string,bool> SettingsMap;

class Engine {

public:

  // All hail the singleton.
  static Engine *Instance( void );

  // Getters for core components.
  Cameras *Cams( void );
  Scene *RootScene( void );
  Screen *MainScreen( void );

  // Settings Getters/Setters.
  bool Opt( const std::string &Option );
  void Opt( const std::string &Option, bool setting );
  bool Set( const std::string &Option );
  bool Flip( const std::string &Option );

  // Non-virtual, non-extendible destructor.
  ~Engine( void );

private:

  // Singleton Pointer
  static Engine *_engineSingleton;

  // Engine's main components
  Scene   _scene;
  Screen  _screen;

  // Settings Storage
  SettingsMap _engineSettings;

  // Singleton Enforcement
  Engine( void );
  Engine( const Engine &copy );
  Engine &operator=( Engine &assign );

};

#endif
