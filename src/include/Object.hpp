/**
 * @file Object.hpp
 * @author John Huston
 * @authors Nick St.Pierre, Nick VerVoort, Chris Compton
 * @date 2013-03-15
 * @brief Header for the Object class.
 */

#ifndef __OBJECT_HPP
#define __OBJECT_HPP

#include <vector>
#include <string>
#include "platform.h"
#include "vec.hpp"
#include "mat.hpp"
#include "Scene.hpp"
#include "TransCache.hpp"
#include "Texture.hpp"
#include "Material.hpp"

using Angel::vec4;

/**
 * @author John Huston
 * @date 2013-03-15
 * @brief Object Class: Renderable Object Implementation
 * @details
 * The Object class represents "one renderable object" in terms
 * of the scene graph. It is a simple unit that is rendered with
 * a single draw() call.
 *
 * It contains all of the necessary state for sending uniforms
 * to the shader, all of the buffers needed to send to the card,
 * and also contains a fully-featured scene graph within itself
 * such that children objects can be attached.
 */
class Object : public Scene {

public:
  
  /**
   * These enumerations describe the types of buffers
   * we want on the GPU per-object.
   *
   * NUM_BUFFERS is a special sentinel enumeration that must always be last.
   */
  enum BufferType {
    VERTICES, //!< VERTICES
    NORMALS, //!< NORMALS
    INDICES, //!< INDICES
    COLORS, //!< COLORS
    TEXCOORDS, //!< TEXCOORDS
    VERTICES_MORPH, //!< VERTICES_MORPH
    NORMALS_MORPH, //!< NORMALS_MORPH
    COLORS_MORPH, //!< COLORS_MORPH
    NUM_BUFFERS //!< NUM_BUFFERS This is a sentinel enumeration.
  };

  /**
   * The Object class takes advantage of child-extendible enumerations.
   * We create an alias here for sake of ease.
   */
  typedef const unsigned int UniformEnum;

  /**
   * We store mappings of Uniform Enumerations,
   * The desired function of the var, to strings, the names of the variables.
   * This is utilized if we ever switch this object's shader,
   * so we can re-associate with the correct uniform locations.
   */
  typedef std::map< Object::UniformEnum, std::string > UniformMap;

  /**
   * enum Uniforms describes the properties of the base object that
   * need to be visible to the GPU.
   *
   * BEGIN and END are special sentinel enumerations that
   * must be first and last, respectively.
   */
  typedef enum Uniforms {
    BEGIN, //!< BEGIN
    IS_TEXTURED = BEGIN, //!< IS_TEXTURED
    OBJECT_CTM, //!< OBJECT_CTM
    MORPH_PCT, //!< MORPH_PCT
    TEX_SAMPLER, //!< TEX_SAMPLER
    LIGHT_AMBIENT,
    NUM_LIGHTS,
    LIGHT_POSITIONS,
    LIGHT_SPECULAR,
    LIGHT_DIFFUSE,
    LIGHT_INTENSITY,
    END //!< END
  } Uniform;

  /**
   * Constructor. Requires at minimum a _name and a shader handle.
   * @param name The _name of this object.
   * @param gShader The shader to use to render this object.
   */
  Object( const std::string &name, GLuint gShader );

  /**
   * Default destructor.
   */
  virtual ~Object( void );

  /**
   * Draw Preparation: Make preparations to draw,
   * but do not actually draw yet.
   */
  void drawPrep( void );

  /**
   * draw method: Render this object to the screen _buffer.
   */
  virtual void draw( bool doDraw = true );

  /**
   * buffer all of our data: Vertices, TexUVs, Normals,
   * Indices, Colors and Morph Buffers.
   */
  virtual void buffer( GLenum usage = GL_STATIC_DRAW );

  /**
   * buffer only the Morph-related buffers.
   */
  void bufferMorphOnly( GLenum usage = GL_STATIC_DRAW );

