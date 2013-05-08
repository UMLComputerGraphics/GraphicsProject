/**
 * @file Object.cpp
 * @author John Huston
 * @authors Nick St.Pierre, Nick VerVoort, Chris Compton
 * @date 2013-03-15
 * @brief Implementation for the Object class.
 */
#include <string>
#include <vector>
#include <stdexcept>
#include <cstdio>

#include "Engine.hpp"
#include "Object.hpp"
#include "Util.hpp"

using Angel::vec4;
using Angel::vec3;
using Angel::mat4;


/**
 * Bind a VBO and associate it with a variable on the shader.
 * Fail if the variable is not located,
 * And cache the attribute location for later if we later need to disable it.
 **/
GLuint Object::createAndBind( GLenum target, enum Object::BufferType typeIndex, const char *name,
			      GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* ptr ) {

  GLuint &buffer = _buffer[typeIndex];
  GLint &index = _attribIndex[typeIndex];

  glBindBuffer( target, buffer );
  index = glGetAttribLocation( shader(), name );
  
  if ( index == -1 ) {
    gprint( PRINT_WARNING, "Warning: Failed to enable VBO for %s\n", name );
    return -1;
  }
  
  glEnableVertexAttribArray( index );
  glVertexAttribPointer( index, size, type, normalized, stride, ptr );

  return index;

}


/**
 * Constructor. Requires at minimum a _name and a shader handle.
 * @param name The _name of this object.
 * @param gShader The shader to use to render this object.
 */
