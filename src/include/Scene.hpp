/**
 * @file Scene.hpp
 * @author John Huston
 * @date 2013-03-16
 * @brief Header for the Scene class.
 */

#ifndef __SCENE_HPP
#define __SCENE_HPP

#include <string>
#include <map>
#include <list>
#include "platform.h" // for GLuint

/**
 * Forward reference so we can return pointers to Objects,
 * Instead of pointers to scenes.
 * TODO: Should I just have them return Scene pointers? Hmm.
 */
class Object;

/**
 * The Scene object keeps track of a list of objects considered to be
 * "children" of the Scene. The Scene itself has no physical representation
 * or presence otherwise on the GPU, it is purely a logical CPU entity.
 *
 * @date 2013-03-16
 * @author John Huston
 */
class Scene {

public:
  /**
   * Nullary constructor.
   */
  Scene();

  /**
   * Destructor! Will traverse its list and delete all objects!
   * !! CAUTION !! If you have registered your own Objects manually,
   * note that the scene will delete them for you!
   *
   * You should, of course, never register objects from the stack.
   */
  virtual ~Scene();

  /**
   Sets the Default shader for the scene.
   In the context of inheritance by objects,
   This sets the shader to use to render the physical object.

   @param gShader The GLuint handle to the shader to use.

   @return void.
   **/
  virtual void shader( GLuint gShader );

  /**
   Retrieves the handle for the default shader for the scene.
   In the context of inheritance by objects,
   This retrieves the shader handle to use to draw the object.

   @return A GLuint handle to the shader program.
   **/
  GLuint shader( void );

  /**
   * addObject creates a new Object with the given name and, optionally,
   * a specified shader and adds it to the Scene graph.
   * If no shader is given, a default shader MUST have been specified
   * for the Scene prior to the call.
   *
   * @param objName The name of the new Object to add.
   * @param Object_Shader The shader that should be used to render this object.
   * @return A pointer to the new Object.
   */
  Object *addObject( const std::string &objName, GLuint Object_Shader = 0 );

  /**
   * delObject will remove from the Scene graph the object with the given name.
   *
   * @param objName Name of the Object to delete.
   */
  void delObject( const std::string &objName );

  /**
   * delObject with no parameters will delete the first Object in the Scene.
   */
  void delObject( void );

  /**
   * popObject deletes the last Object in the Scene.
   */
  void popObject( void );

  Object *next( void );
  Object *prev( void );
  Object *active( void ) const;

  void Draw( void );

  Object *operator[]( const std::string &objname );

  /* WARNING: Copies do NOT copy Children... ! */
  Scene( const Scene &copy );
  Scene &operator=( const Scene &copy );

  /**
   * Register a created object with the scene graph.
   * @param name The name of the object (For the associative map),
   * @param obj The Object pointer to add to the scene.
   */
  void insertObject( Object *obj );

protected:
  /**
   * For the purposes of rapid propagation of scene-graph changes,
   * Object pointers are stored in a regular flat list.
   */
  std::list< Object* > _list;

  /**
   * For the purposes of accessing named objects quickly, though,
   * objects are also re-stored in an associative map.
   * It's a little superfluous, but here it is!
   */
  std::map< std::string, Object* > _map;

  /**
   * We keep an iterator on-hand that references what the
   * scene considers to be it's active, current object.
   */
  std::list< Object* >::iterator _currentObj;

  /**
   * A handle to a shader program to be used as the default shader
   * for new children objects added to the scene.
   */
  GLuint _gShader;

  // Internal use =============================================================
  /**
   * Very seriously delete a child object and free his memory.
   * @param obj The object to delete.
   */
  void deleteObject( Object *obj );

};

#endif
