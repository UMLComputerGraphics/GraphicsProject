#include <string>
#include <map>
#include <list>
#include <stdexcept>

#include "Scene.hpp"
#include "Object.hpp"

typedef std::pair<std::string, Object *> mapping ;

Scene::Scene() {
  _currentObj = _list.end() ;
  _gShader = 0 ;
}

Scene::~Scene() {

  /* Traverse the list and free all Objects. */
  std::list<Object*>::reverse_iterator it ;
  for (it = _list.rbegin() ; it != _list.rend(); ++it)
    delete *it ;

}

void Scene::InsertObject(const std::string name, Object *obj) {
  _list.push_back(obj) ;
  _map.insert(mapping(name, obj)) ;
}

Object *Scene::AddObject(const std::string &objName, GLuint shader) {

  // Note that 'shader' defaults to 0.
  if ((!shader) && (!_gShader))
    throw std::invalid_argument("A call to AddObject() was made without "
        "specifying either the object-specific shader,\n"
        "\tor informing the parent Scene of a default shader to use.") ;

  Object *obj = new Object(objName, ((shader) ? shader : _gShader)) ;
  InsertObject(objName, obj) ;
  return obj ;

}

/**
 Sets the Default shader for the scene.
 In the context of inheritance by objects,
 This sets the shader to use to render the physical object.

 @param gShader The GLuint handle to the shader to use.

 @return void.
 **/

void Scene::Shader(GLuint gShader) {
  this->_gShader = gShader ;
}

/**
 Retrieves the handle for the default shader for the scene.
 In the context of inheritance by objects,
 This retrieves the shader handle to use to draw the object.

 @return A GLuint handle to the shader program.
 **/

GLuint Scene::Shader(void) {
  return _gShader ;
}

/**
 DeleteObject is the actual implementation function that will
 remove an Object from the Scene list and Scene map,
 then free the object.
 @param obj The pointer to the object to free.
 **/
void Scene::DeleteObject(Object *obj) {

  if (obj == Active()) Prev() ;

  _list.remove(obj) ;
  _map.erase(obj->Name()) ;
  delete obj ;

}

void Scene::DelObject(const std::string &objName) {
  Object *obj = _map[objName] ;
  DeleteObject(obj) ;
}

void Scene::DelObject(void) {
  DeleteObject(*(_list.begin())) ;
}

/**
 Completely remove this object and all his children.
 **/
void Scene::DestroyObject(void) {
  /*  std::list< Object* >::iterator it;
   for (it = list.begin(); it != list.end(); ++it) {
   (*it)->DestroyObject();
   DeleteObject( *it );
   }
   */
}

void Scene::PopObject(void) {
  DeleteObject(*(--_list.end())) ;
}

Object *Scene::Next(void) {

  // If the list is empty, we can't cycle.
  if (_list.size() == 0)
    throw std::logic_error("Next() called, but there are no Objects"
        " in this list.") ;

  // Move to the next one. Cycle back if needed.
  if (++_currentObj == _list.end()) _currentObj = _list.begin() ;

  return *_currentObj ;

}

Object *Scene::Prev(void) {

  if (_list.size() == 0)
    throw std::logic_error("Prev() called, but there are no objects"
        " in this list.") ;

  if (_currentObj == _list.begin())
    _currentObj = --_list.end() ;
  else --_currentObj ;

  return *_currentObj ;

}

Object *Scene::Active(void) {

  if (_list.size() == 0)
    throw std::logic_error("Active() called, but the object list is empty.") ;
  else if (_currentObj == _list.end())
    throw std::logic_error(
        "Active() called, but the active object is out-of-bounds.") ;
  else return *_currentObj ;

}

void Scene::Draw(void) {
  std::list<Object*>::iterator it ;
  for (it = _list.begin() ; it != _list.end(); ++it) {
    (*it)->Draw() ;
  }
}

Object *Scene::operator[](std::string const &objname) {

  std::map<std::string, Object*>::iterator ret ;
  ret = _map.find(objname) ;

  if (ret == _map.end()) return NULL ;
  //if (ret == map.end()) throw std::out_of_range("Requested scene object \"" + objname + "\" not in scene");
  return ret->second ;

}

Scene &Scene::operator=(const Scene &copy) {

  this->_gShader = copy._gShader ;
  this->_map.clear() ;
  this->_list.clear() ;
  this->_currentObj = _list.end() ;
  return *this ;

}

Scene::Scene(const Scene &copy) {
  (*this) = copy ;
}