Object::Object( const std::string &name, GLuint gShader ) {
  
  /* The constructor is going to initialize the VAO and a series of VBOs.
   The VAO is our general handle to this collection of VBOs.

   Each VBO contains some component data for how to render the vertex:
   Position, Color, Direction (Normal), texture and draw Order. */

  gprint( PRINT_INFO, "\n-- Creating Object (%s) --\n", name.c_str() );

  // Create room for our GLUniform _handles
  gprint( PRINT_DEBUG, "Creating %d handles for uniforms\n", Object::END );
  _handles.resize( Object::END );
  
  // Associate this Object with the shader.
  shader( gShader );
  
  // Set our _name from the constructor...
  _name = name;
  
  // Initialize our draw mode to GL_TRIANGLES until informed otherwise.
  _drawMode = GL_TRIANGLES;
  
  // Get Uniform _handles for the following shader variables
  link( Object::IS_TEXTURED, "fIsTextured" );
  link( Object::OBJECT_CTM, "OTM" );
  link( Object::MORPH_PCT, "morphPercentage" );
  link( Object::TEX_SAMPLER, "sampler" );
  link( Object::LIGHT_AMBIENT, "LightAmbient" );
  link( Object::LIGHT_POSITIONS, "uLightPositions" );
  link( Object::NUM_LIGHTS, "uNumberOfLights" );
  link( Object::LIGHT_DIFFUSE, "uLightDiffuse" );
  link( Object::LIGHT_SPECULAR, "uLightSpecular" );
  link( Object::LIGHT_INTENSITY, "uLightIntensity" );
  
  // Default to "Not Textured"
  _isTextured = false;
  _textureID = -1;
  _numTextures = 0;
  _isLit = true;

  // Linear Interpolation Demo: Morph Percentage
  _morphPercentage = 0.0;

  // Pointer to an Object to Morph to.
  _morphTarget = NULL;
  _material = NULL;

  // Default to no-morph -- please remember that C types need to be initialized! 
  _morphEnabled = false;
  
  /* Create our VAO, which is our handle to all 
   the rest of the following information. */
  glGenVertexArrays( 1, &_vao );
  glBindVertexArray( _vao );
  
  if (gShader == 0) {
    gprint( PRINT_WARNING, "Warning: Object %s created without a valid shader.\n", _name.c_str() );
    gprint( PRINT_WARNING, "Disabling use of renderable geometry for this object.\n" );
    glBindVertexArray( 0 );
    return;
  }

  /* Create (Eight) VBOs: One each for Positions, Colors, Normals, 
   Textures, draw Order; Three for Morphing (Position,Colors,Normals.) */

  glGenBuffers( NUM_BUFFERS, _buffer );

  // Create the Vertex _buffer and link it with the shader.
  createAndBind( GL_ARRAY_BUFFER, VERTICES, "vPosition", 4, GL_FLOAT, GL_FALSE, 0, 0 );

  // Create the MORPH Vertex _buffer and link it with the shader.
  createAndBind( GL_ARRAY_BUFFER, VERTICES_MORPH, "vPositionMorph", 4, GL_FLOAT, GL_FALSE, 0, 0 );
  
  // Create the Normal _buffer and link it with the shader.
  createAndBind( GL_ARRAY_BUFFER, NORMALS, "vNormal", 3, GL_FLOAT, GL_FALSE, 0, 0 );
  
  // Create the Normal MORPH _buffer and link it with the shader. 
  createAndBind( GL_ARRAY_BUFFER, NORMALS_MORPH, "vNormalMorph", 3, GL_FLOAT, GL_FALSE, 0, 0 );
  
  // Create the Color _buffer and link it with the shader.
  createAndBind( GL_ARRAY_BUFFER, COLORS, "vColor", 4, GL_FLOAT, GL_FALSE, 0, 0 );
  //Only need to call these once, consider removing from constructor?
  glEnable( GL_BLEND );
  glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

  // Create the Color Morph _buffer and link it with the shader.
  createAndBind( GL_ARRAY_BUFFER, COLORS_MORPH, "vColorMorph", 4, GL_FLOAT, GL_FALSE, 0, 0 );
  //glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
  
  // Create the texture Coordinate _buffer and link it with the shader.
  createAndBind( GL_ARRAY_BUFFER, TEXCOORDS, "vTex", 2, GL_FLOAT, GL_FALSE, 0, 0 );

  // Create the drawing order _buffer, but we don't need to link it
  // with any uniform, because we won't be accessing this data directly.
  // I.e, the numbers here are not important once we are in the vshader.
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, _buffer[INDICES] );

  gprint( PRINT_DEBUG, "Array/Buffer Handles: %u %u %u %u %u %u %u %u\n",
	  _buffer[VERTICES], _buffer[NORMALS], _buffer[COLORS],
	  _buffer[TEXCOORDS], _buffer[INDICES], _buffer[VERTICES_MORPH],
	  _buffer[NORMALS_MORPH], _buffer[COLORS_MORPH] );
  
  if (glGetError()) {
    gprint( PRINT_ERROR, "glGetError() true after %s construction\n", _name.c_str() );
  }

  /* Unset the VAO context. */
  glBindVertexArray( 0 );
}

/*
void Object::rebind(){

  index = glGetAttribLocation( shader(), name );


  // Create the Vertex _buffer and link it with the shader.
  createAndBind( GL_ARRAY_BUFFER, VERTICES, "vPosition", 4, GL_FLOAT, GL_FALSE, 0, 0 );

  // Create the MORPH Vertex _buffer and link it with the shader.
  createAndBind( GL_ARRAY_BUFFER, VERTICES_MORPH, "vPositionMorph", 4, GL_FLOAT, GL_FALSE, 0, 0 );
  
  // Create the Normal _buffer and link it with the shader.
  createAndBind( GL_ARRAY_BUFFER, NORMALS, "vNormal", 3, GL_FLOAT, GL_FALSE, 0, 0 );
  
  // Create the Normal MORPH _buffer and link it with the shader. 
  createAndBind( GL_ARRAY_BUFFER, NORMALS_MORPH, "vNormalMorph", 3, GL_FLOAT, GL_FALSE, 0, 0 );
  
  // Create the Color _buffer and link it with the shader.
  createAndBind( GL_ARRAY_BUFFER, COLORS, "vColor", 4, GL_FLOAT, GL_FALSE, 0, 0 );
  glEnable( GL_BLEND );
  glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

  // Create the Color Morph _buffer and link it with the shader.
  createAndBind( GL_ARRAY_BUFFER, COLORS_MORPH, "vColorMorph", 4, GL_FLOAT, GL_FALSE, 0, 0 );
  glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
  
  // Create the texture Coordinate _buffer and link it with the shader.
  createAndBind( GL_ARRAY_BUFFER, TEXCOORDS, "vTex", 2, GL_FLOAT, GL_FALSE, 0, 0 );

  // Create the drawing order _buffer, but we don't need to link it
  // with any uniform, because we won't be accessing this data directly.
  // I.e, the numbers here are not important once we are in the vshader.
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, _buffer[INDICES] );

  gprint( PRINT_DEBUG, "Array/Buffer Handles: %u %u %u %u %u %u %u %u\n",
	  _buffer[VERTICES], _buffer[NORMALS], _buffer[COLORS],
	  _buffer[TEXCOORDS], _buffer[INDICES], _buffer[VERTICES_MORPH],
	  _buffer[NORMALS_MORPH], _buffer[COLORS_MORPH] );

  // Unset the VAO context.
  glBindVertexArray( 0 );

}
*/

