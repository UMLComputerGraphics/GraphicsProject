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

protected:

  // These describe the types of buffers we want on the GPU per-object.
  enum bufferType { 
    VERTICES,
    NORMALS,
    INDICES,
    COLORS,
    TEXCOORDS,
    VERTICES_MORPH,
    NORMALS_MORPH,
    COLORS_MORPH,
    NUM_BUFFERS }; // NUM_BUFFERS should always be the last enumeration. :)

public:

  typedef const unsigned int UniformEnum;
  typedef std::map< Object::UniformEnum, std::string > UniformMap;

  typedef enum Uniforms {
    BEGIN,
    IsTextured = BEGIN,
    ObjectCTM,
    MorphPercentage,
    END
  } Uniform;

  Object( const std::string &name, GLuint gShader );
  virtual ~Object( void );
  virtual void Draw( void );
  virtual void Buffer( void );
  void BufferMorphOnly( void ) ;
  void Mode( GLenum new_node );
  void Texture( const char** filename );
  const std::string &Name( void ) const;

  /* OpenGL Methods */
  virtual void Link( UniformEnum which, const std::string &name );
  virtual void send( UniformEnum which );
  virtual GLuint Shader( void );
  virtual void Shader( GLuint newShader );

  /* Animation and Related */
  void Animation(void (*anim_func)( TransCache &arg ));
  void Propagate( void );

  /* Info Get */  
  vec4 GetPosition() const ;

  /* Morph Methods */
  Object* getMorphTargetPtr() const     ;
  Object* genMorphTarget(GLuint)        ;
  float getMorphPercentage()     const  ;
  void  setMorphPercentage(const float) ;
  void  destroyMorphTarget()            ;
  int getNumberPoints();

  // Should these be protected in any way? Hm.
  std::vector<Angel::vec4> points;
  std::vector<Angel::vec3> normals;
  std::vector<unsigned int> indices;
  std::vector<Angel::vec4> colors;
  std::vector<Angel::vec2> texcoords;

  // What about this? ...
  // Transformation State
  TransCache trans;

protected:
  /** name is used as an identifying handle for the object. **/
  std::string name;

  /** Vertex Array Object handle identifying our buffers/object. **/
  GLuint vao;

  /** Handles to our buffers (Vertices, TexUVs, etc.) **/
  GLuint buffer[NUM_BUFFERS];

  /** Drawing mode for this object. GL_TRIANGLES, GL_LINE_LOOP, etc. **/
  GLenum draw_mode;

  /** Is this object textured? **/
  bool isTextured;

  /** Morphing/Tweening Things **/
  /*   0.0 means 100% the actual object,
     100.0 means 100% the target object  */
  float   morphPercentage ;
  Object *morphTarget     ;

  // Save the names of Uniform variables in this map ...
  std::map< Object::UniformEnum, std::string > _uniformMap;

  /** 
      Handles to Uniforms on the shader. 
      Private to allow derived classes
      to extend it as needed.
  **/
  std::vector< GLint > handles;

};

#endif