  /**
   * Select a new OpenGL draw mode for this Object.
   * Can be GL_LINES, GL_LINE_LOOP, GL_TRIANGLES, etc.
   * @see http://www.opengl.org/wiki/Primitive
   * @param new_mode The primitive rendering mode to use.
   */
  void drawMode( GLenum new_mode );

  /**
   * Binds a texture to this Object.
   * @param filename The filename of the texture to load.
   */
  void texture( const char* filename );

  /**
   * Retrieve the _name of this Object.
   * @return The _name of this Object.
   */
  const std::string &name( void ) const;

  // OpenGL Methods ===========================================================
  
  /**
   * link a specified Uniform against the shader's variable _name.
   *
   * @param which The Uniform to link.
   * @param name The variable name on the shader.
   */
  virtual void link( UniformEnum which, const std::string &name );

  /**
   * Send a Uniform to the shader.
   * @param which The uniform to send.
   */
  virtual void send( UniformEnum which );

  /**
   Returns the Object's current shader.
   Defined because C++ will not let you overload an overrided function,
   without re-overloading it in the derived class.

   @return a GLuint handle to the shader program used by this Object.
   **/
  virtual GLuint shader( void );

  /**
   Sets the shader to be used by this object.
   Triggers a query of the shader program,
   for the locations of the Uniform locations
   that the object needs.

   @param newShader a GLuint handle to the shader program to use.

   @return None.
   **/
  virtual void shader( GLuint newShader );

  void relinkUniforms( void );

  // animation and Related ====================================================
  
  /**
   * Apply an animation callback function to this Object.
   * Works once only: Does not save the function or automatically run on idle.
   * @param anim_func The transformation/animation function to apply.
   */
  void animation( void (*anim_func)( TransCache &arg ) );

  /**
   * Scene-graph changes are not automatically applied to children.
   * For efficiency reasons, you need to call propagateOLD() manually.
   * TODO: Deprecated.
   */
  void propagateOLD( void );

  /**
   * sceneCascade sends our current inheritable transformation matrix
   * to all of our children, /if/ it is marked as needing to be updated.
   */
  void sceneCascade( void );

  // Getters ==================================================================
  
  /**
   * Obtain the vec4 representative of the Object's current position in space.
   * @return vec4 representing the Object's position in space.
   */
  vec4 position() const;

  // Morph Methods ============================================================
  
  /**
   * Retrieve a pointer to this object's morph target.
   * @return An Object pointer to the morph target.
   */
  Object* morphTarget() const;

  /**
   * Instantiate a new morphing target.
   * @param shader The shader to use for the new morphing target.
   * NOT USED for rendering the object, but Objects cannot be instantiated
   * without a shader, so here it is.
   *
   * @return A pointer to the newly created target.
   */
  Object* genMorphTarget( GLuint shader = 0 );

  /**
   * Retrieve the morph Percentage of this object.
   * @return The morph percentage, as a float.
   */
  float morphPercentage() const;

  /**
   * Set the morph percentage of this Object.
   * @param newPercentage The new morphing percentage.
   */
  void morphPercentage( const float newPercentage );


  bool morphEnabled() const;

  /**
   * Set the morph enabled of this Object.
   * @param enabled The new morphing enabled
   */
  void morphEnabled( const bool enabled );

  /**
   * Obliterate the morph target for this object.
   */
  void destroyMorphTarget();

  /**
   * Retrieve the number of _vertices this object has.
   * @return An integer representing the number of vertices the object has.
   */
  size_t numberOfPoints();

  /**
  * Adds material data to the object
  * @param newMaterial The material to set to the object
  **/
  void addMaterial(Material *newMaterial);

  /**
   * Retrieves the material object so we can get properties from it
   * @return The material of this object
   */
  Material *getMaterial();

  /**
   * Set the Texture ID / Texture Unit for this Object.
   * @param newTextureID The new Texture Unit ID/Index for this Object.
   * @return None.
   */
  void textureID( GLint newTextureID ) {
    _textureID = newTextureID;
  }
  