/**
   helper function for checkForAttribCorruption()
   @param typeindex index into the _attribIndex member array
   @param name name of the attribute to change
   @return returns true if a problem is found, false otherwise
 **/
bool
Object::checkForAttribCorruptionHelper(int typeIndex, const char *name)
{
  if ( _attribIndex[typeIndex] != glGetAttribLocation( shader(), name ) )
    {
      fprintf( stderr, "OH NOOO: %s was changed since initialization\n", name);
      return true;
    }

  else return false;

}

/**
   Check if any vertex attribute location changed since initilization
   @return returns true if any problems are found, false otherwise
 **/
bool
Object::checkForAttribCorruption(void)
{

  bool status = false ;

  glBindVertexArray( _vao );

  if ( checkForAttribCorruptionHelper( VERTICES, "vPosition" )) status = true;
  if ( checkForAttribCorruptionHelper( NORMALS,  "vNormal"   )) status = true;
  if ( checkForAttribCorruptionHelper( COLORS,   "vColor"    )) status = true;

  // Unset the VAO context.
  glBindVertexArray( 0 );

  if ( status ) gprint( PRINT_WARNING, "Warning: attribute location corruption occurred: PANIC");

  return status ;

}

/**
 * Default destructor.
 */
Object::~Object( void ) {
  
  destroyMorphTarget();
  std::vector< Texture * >::iterator it;

  for ( it = _textures.begin(); it != _textures.end(); ++it )
    delete (*it);
  
  if (_morphTarget != NULL) {
    delete _morphTarget;
    _morphTarget = NULL;
  }

  if ( _material != NULL ) delete _material;

}

/**
 * Draw Preparation: Make preparations to draw,
 * but do not actually draw yet.
 */
void Object::drawPrep( void ) {

  // Check to see if the correct shader program is engaged.
  GLuint currShader = Engine::instance()->currentShader();
  if (currShader != shader()) {
    gprint( PRINT_VERBOSE, "Object %s requesting switchShader from %d to %d\n",
            _name.c_str(), currShader, shader() );
    Engine::instance()->switchShader( shader() );
  }

  this->checkForAttribCorruption();

  glBindVertexArray( _vao );

  send( Object::IS_TEXTURED );
  send( Object::OBJECT_CTM );
  send( Object::MORPH_PCT );
  send( Object::TEX_SAMPLER );

  //light stuff
  send( Object::LIGHT_AMBIENT );
  send( Object::NUM_LIGHTS );
  send( Object::LIGHT_POSITIONS );
  send( Object::LIGHT_DIFFUSE );
  send( Object::LIGHT_SPECULAR );
  send( Object::LIGHT_INTENSITY );
}

/**
 * draw method: Render this object to the screen _buffer.
 */
