#ifndef __OBJECT_HPP
#define __OBJECT_HPP

#include <vector>
#include <string>
#include "platform.h"
#include "vec.hpp"
#include "mat.hpp"
#include "Scene.hpp"
#include "TransCache.hpp"


using Angel::vec4;
typedef Angel::vec4 color4;
typedef Angel::vec4 point4;


class Object : public Scene {

  enum bufferType { 
    VERTICES,
    NORMALS,
    INDICES,
    COLORS,
    TEXCOORDS,
    VERTICES_MORPH,
    NORMALS_MORPH,
    COLORS_MORPH } ;

public:

  typedef const unsigned int UniformEnum;

  typedef enum Uniforms {
    Begin,
    IsTextured = Begin,
    ObjectCTM,
    MorphPercentage,
    End
  } Uniform;

  Object( const std::string &name, GLuint gShader );
  virtual ~Object( void );
  void Draw( void );
  void Buffer( void );
  void BufferMorphOnly( void ) ;
  void Mode( GLenum new_node );
  void Texture( const char** filename );
  const std::string &Name( void ) const;

  /* OpenGL Methods */
  virtual void Link( UniformEnum which, const std::string &name );
  virtual void Send( UniformEnum which );

  /* Animation and Related */
  void Animation(void (*anim_func)( TransCache &arg ));
  void Propegate( void );

  /* Info Get */  
  vec4 GetPosition() const ;

  /* Morph Methods */
  Object* getMorphTargetPtr() const     ;
  Object* genMorphTarget(GLuint)        ;
  float getMorphPercentage()     const  ;
  void  setMorphPercentage(const float) ;
  void  destroyMorphTarget()            ;

  /* Bad. Bad! Protect these. ...Later? :( */
  std::vector<Angel::vec4> points;
  std::vector<Angel::vec3> normals;
  std::vector<unsigned int> indices;
  std::vector<Angel::vec4> colors;
  std::vector<Angel::vec2> texcoords;



  /** Transformation State **/
  TransCache trans;

  /** 
      Handles to Uniforms on the shader. 
      Private to allow derived classes
      to extend it as needed.
  **/

  std::vector< GLint > handles;



protected:
  /** name is used as an identifying handle for the object. **/
  std::string name;     

  /** Vertex Array Object handle identifying our buffers/object. **/
  GLuint vao;

  /** Handles to our five buffers **/
  GLuint buffer[8]; // #MORPH

  /** Drawing mode for this object. GL_TRIANGLES, GL_LINE_LOOP, etc. **/
  GLenum draw_mode;

  bool isTextured;


  /** Morphing/Tweening Things **/


  /*   0.0 means 100% the actual object,
     100.0 means 100% the target object  */
  float   morphPercentage ;
  Object *morphTarget     ;


};

#endif
