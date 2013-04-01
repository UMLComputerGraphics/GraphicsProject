/**
 * @file Scene.cpp
 * @date 2013-03-29
 * @author John Huston
 * @brief Scene graph implementation.
 * @details Scene graph implementation.
 */

#include <string>
#include <map>
#include <list>
#include <stdexcept>

#include "Scene.hpp"
#include "Object.hpp"

/**
 * Shorthand alias for the <string:object> pair used in the Scene graph.
 */
typedef std::pair< std::string, Object * > NameObjPair;

/**
 * Nullary constructor.
 */
Scene::Scene() {
  _currentObj = _list.end();
  _gShader = 0;
}

/**
 * Destructor! Will traverse its list and delete all objects!
 * !! CAUTION !! If you have registered your own Objects manually,
 * note that the scene will delete them for you!
 *
 * You should, of course, never register objects from the stack.
 */
Scene::~Scene() {
  
  /* Traverse the list and free all Objects. */
  std::list< Object* >::reverse_iterator it;
  for ( it = _list.rbegin(); it != _list.rend(); ++it )
    delete *it;
  
}

/**
 Sets the Default shader for the scene.
 In the context of inheritance by objects,
 This sets the shader to use to render the physical object.

 @param gShader The GLuint handle to the shader to use.

 @return void.
 **/
void Scene::shader( GLuint gShader ) {
  this->_gShader = gShader;
}

/**
 Retrieves the handle for the default shader for the scene.
 In the context of inheritance by objects,
 This retrieves the shader handle to use to draw the object.

 @return A GLuint handle to the shader program.
 **/
GLuint Scene::shader( void ) {
  return _gShader;
}

/**
 * addObject creates a new Object with the given name and, optionally,
 * a specified shader and adds it to the Scene graph.
 * If no shader is given, a default shader MUST have been specified
 * for the Scene prior to the call.
 *
 * @param objName The name of the new Object to add.
 * @param shader The shader that should be used to render this object.
 * @param rayTrace Whether or not the new object should be handled differently for raytracing.
 * @return A pointer to the new Object.
 */
Object *Scene::addObject( const std::string &objName, GLuint shader, bool rayTrace ) {
  
  // Note that 'shader' defaults to 0.
  if ( (!shader) && (!_gShader) )
    throw std::invalid_argument(
        "A call to AddObject() was made without "
        "specifying either the object-specific shader,\n"
        "\tor informing the parent Scene of a default shader to use." );
  
  Object *obj = new Object( objName, ((shader) ? shader : _gShader), rayTrace );
  
  insertObject( obj );
  
  return obj;
  
}

/**
 * delObject will remove from the Scene graph the object with the given name.
 *
 * @param objName Name of the Object to delete.
 */
void Scene::delObject( const std::string &objName ) {
  Object *obj = _map[objName];
  deleteObject( obj );
}

/**
 * delObject with no parameters will delete the first Object in the Scene.
 */
void Scene::delObject( void ) {
  deleteObject( *(_list.begin()) );
}

/**
 * popObject deletes the last Object in the Scene.
 */
void Scene::popObject( void ) {
  deleteObject( *(--_list.end()) );
}

/**
 * insertObject will register a new Object in the scene graph.
 * WARNING! This Object now "Belongs to" the scene graph and
 * the scene graph will manage the object for you. Do not free it!
 *
 * @param obj The object pointer to register with the Scene graph.
 */
void Scene::insertObject( Object *obj ) {
  _list.push_back( obj );
  _map.insert( NameObjPair( obj->name(), obj ) );
}


/**
 * Select as the "active object" the next object in the list.
 * If there is no "Next" object, cycle back to the first.
 * @return A pointer to the Next object in the list.
 */
Object *Scene::next( void ) {
  
  // If the list is empty, we can't cycle.
  if ( _list.size() == 0 )
    throw std::logic_error( "Next() called, but there are no Objects"
                            " in this list." );
  
  // Move to the next one. Cycle back if needed.
  if ( ++_currentObj == _list.end() ) _currentObj = _list.begin();
  
  return *_currentObj;
  
}

/**
 * Selects as the active object the previous item in the list.
 * If there is no previous item, cycles back to the last item in the list.
 * @return A pointer to the Previous object in the list.
 */
Object *Scene::prev( void ) {
  
  if ( _list.size() == 0 )
    throw std::logic_error( "Prev() called, but there are no objects"
                            " in this list." );
  
  if ( _currentObj == _list.begin() ) _currentObj = --_list.end();
  else --_currentObj;
  
  return *_currentObj;
  
}

/**
 * Returns a pointer to the "active" object in the scene graph.
 * @return An Object pointer to the active object at this level.
 */
Object *Scene::active( void ) const {
  
  if ( _list.size() == 0 ) throw std::logic_error(
      "Active() called, but the object list is empty." );
  else if ( _currentObj == _list.end() ) throw std::logic_error(
      "Active() called, but the active object is out-of-bounds." );
  else return *_currentObj;
  
}

/**
 * Calls the draw method on all children.
 * @return void.
 */
void Scene::draw( void ) {
  std::list< Object* >::iterator it;
  for ( it = _list.begin(); it != _list.end(); ++it ) {
    (*it)->draw();
  }
}

/**
 * Fetch an Object pointer from the scene graph with the matching name.
 * @param objname The name of the object to fetch.
 * @return The requested Object pointer.
 */
Object *Scene::operator[]( std::string const &objname ) {
  
  std::map< std::string, Object* >::iterator ret;
  ret = _map.find( objname );
  
  //if ( ret == _map.end() ) return NULL;
  if (ret == _map.end())
    throw std::out_of_range("Requested scene object \"" + objname + "\" not in scene");
  return ret->second;
  
}

/**
 * "Copies" a scene into a new scene: Objects and Active state
 * are left behind, though, so it's not much of a copy.
 * @param copy The scene to copy from.
 * @return A reference to the scene we copied into.
 */
Scene &Scene::operator=( const Scene &copy ) {
  
  this->_gShader = copy._gShader;
  this->_map.clear();
  this->_list.clear();
  this->_currentObj = _list.end();
  return *this;
  
}

/**
 * Copy constructor: Uses Scene::operator= to do its dirty work.
 * @param copy The scene to copy from.
 */
Scene::Scene( const Scene &copy ) {
  (*this) = copy;
}

/**
 deleteObject is the actual implementation function that will
 remove an Object from the Scene list and Scene map,
 then free the object.
 @param obj The pointer to the object to free.
 **/
void Scene::deleteObject( Object *obj ) {

  if ( obj == active() ) prev();

  _list.remove( obj );
  _map.erase( obj->name() );
  delete obj;

}