void Object::draw( bool doDraw ) {

  if (!doDraw)
    Scene::draw( doDraw );

  drawPrep();
  
  /* Are we using a draw order? */
  if ( _indices.size() > 1 ) glDrawElements( _drawMode, _indices.size(),
                                             GL_UNSIGNED_INT, 0 );
  else glDrawArrays( _drawMode, 0, _vertices.size() );
  
  glBindVertexArray( 0 );
  
  // draw all of our Children.
  // (With clothes on, pervert.)
  Scene::draw( doDraw );
  
}


inline void disableIfEnabled( GLint index ) {
  if (index != -1) 
    glDisableVertexAttribArray( index );
}

/**
 * buffer all of our data: Vertices, TexUVs, Normals,
 * Indices, Colors and Morph Buffers.
 */
void Object::buffer( GLenum usage ) {
  
  glBindVertexArray( _vao );
  
  glBindBuffer( GL_ARRAY_BUFFER, _buffer[VERTICES] );
  glBufferData( GL_ARRAY_BUFFER, sizeof(Angel::vec4) * _vertices.size(),
                &(_vertices[0]), usage );
  
  glBindBuffer( GL_ARRAY_BUFFER, _buffer[NORMALS] );
  glBufferData( GL_ARRAY_BUFFER, sizeof(Angel::vec3) * _normals.size(),
                &(_normals[0]), usage );
  
  if (_texUVs.size() == 0) {
    // Disable Textures ...
    _isTextured = false;
    disableIfEnabled( _attribIndex[ TEXCOORDS ] );

    // Enable Colors.
    glBindBuffer( GL_ARRAY_BUFFER, _buffer[COLORS] );
    glBufferData( GL_ARRAY_BUFFER, sizeof(Angel::vec4) * _colors.size(),
		  &(_colors[0]), usage );
  } else {
    // Enable Textures ...
    _isTextured = true;
    glBindBuffer( GL_ARRAY_BUFFER, _buffer[TEXCOORDS] );
    glBufferData( GL_ARRAY_BUFFER, sizeof(Angel::vec2) * _texUVs.size(),
		  (_texUVs.size() ? &(_texUVs[0]) : NULL), usage );

    // Disable Colors.
    disableIfEnabled( _attribIndex[ COLORS ] );
    fprintf( stderr, "Disabled colors for %s\n", _name.c_str() );
  }
  
  if (_morphTarget == NULL) {
    disableIfEnabled( _attribIndex[ VERTICES_MORPH ] );
    disableIfEnabled( _attribIndex[ NORMALS_MORPH ] );
    disableIfEnabled( _attribIndex[ COLORS_MORPH ] );

    // Note: We disable the Morph buffers if they are not
    // being used, however, the 'glVertexAttrib*' method
    // of specifying a "default" value in this case is
    // apparently not well supported in glsl 1.2 and/or intel OpenGL.
    // In this case, the value of these attributes
    // on the shader is 'undefined.'

  } else {
    bufferMorphOnly( usage );
  }

  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, _buffer[INDICES] );
  glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * _indices.size(),
                &(_indices[0]), usage );
  
  glBindVertexArray( 0 );
  
}

/**
 * buffer only the Morph-related buffers.
 */
void Object::bufferMorphOnly( GLenum usage ) {
  
  glBindVertexArray( _vao );
  
  glBindBuffer( GL_ARRAY_BUFFER, _buffer[VERTICES_MORPH] );
  glBufferData( GL_ARRAY_BUFFER,
                sizeof(Angel::vec4) * morphTarget()->_vertices.size(),
                &(morphTarget()->_vertices[0]), usage );
  
  glBindBuffer( GL_ARRAY_BUFFER, _buffer[NORMALS_MORPH] );
  glBufferData( GL_ARRAY_BUFFER,
                sizeof(Angel::vec3) * morphTarget()->_normals.size(),
                &(morphTarget()->_normals[0]), usage );
  
  glBindBuffer( GL_ARRAY_BUFFER, _buffer[COLORS_MORPH] );
  glBufferData( GL_ARRAY_BUFFER,
                sizeof(Angel::vec4) * morphTarget()->_colors.size(),
                &(morphTarget()->_colors[0]), usage );
  
  // #MORPH
  // TODO: MORPH TEXTURES AND INDICIES
  
  glBindVertexArray( 0 );
  
}

