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
  Scene( );
  virtual ~Scene( );

  void SetShader( GLuint gShader );
  GLuint GetShader( void );

  Object *AddObject( const std::string &objName );
  void DelObject( const std::string &objName );
  void DelObject( void );
  void PopObject( void );
  void DestroyObject( void );

  Object *Next( void );
  Object *Prev( void );
  Object *Active( void );
  
  void Draw( void );

  Object *operator[]( const std::string &objname );

  /* WARNING: Copies do NOT copy Children... ! */
  Scene( const Scene &copy );
  Scene &operator=( const Scene &copy );

protected:
  std::list< Object* > list;
  std::map< std::string, Object* > map;
  std::list< Object* >::iterator currentObj;
  
  /* Handle to the shader ... */
  GLuint gShader;
  
  /* Internal use */
  void DeleteObject( Object *obj );
  void InsertObject( const std::string name, Object *obj );

};

#endif
