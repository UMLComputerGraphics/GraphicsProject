#ifndef __SCENE_HPP
#define __SCENE_HPP

#include <string>
#include <map>
#include <list>
#include "platform.h" // for GLuint
/* Forward reference so that our scene can return
 pointers to Objects, and not more scenes. */
class Object;

class Scene {

public:
  Scene();
  virtual ~Scene();

  enum ObjectType { OBJECT, PARTICLESYSTEM };

  virtual void Shader( GLuint gShader );
  GLuint Shader( void );

  Object *AddObject( const std::string &objName, GLuint Object_Shader = 0, ObjectType oType = OBJECT );
  void InsertObject( const std::string, Object *obj );
  void DelObject( const std::string &objName );
  void DelObject( void );
  void PopObject( void );
  void DestroyObject( void );

  Object *next( void );
  Object *prev( void );
  Object *active( void ) const;

  void Draw( void );

  Object *operator[]( const std::string &objname );

  /* WARNING: Copies do NOT copy Children... ! */
  Scene( const Scene &copy );
  Scene &operator=( const Scene &copy );

protected:
  std::list< Object* > _list;
  std::map< std::string, Object* > _map;
  std::list< Object* >::iterator _currentObj;

  /* Handle to the shader ... */
  GLuint _gShader;

  /* Internal use */
  void DeleteObject( Object *obj );


};

#endif