/**
 * Select a new OpenGL draw mode for this Object.
 * Can be GL_LINES, GL_LINE_LOOP, GL_TRIANGLES, etc.
 * @see http://www.opengl.org/wiki/Primitive
 * @param new_mode The primitive rendering mode to use.
 */
void Object::drawMode( GLenum new_mode ) {
  _drawMode = new_mode;
}


/**
 * Binds a texture to this Object.
 * @param filename The filename of the texture to load.
 */
void Object::texture( const char* filename ) {

  TextureManagement *tx = Engine::instance()->texMan();
  Texture *newTex = new Texture( GL_TEXTURE_2D );
  newTex->load( filename );
  newTex->buffer();

  _textureID = tx->assign( newTex );
  _texIDs.push_back( _textureID );
  _textures.push_back( newTex );
  _numTextures = _texIDs.size();

  send( Object::TEX_SAMPLER );

}

/**
 * Retrieve the _name of this Object.
 * @return The _name of this Object.
 */
const std::string &Object::name( void ) const {
  
  return _name;
  
}

/**
 * link a specified Uniform against the shader's variable _name.
 *
 * @param which The Uniform to link.
 * @param name The variable _name on the shader.
 */
void Object::link( UniformEnum which, const std::string &name ) {
  
  // TODO: FIXME: GROSS: HACK: This might not work,
  // Depending on when it is called ...
  GLint aShader = Engine::instance()->currentShader();
  // The previous behavior was:
  // aShader = shader();

  if ( which >= _handles.size() ) {
    gprint( PRINT_WARNING, "Warning: Ignoring request to link a uniform "
	    "(#%u) beyond our handles array [%lu].\n",
	    which, _handles.size() );
    return;
  }
  
  // Save the link between the Uniform and the Variable _name.
  _uniformMap[which] = name;

  gprint( PRINT_VERBOSE, "Linking enum[%u] with %s for object %s\n",
	  which, name.c_str(), this->_name.c_str() );

  if (aShader == 0) {
    gprint( PRINT_VERBOSE, "Skipping link: [%s][%s]: No shader set.\n",
	    _name.c_str(), name.c_str() );
    return;
  }

  _handles[which] = glGetUniformLocation( aShader, name.c_str() );
  if (glGetError()) {
    gprint( PRINT_ERROR, "ERROR: [%s] failed to call glGetUniformLocation( %u, %s );\n",
	    _name.c_str(), shader(), name.c_str() );
  }

  gprint( PRINT_VERBOSE, "Linking handles[%d] to %s; got %d.\n", which,
	  name.c_str(), _handles[which] );
  
}

/**
 * Send a Uniform to the shader.
 * @param which The uniform to send.
 */
