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
#include <vector>
#include "platform.h" // for GLuint
#include "RayTracer.h"

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
   * @param shader The shader that should be used to render this object.
   * @return A pointer to the new Object.
   */
  Object *addObject( const std::string &objName, GLuint shader = 0 );

  /*
   * Switches shaders used by objects initialized with a certain shader
   *    (this doesn't move their key in the map!!!)
   * @param originalGangster The shader with which the object was initialized
   * @param newShader The shader to switch it to
   */
  void replaceShader(GLuint originalGangster, GLuint newShader);

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

  /**
   * insertObject will register a new Object in the scene graph.
   * WARNING! This Object now "Belongs to" the scene graph and
   * the scene graph will manage the object for you. Do not free it!
   *
   * @param obj The object pointer to register with the Scene graph.
   */
  void insertObject( Object *obj );

  /**
   * Select as the "active object" the next object in the list.
   * If there is no "Next" object, cycle back to the first.
   * @return A pointer to the Next object in the list.
   */
  Object *next( void );

  /**
   * Selects as the active object the previous item in the list.
   * If there is no previous item, cycles back to the last item in the list.
   * @return A pointer to the Previous object in the list.
   */
  Object *prev( void );

  /**
   * Returns a pointer to the "active" object in the scene graph.
   * @return An Object pointer to the active object at this level.
   */
  Object *active( void ) const;

  /**
   * Calls the draw method on all children.
   * @return void.
   */
  void draw( bool doDraw = true );

  /**
   * Fetch an Object pointer from the scene graph with the matching name.
   * @param objname The name of the object to fetch.
   * @return The requested Object pointer.
   */
  Object *operator[]( const std::string &objname );

  /**
   * Find an object in our tree at ANY level,
   * in contrast to operator[] above.
   * @param objname The name of the object to find.
   * @return The requested object pointer.
   */
  Object *search( const std::string &objname );

  /**
   * "Copies" a scene into a new scene: Objects and Active state
   * are left behind, though, so it's not much of a copy.
   * @param copy The scene to copy from.
   * @return A reference to the scene we copied into.
   */
  Scene &operator=( const Scene &copy );

  /**
   * Copy constructor: Uses Scene::operator= to do its dirty work.
   * @param copy The scene to copy from.
   */
  Scene( const Scene &copy );

  /**
   * Propagate all changes throughout the Scene graph.
   */
  void propagate( void );

  void printTree( unsigned level = 0 );

  virtual void sceneToRaytracer( RayTracer &rt );
  virtual void bufferToRaytracer( RayTracer &rt );

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
   * For looking up objects based on their shader, to enable flip-flopping
   */
  std::map< GLuint, std::vector<Object*> > _shader_map;

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
