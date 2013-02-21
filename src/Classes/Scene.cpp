#include <string>
#include <map>
#include <list>
#include <stdexcept>

#include "Scene.hpp"
#include "Object.hpp"

typedef std::pair< std::string, Object * > mapping;

Scene::Scene() {
  currentObj = list.begin();
}

Scene::~Scene() {

  /* Traverse the list and free all Objects. */  
  std::list< Object* >::reverse_iterator it;
  for (it = list.rbegin(); it != list.rend(); ++it)
    delete *it;

}

void Scene::InsertObject( const std::string name, Object *obj ) {
  list.push_back( obj );
  map.insert( mapping( name, obj ) );
}

Object *Scene::AddObject( const std::string &objName ) {
  
  Object *obj = new Object( objName, gShader );
  InsertObject( objName, obj );
  return obj;

}

void Scene::SetShader( GLuint gShader ) {
  this->gShader = gShader;
}

GLuint Scene::GetShader( void ) {
  return gShader;
}

/**
   DeleteObject is the actual implementation function that will
   remove an Object from the Scene list and Scene map,
   then free the object.
   @param obj The pointer to the object to free.
**/
void Scene::DeleteObject( Object *obj ) {

  if (obj == Active()) Prev();

  list.remove( obj );
  map.erase( obj->Name() );
  delete obj;

}

void Scene::DelObject( const std::string &objName ) {
  Object *obj = map[ objName ];
  DeleteObject( obj );
}

void Scene::DelObject( void ) {
  DeleteObject(*(list.begin()));
}


/**
   Completely remove this object and all his children.
**/
void Scene::DestroyObject( void ) {
  /*  std::list< Object* >::iterator it;
  for (it = list.begin(); it != list.end(); ++it) {
    (*it)->DestroyObject();
    DeleteObject( *it );
  }
  */
}
  

void Scene::PopObject( void ) {
  DeleteObject(*(--list.end()));
}

Object *Scene::Next( void ) {

  ++currentObj;
  if (currentObj == list.end())
    currentObj = list.begin();

  return *currentObj;

}

Object *Scene::Prev( void ) {

  if (currentObj == list.begin()) {
    currentObj = --list.end();
  } else {
    --currentObj;
  }

  return *currentObj;

}


Object *Scene::Active( void ) {
  return *currentObj;
}

void Scene::Draw( void ) {
  std::list< Object* >::iterator it;
  for (it = list.begin(); it != list.end(); ++it) {
    (*it)->Draw();
  }
}

Object *Scene::operator[]( std::string const &objname ) {
  
  std::map< std::string, Object* >::iterator ret;
  ret = map.find( objname );
  
  if (ret == map.end()) return NULL;
  //if (ret == map.end()) throw std::out_of_range("Requested scene object \"" + objname + "\" not in scene");
  return ret->second;

}

Scene &Scene::operator=( const Scene &copy ) {

  this->gShader = copy.gShader;
  this->map.clear();
  this->list.clear();
  this->currentObj = list.end();
  return *this;

}

Scene::Scene( const Scene &copy ) {
  (*this) = copy;
}