void Object::send( Object::UniformEnum which ) {

  if (glGetError()) {
    gprint( PRINT_ERROR, "ERROR: glGetError() returning true prior to exec of send().\n" );
  }

  if (shader() == 0) {
    gprint( PRINT_WARNING, "Warning: Object::send() for [%s][%u] called with no shader.\n", _name.c_str(), which );
    return;
  }

  switch ( which ) {
  
  case Object::IS_TEXTURED:
    glUniform1i( _handles[Object::IS_TEXTURED], (_isTextured) ? 1 : 0 );
    break;
    
  case Object::OBJECT_CTM:
    glUniformMatrix4fv( _handles[Object::OBJECT_CTM], 1, GL_TRUE,
                        _trans.otm() );
    break;
    
  case Object::MORPH_PCT:
    glUniform1f( _handles[Object::MORPH_PCT], this->morphPercentage() );
    break;
    
  case Object::TEX_SAMPLER:
    if (_isTextured) {
      //glUniform1i( _handles[Object::TEX_SAMPLER], _textureID );
      glUniform1iv( _handles[Object::TEX_SAMPLER], _numTextures, &_texIDs[0]);
      //fprintf( stderr, "Sending sampler uniform: %d textures\n", _numTextures );
    }
    break;
  default:
    {
      GLint *numL = Engine::instance()->getNumLights();
      GLfloat *amb = Engine::instance()->getLightAmbient();
      GLfloat *diff = Engine::instance()->getLightDiffuses();
      GLfloat *spec = Engine::instance()->getLightSpeculars();
      GLfloat *pos = Engine::instance()->getLightPositions();
      GLfloat *intens = Engine::instance()->getLightIntensities();
      switch ( which )
      {
        case Object::LIGHT_AMBIENT:
          if (_isLit && amb) {
            glUniform4fv( _handles[Object::LIGHT_AMBIENT], 1, amb);
          }
          break;
        case Object::LIGHT_DIFFUSE:
          if (_isLit && diff && numL && *numL > 0) {
            glUniform4fv( _handles[Object::LIGHT_DIFFUSE], *numL, diff);
          }
          break;
        case Object::LIGHT_SPECULAR:
          if (_isLit && spec && numL && *numL > 0) {
            glUniform4fv( _handles[Object::LIGHT_SPECULAR], *numL, spec);
          }
          break;
        case Object::NUM_LIGHTS:
          if (_isLit && numL && *numL > 0) {
            glUniform1i( _handles[Object::NUM_LIGHTS], *numL);
          }
          break;
        case Object::LIGHT_POSITIONS:
          if (_isLit && pos && numL && *numL > 0) {
            glUniform4fv( _handles[Object::LIGHT_POSITIONS], *numL, pos);
          }
          break;
        case Object::LIGHT_INTENSITY:
          if (_isLit && intens && numL && *numL > 0) {
            glUniform1fv( _handles[Object::LIGHT_INTENSITY], *numL, intens);
          }
          break;
        default:
          throw std::invalid_argument( "Unknown Uniform Handle Enumeration." );
      }
      break;
    }
  }

  if (glGetError()) {
    fprintf( stderr, "ERROR: Object::send() failed for [%s][%u] (handle was %u)\n", _name.c_str(), which, _handles[which] );
    //exit(EXIT_FAILURE);
  }

}

/**
 Returns the Object's current shader.
 Defined because C++ will not let you overload an overrided function,
 without re-overloading it in the derived class.

 @return a GLuint handle to the shader program used by this Object.
 **/
GLuint Object::shader( void ) {
  
  return Scene::shader();
  
}

/**
 Sets the shader to be used by this object.
 Triggers a query of the shader program,
 for the locations of the Uniform locations
 that the object needs.

 @param newShader a GLuint handle to the shader program to use.

 @return None.
 **/
void Object::shader( GLuint newShader ) {
  
  // Cache the shader handle for later.
  Scene::shader( newShader );
  
  // We have to use the program to query the glUniform locations.
  Engine::instance()->switchShader( newShader );
  
  relinkUniforms();

}

/**
 * TODO: FIXME: GROSS BAD UGH
 */
void Object::relinkUniforms( void ) {
  // Re-link our Uniforms to this shader.
  UniformMap::iterator it;
  for ( it = _uniformMap.begin(); it != _uniformMap.end(); ++it ) {
    link( it->first, it->second );
  }
}

/**
 * Apply an animation callback function to this Object.
 * Works once only: Does not save the function or automatically run on idle.
 * @param anim_func The transformation/animation function to apply.
 */
void Object::animation( void (*anim_func)( TransCache &arg ) ) {
  anim_func( this->_trans );
  Object::propagateOLD();
}

/**
 * Scene-graph changes are not automatically applied to children.
 * For efficiency reasons, you need to call propagateOLD() manually.
 * TODO: Deprecated.
 */
