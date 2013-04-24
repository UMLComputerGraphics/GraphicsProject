/**
 @file Engine.hpp
 @author John Huston
 @authors https://github.com/UMLComputerGraphics
 @date 2013-03-13
 @brief Header for the Engine class.
 **/
#ifndef __ENGINE_HPP
#define __ENGINE_HPP

/* OpenGL and Globals */
#include "platform.h"
#include "globals.h"
/* Engine Classes */
#include "Camera.hpp"
#include "Cameras.hpp"
#include "Screen.hpp"
#include "Object.hpp"
#include "Timer.hpp"
#include "Scene.hpp"
#include "Texture.hpp"
#include "TextureMan.hpp"
#include "ParticleSystem.hpp"
/* Utilities */
#include "Util.hpp"
#include "model.hpp"
#include "InitShader.hpp"
#include "ObjLoader.hpp"

/* System */
#include <string>
#include <map>

/**
 * An alias for the type used by the Settings Map.
 */
typedef std::map< std::string, bool > SettingsMap;

/**
 * @author John Huston, jhuston@cs.uml.edu
 * @date 2013-03-13
 *
 * @brief The Engine class is a singleton-style class which
 * helps keep track of instances of important objects (for Cameras, Objects, etc)
 * as well as some settings and variables that would otherwise
 * clog up global namespace.
 */
class Engine {
  
public:
  
  /**
     * instance returns, or creates and then returns,
     * a pointer to the Engine object.
     * All hail the singleton!
     *
     * @return A pointer to the Engine object.
     */
    static Engine *instance( void );
    
    // Getters for core components.
    /**
     * Retrieves a pointer to the Camera List.
     * @return A pointer to the Camera List.
     */
    Cameras *cams( void );
    
    /**
     * Retrieves a pointer to the Root, top-level Scene graph.
     * @return A pointer to the Root-level Scene graph.
     */
    Scene *rootScene( void );
    
    /**
     * Retrieves a pointer to the core 'Screen' object.
     * @return A pointer to the core 'Screen' object.
     */
    Screen *mainScreen( void );
    
    /**
     * Retrieves a pointer to the Texture Management object.
     * @return A pointer to the Texture Management object.
     */
    TextureManagement *texMan( void );
    
    // Settings Getters/Setters.
    /**
     * opt retrieves the current setting of an option in the Engine.
     * @param Option The name of the option to access.
     * @return A bool: the current value of the setting.
     */
    bool opt( const std::string &Option );
    
    /**
     * opt, with a second parameter, sets an Engine option.
     * @param Option The name of the option to set.
     * @param setting The value to give the option.
     * @return void.
     */
    void opt( const std::string &Option, bool setting );
    
    /**
     * set checks to see if an option has been explicitly set
     * to either True/False.
     * @param Option The option to check the existence of
     * @return A boolean: True if the option has been set, False otherwise.
     */
    bool set( const std::string &Option );
    
    /**
     * flip changes a setting from its current value to its negated form.
     * @param Option The option to toggle.
     * @return The new, current value of the option.
     */
    bool flip( const std::string &Option );
    
    /**
     * Let's get started!
     */
    static void init( int *argc, char *argv[], const char *title );
    
    void registerIdle( void (idleFunc)( void ) );
    void callIdle( void );

  GLuint currentShader( void ) const;
  void switchShader( GLint program );
    
  Camera *currentCamera( void ) const;
  void switchCamera( Camera *camera );

    /**
     * Default, non-virtual destructor.
     */
    ~Engine( void );
    

private:
    
  /**
   * The Engine's main idle function.
   * Remains a static function so its type matches
   * the signature expected by glut.
   */
  static void idle( void );

  /**
   * The Engine's main display function.
   * Remains a static function so its type matches
   * the signature expected by glut.
   */
  static void displayScreen( void );
  
  /**
   * The Engine's viewport display function.
   * Remains a static function so its type matches
   * the signature expected by glut.
   */
  static void displayViewport( void );

  /**
   * This private data member is a callback
   * for a user-supplied idle function.
   * It is called every time GLUT otherwise calls
   * the Engine's idle callback.
   */
  void ( *_idleFunc )();

  /**
   * static, stateful variable that is our singleton pointer.
   */
  static Engine *_engineSingleton;
  
  // Engine's main components
  /**
   * The root Scene graph for the Engine.
   */
  Scene _scene;
    
    /**
     * The core "Screen" object for the Engine.
     */
    Screen _screen;
    
    // Settings Storage
    /**
     * _engineSettings is a std::map that contains
     * a series of <std::string, bool> pairs that represent our
     * Engine options.
     */
    SettingsMap _engineSettings;
    
    /**
     * _texMan is a TextureManagement object that
     * helps to manage all of the active Texture objects.
     */
    TextureManagement _texMan;
    
    /**
     * Because Cameras possess physical state (If they want,)
     * The shader attached to a Camera should not be regarded
     * as the "Current Program" but rather its "Desired Program."
     * To fill that void, then, I present to you:
     * An Engine-wide "Current Program" attribute.
     * God Bless.
     */
    GLint _currentShader;

    /**
     * Which camera is rendering right now?
     * This variable saves a pointer to whichever
     * Camera is actively rendering at the moment.
     */
    Camera *_renderingCamera;

    // Singleton Enforcement
    /**
     * Default constructor. Cannot be called, this is a singleton class.
     */
    Engine( void );
    
    /**
     * Copy constructor. Cannot be called.
     * @param copy Not used.
     * @return Will always throw an exception.
     * @throw Will always throw std::logic_error.
     */
    Engine( const Engine &copy );
    
    /**
     * Assignment operator. Cannot be used. This is a singleton class.
     * @param assign Not used.
     * @return Will always throw an exception.
     * @throw Will always throw std::logic_error.
     */
    Engine &operator=( Engine &assign );
    
};

#endif