  // FIXME: Should these members be protected?
  /** vertex buffer. **/
  std::vector< Angel::vec4 > _vertices;
  /** Normals buffer. **/
  std::vector< Angel::vec3 > _normals;
  /** Draw Order Index buffer. If not used, engine assumes GL_DRAW_ARRAYS. **/
  std::vector< unsigned int > _indices;
  /** Colors buffer. **/

  std::vector< Angel::vec4 > _colors;
  /** Texture Coordinates buffer. **/
  std::vector< Angel::vec2 > _texUVs;

  // FIXME: Should the transformational state be protected?
  /**
   * The _trans cache encompasses the current transformational
   * state of this object.
   */
  TransCache _trans;

  /**
   * Retrieve a vec3 containing the maximum x,y,z values found in this Object.
   * @return vec3( maxX, maxY, maxZ )
   */
  Angel::vec3 getMax( void );

  /**
   * Retrieve a vec3 containing the minimum x,y,z values found in this Object.
   * @return vec3( minX, minY, minZ )
   */
  Angel::vec3 getMin( void );
  Angel::vec4 getRealMin( void );
  Angel::vec4 getRealMax( void );

  virtual void sceneToRaytracer( RayTracer &rt );
  virtual void bufferToRaytracer( RayTracer &rt );
  //void setLights(GLfloat* ambient, GLint* numlights, GLfloat* positions, GLfloat* diffuse, GLfloat* specular);

protected:

  /** _name is used as an identifying handle for the object. **/
  std::string _name;

  /** Vertex Array Object handle identifying our buffers/object. **/
  GLuint _vao;

  /** Handles to our buffers (Vertices, TexUVs, etc.) **/
  GLuint _buffer[NUM_BUFFERS];
  GLint _attribIndex[NUM_BUFFERS];

  /** Drawing mode for this object. GL_TRIANGLES, GL_LINE_LOOP, etc. **/
  GLenum _drawMode;

  /** Is this object textured? **/
  bool _isTextured;

  // Morphing/Tweening Things
  
  /**
   * The percentage of the morph. 0.0 means 100% the original, current object.
   * 100.0 means 100% the new, targeted object.
   */
  float _morphPercentage;


  bool _morphEnabled;

  /**
   * A pointer to the object we wish to morph into.
   */
  Object *_morphTarget;

  /**
   * A map between Uniform variable functions and the actual
   * uniform variable names. Used when linking against a shader.
   */
  std::map< Object::UniformEnum, std::string > _uniformMap;

  /** 
   Handles to Uniforms on the shader.
   Protected to allow derived classes
   to extend it as needed.
   **/
  std::vector< GLint > _handles;

  std::vector< GLint > _texIDs;
  std::vector< Texture * > _textures;
  GLuint _numTextures;

  /**
   * The texture unit index this Object uses.
   */
  GLint _textureID;

  /** The diffuse color of the object **/
  Angel::vec3 color;

  /*GLfloat *_lightAmbient;
  GLfloat *_lightPositions;
  GLfloat *_lightDiffuse;
  GLfloat *_lightSpecular;
  GLint *_numLights;*/

  GLint _uLightAmbient;
  GLint _uLightPositions;
  GLint _uLightDiffuse;
  GLint _uLightSpecular;
  GLint _uNumLights;
  GLint _uLightIntensity;


  /** The material properties of this object **/
  Material *_material;

private:

  // Create and activate a Vertex Attrib Array and bind to a VBO
  GLuint createAndBind( GLenum target, enum Object::BufferType typeIndex, const char *name,
			GLint size, GLenum type, GLboolean normalized, GLsizei stride,
			const GLvoid *ptr );

  //helper for checkForAttribCorruption()
  bool checkForAttribCorruptionHelper(int typeIndex, const char *name);

  /**
   Check if any vertex attribute location changed since initilization
   @return returns true if any problems are found, false otherwise
  **/
  bool checkForAttribCorruption(void);
  
  bool _isLit;

};

#endif