void Object::propagateOLD( void ) {
  
  // This function is deprecated: If we detect the "new"
  // Scene Graph is in use, do NOTHING.
  if (_trans.size() > 0) return;
  //fprintf( stderr, "\n" );
  //fprintf( stderr, "propagateOLD called on %s\n", _name.c_str() );
  
  std::list< Object* >::iterator it;
  
  //std::cerr << "Calling CALCCTM:\n";
  //Update my Object's CTM...
  this->_trans.calcCTM();
  
  //send my otm as the ptmOLD to all of my children.
  for ( it = _list.begin(); it != _list.end(); ++it ) {
    (*it)->_trans.ptmOLD( this->_trans.otm() );
    //Tell that child to update his CTM and propagateOLD.
    (*it)->propagateOLD();
  }
  
  //std::cerr << "{" << _name << "::otm:" << this->_trans.otm() << "}\n";
  
}

/**
 * sceneCascade sends our current inheritable transformation matrix
 * to all of our children, /if/ it is marked as needing to be updated.
 */
void Object::sceneCascade( void ) {
  if (_trans.cascade()) {
    std::list< Object* >::iterator it;
    for ( it = _list.begin(); it != _list.end(); ++it ) {
      (*it)->_trans.ptm( _trans.pitm() );
    }
    _trans.cascade( false );
  }
}

/**
 * Obtain the vec4 representative of the Object's current position in space.
 * @return vec4 representing the Object's position in space.
 */
vec4 Object::position( void ) const {
  
  mat4 theOTM = this->_trans.otm();
  
  return vec4( theOTM[0][3], theOTM[1][3], theOTM[2][3], 1.0 );
}

/**
 * Retrieve a pointer to this object's morph target.
 * @return An Object pointer to the morph target.
 */
Object* Object::morphTarget( void ) const {
  
  return _morphTarget;
  
}

/**
 * Instantiate a new morphing target.
 * @param shader The shader to use for the new morphing target.
 * NOT USED for rendering the object, but Objects cannot be instantiated
 * without a shader, so here it is.
 *
 * @return A pointer to the newly created target.
 */
Object* Object::genMorphTarget( GLuint shader ) {
  
  // If the user declines to specify a shader,
  // Use whichever one we're using.
  if (shader == 0) shader = this->shader();

  Object *obj = new Object( this->_name + "_morph", shader );
  _morphTarget = obj;
  return obj;
  
}

/**
 * Retrieve the morph Percentage of this object.
 * @return The morph percentage, as a float.
 */
float Object::morphPercentage( void ) const {
  
  return _morphPercentage;
}

/**
 * Set the morph percentage of this Object.
 * @param newPercentage The new morphing percentage.
 */
void Object::morphPercentage( const float newPercentage ) {
  _morphPercentage = newPercentage;
}

bool Object::morphEnabled(void) const {
    return _morphEnabled;
}

/**
 * Set the morph Enabled of this Object.
 * @param newEnabled The new morphing Enabled.
 */
void Object::morphEnabled( const bool enabled ) {
  _morphEnabled = enabled;
}

/**
 * Obliterate the morph target for this object.
 */
void Object::destroyMorphTarget( void ) {
  
  if ( _morphTarget != NULL ) {
    delete _morphTarget;
    _morphTarget = NULL;
  }
  
}

/**
 * Retrieve the number of _vertices this object has.
 * @return An integer representing the number of vertices the object has.
 */
size_t Object::numberOfPoints( void ) {
  return _vertices.size();
}

/**
* Adds material data to the object
* @param newMaterial The material to set to the object
**/
void Object::addMaterial(Material *newMaterial) {
 
  _material = newMaterial;
  this->color = _material->getDiffuse();

  _colors.clear();

  for ( size_t i = 0; i < _vertices.size(); ++i ) {
    _colors.push_back(vec4(color, 1));
  }
}

/**
 * Retrieves the material object so we can get properties from it
 * @return The material of this object
 */
Material *Object::getMaterial()
{
  return _material;
}

/**
 * Retrieve a vec3 containing the maximum x,y,z values found in this Object.
 * @return vec3( maxX, maxY, maxZ )
 */
Angel::vec3 Object::getMax( void ) {
  
  Angel::vec3 max = Angel::vec3( -INFINITY, -INFINITY, -INFINITY);
  for ( size_t i = 0; i < _vertices.size(); ++i ) {
    if ( _vertices[i].x > max.x ) max.x = _vertices[i].x;
    if ( _vertices[i].y > max.y ) max.y = _vertices[i].y;
    if ( _vertices[i].z > max.z ) max.z = _vertices[i].z;
  }

  return max;

}

/**
 * Retrieve a vec3 containing the minimum x,y,z values found in this Object.
 * @return vec3( minX, minY, minZ )
 */
Angel::vec3 Object::getMin( void ) {
  
  Angel::vec3 min = Angel::vec3( INFINITY, INFINITY, INFINITY);
  for ( size_t i = 0; i < _vertices.size(); ++i ) {
    if ( _vertices[i].x < min.x ) min.x = _vertices[i].x;
    if ( _vertices[i].y < min.y ) min.y = _vertices[i].y;
    if ( _vertices[i].z < min.z ) min.z = _vertices[i].z;
  }

  return min;

}

Angel::vec4 Object::getRealMax( void ) {
  return _trans.otm() * Angel::vec4( getMax(), 1.0 );
}
Angel::vec4 Object::getRealMin( void ) {
  return _trans.otm() * Angel::vec4( getMin(), 1.0 );
}

void Object::sceneToRaytracer( RayTracer &rt ) {
  rt.addTransformation( _trans.otm() );
  Scene::sceneToRaytracer( rt );
}

#define vec3ify( invec ) vec3( invec.x, invec.y, invec.z )
void Object::bufferToRaytracer( RayTracer &rt ) {

  vec3 diffuse, ambient, specular;
  float shininess = 0.0, refract = 0.0;
  const float reflect = 0.5;

  // If _material doesn't exist, try to use colors.
  // If no colors exist, make a default material.
  if ((_colors.size() < _vertices.size()) && (_material == NULL)) {
    _material = new Material();
    diffuse = _material->getDiffuse();
    ambient = _material->getAmbient();
    specular = _material->getSpecular();
    shininess = _material->getShininess();
    refract = _material->getRefract();
    gprint( PRINT_INFO, "Using DEFAULT material properties.\n" );
  }
  if (_material) {
    gprint( PRINT_DEBUG, "Using material properties.\n" );
    diffuse = _material->getDiffuse();
    ambient = _material->getAmbient();
    specular = _material->getSpecular();
    shininess = _material->getShininess();
    refract = _material->getRefract();
  }

  std::vector< Angel::vec4 >::iterator it;

  size_t i = 0;
  for ( it = _vertices.begin(); it != _vertices.end(); ++it, i += 3 ) {
    vec4 a = *it;
    if ( ++it == _vertices.end() ) break;
    vec4 b = *(it);
    if ( ++it == _vertices.end() ) break;
    vec4 c = *(it);

    a = _trans.otm() * a;
    b = _trans.otm() * b;
    c = _trans.otm() * c;
 
    if (_material) {
      rt.addTriangle( vec3ify(a), vec3ify(b), vec3ify(c),
		      diffuse, ambient, specular, 
		      shininess, reflect, refract,
		      _normals.at(i) );
    } else {
      vec4 color4 = _colors.at(i) + _colors.at(i+1) + _colors.at(i+2);
      color4 = color4 / 3.0;

      rt.addTriangle( vec3ify(a), vec3ify(b), vec3ify(c),
		      vec3ify(color4), vec3ify(color4), vec3ify(color4),
		      //vec3(0.5,0.5,0.5), vec3(0.05,0.05,0.05), vec3(1.0,1.0,1.0),
		      100.0,0.3,0.0,
		      _normals.at(i) );
    }
  }
  
  Scene::bufferToRaytracer( rt );
  
}

/*void Object::setLights(GLfloat* ambient, GLint* numlights, GLfloat* positions, GLfloat* diffuse, GLfloat* specular)
{
  _isLit = true;
  _lightAmbient = ambient;
  _numLights = numlights;
  _lightPositions = positions;
  _lightDiffuse = diffuse;
  _lightSpecular = specular;

}
*/
