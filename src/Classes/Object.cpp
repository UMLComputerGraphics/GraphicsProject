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

using Angel::vec4;
using Angel::mat4;

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

  // Create room for our GLUniform _handles
  if ( DEBUG )
    fprintf( stderr, "\nCreating %d handles for uniforms\n", Object::END );
  _handles.resize( Object::END );
  
  // Associate this Object with the shader.
  shader( gShader );
  
  // Set our _name from the constructor...
  this->_name = name;
  
  /* Initialize our draw mode to GL_TRIANGLES until informed otherwise. */
  _drawMode = GL_TRIANGLES;
  
  // Get Uniform _handles for the following shader variables
  link( Object::IS_TEXTURED, "fIsTextured" );
  link( Object::OBJECT_CTM, "OTM" );
  link( Object::MORPH_PCT, "morphPercentage" );
  link( Object::TEX_SAMPLER, "sampler" );
  
  //Default to "Not Textured"
  this->_isTextured = false;
  
  // Linear Interpolation Demo: Morph Percentage
  this->_morphPercentage = 1.0;
  // Pointer to an Object to Morph to.
  this->_morphTarget = NULL;
  
  /* Create our VAO, which is our handle to all 
   the rest of the following information. */glGenVertexArrays( 1, &_vao );
  glBindVertexArray( _vao );
  GLuint glsl_uniform;
  
  /* Create (Eight) VBOs: One each for Positions, Colors, Normals, 
   Textures, draw Order; Three for Morphing (Position,Colors,Normals.) */glGenBuffers(
      NUM_BUFFERS, _buffer );
  
  /* Create the Vertex _buffer and link it with the shader. */glBindBuffer(
      GL_ARRAY_BUFFER, _buffer[VERTICES] );
  glsl_uniform = glGetAttribLocation( gShader, "vPosition" );
  glEnableVertexAttribArray( glsl_uniform );
  glVertexAttribPointer( glsl_uniform, 4, GL_FLOAT, GL_FALSE, 0, 0 );
  
  /* Create the MORPH Vertex _buffer and link it with the shader. */glBindBuffer(
      GL_ARRAY_BUFFER, _buffer[VERTICES_MORPH] );
  glsl_uniform = glGetAttribLocation( gShader, "vPositionMorph" );
  glEnableVertexAttribArray( glsl_uniform );
  glVertexAttribPointer( glsl_uniform, 4, GL_FLOAT, GL_FALSE, 0, 0 );
  
  /* Create the Normal _buffer and link it with the shader. */glBindBuffer(
      GL_ARRAY_BUFFER, _buffer[NORMALS] );
  glsl_uniform = glGetAttribLocation( gShader, "vNormal" );
  glEnableVertexAttribArray( glsl_uniform );
  glVertexAttribPointer( glsl_uniform, 3, GL_FLOAT, GL_FALSE, 0, 0 );
  
  /* Create the Normal MORPH _buffer and link it with the shader. */glBindBuffer(
      GL_ARRAY_BUFFER, _buffer[NORMALS_MORPH] );
  glsl_uniform = glGetAttribLocation( gShader, "vNormalMorph" );
  glEnableVertexAttribArray( glsl_uniform );
  glVertexAttribPointer( glsl_uniform, 3, GL_FLOAT, GL_FALSE, 0, 0 );
  
  /* Create the Color _buffer and link it with the shader. */glBindBuffer(
      GL_ARRAY_BUFFER, _buffer[COLORS] );
  glEnable( GL_BLEND );
  glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
  glsl_uniform = glGetAttribLocation( gShader, "vColor" );
  glEnableVertexAttribArray( glsl_uniform );
  glVertexAttribPointer( glsl_uniform, 4, GL_FLOAT, GL_FALSE, 0, 0 );
  
  /* Create the Color Morph _buffer and link it with the shader. */glBindBuffer(
      GL_ARRAY_BUFFER, _buffer[COLORS_MORPH] );
  glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
  glsl_uniform = glGetAttribLocation( gShader, "vColorMorph" );
  glEnableVertexAttribArray( glsl_uniform );
  glVertexAttribPointer( glsl_uniform, 4, GL_FLOAT, GL_FALSE, 0, 0 );
  
  /* Create the texture Coordinate _buffer and link it with the shader. */glBindBuffer(
      GL_ARRAY_BUFFER, _buffer[TEXCOORDS] );
  glsl_uniform = glGetAttribLocation( gShader, "vTex" );
  glEnableVertexAttribArray( glsl_uniform );
  glVertexAttribPointer( glsl_uniform, 2, GL_FLOAT, GL_FALSE, 0, 0 );
  
  if ( DEBUG )
    fprintf( stderr, "buffhandles: %u %u %u %u %u %u %u %u\n",
             _buffer[VERTICES], _buffer[NORMALS], _buffer[COLORS],
             _buffer[TEXCOORDS], _buffer[INDICES], _buffer[VERTICES_MORPH],
             _buffer[NORMALS_MORPH], _buffer[COLORS_MORPH] );
  
  /* Create the Drawing Order _buffer, but we don't need to link it 
   with any uniform,
   because we won't be accessing this data directly. (I.e, the numbers here
   are not important once we are in the Vertex shader. */glBindBuffer(
      GL_ELEMENT_ARRAY_BUFFER, _buffer[INDICES] );
  
  /* Unset the VAO context. */
  glBindVertexArray( 0 );
}

/**
 * Default destructor.
 */
Object::~Object( void ) {
  
  destroyMorphTarget();
  
}

/**
 * draw method: Render this object to the screen _buffer.
 */
void Object::draw( void ) {
  
  glBindVertexArray( _vao );
  
  // Check to see if the correct shader program is engaged.
  GLint currShader;
  glGetIntegerv( GL_CURRENT_PROGRAM, &currShader );
  if ( (GLuint) currShader != shader() ) {
    
    Camera *activeCamera = Engine::instance()->cams()->active();
    
    //if (DEBUG) std::cerr << "Switching shading context.\n";
    
    // Set OpenGL to use this object's shader.
    glUseProgram( shader() );
    
    // Set the Active Camera's shader to the Object's shader.
    activeCamera->shader( shader() );
    
    // send the Camera's info to the new shader.
    activeCamera->view();
  }
  
  send( Object::IS_TEXTURED );
  send( Object::OBJECT_CTM );
  send( Object::MORPH_PCT );
  send( Object::TEX_SAMPLER );
  
  //  this->morphPercentage() == -1.0 ? ; : send( Object::MORPH_PCT );
  
  /* Are we using a draw order? */
  if ( _indices.size() > 1 ) glDrawElements( _drawMode, _indices.size(),
                                             GL_UNSIGNED_INT, 0 );
  else glDrawArrays( _drawMode, 0, _vertices.size() );
  
  glBindVertexArray( 0 );
  
  // draw all of our Children.
  // (With clothes on, pervert.)
  Scene::draw();
  
}

/**
 * buffer all of our data: Vertices, TexUVs, Normals,
 * Indices, Colors and Morph Buffers.
 */
void Object::buffer( void ) {
  
  glBindVertexArray( _vao );
  
  glBindBuffer( GL_ARRAY_BUFFER, _buffer[VERTICES] );
  glBufferData( GL_ARRAY_BUFFER, sizeof(Angel::vec4) * _vertices.size(),
                &(_vertices[0]), GL_STATIC_DRAW );
  
  glBindBuffer( GL_ARRAY_BUFFER, _buffer[NORMALS] );
  glBufferData( GL_ARRAY_BUFFER, sizeof(Angel::vec3) * _normals.size(),
                &(_normals[0]), GL_STATIC_DRAW );
  
  glBindBuffer( GL_ARRAY_BUFFER, _buffer[COLORS] );
  glBufferData( GL_ARRAY_BUFFER, sizeof(Angel::vec4) * _colors.size(),
                &(_colors[0]), GL_STATIC_DRAW );
  
  /* Without the following workaround code,
   Mac OSX will segfault attempting to access
   the texcoordinate buffers on nontextured objects. */
  if ( _texUVs.size() == 0 && _isTextured == false ) {
    _texUVs.push_back( Angel::vec2( -1, -1 ) );
  } else if ( _texUVs.size() > 1 ) {
    /* Yes, this workaround prevents us from having
     textured objects with only one point.
     Oops. */
    _isTextured = true;
  }
  
  glBindBuffer( GL_ARRAY_BUFFER, _buffer[TEXCOORDS] );
  glBufferData( GL_ARRAY_BUFFER, sizeof(Angel::vec2) * _texUVs.size(),
                (_texUVs.size() ? &(_texUVs[0]) : NULL), GL_STATIC_DRAW );
  
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, _buffer[INDICES] );
  glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * _indices.size(),
                &(_indices[0]), GL_STATIC_DRAW );
  
  glBindVertexArray( 0 );
  
}

/**
 * buffer only the Morph-related buffers.
 */
void Object::bufferMorphOnly( void ) {
  
  glBindVertexArray( _vao );
  
  glBindBuffer( GL_ARRAY_BUFFER, _buffer[VERTICES_MORPH] );
  glBufferData( GL_ARRAY_BUFFER,
                sizeof(Angel::vec4) * morphTarget()->_vertices.size(),
                &(morphTarget()->_vertices[0]), GL_STATIC_DRAW );
  
  glBindBuffer( GL_ARRAY_BUFFER, _buffer[NORMALS_MORPH] );
  glBufferData( GL_ARRAY_BUFFER,
                sizeof(Angel::vec3) * morphTarget()->_normals.size(),
                &(morphTarget()->_normals[0]), GL_STATIC_DRAW );
  
  glBindBuffer( GL_ARRAY_BUFFER, _buffer[COLORS_MORPH] );
  glBufferData( GL_ARRAY_BUFFER,
                sizeof(Angel::vec4) * morphTarget()->_colors.size(),
                &(morphTarget()->_colors[0]), GL_STATIC_DRAW );
  
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
 * FIXME: This is a junk, nonflexible method.
 * It would be better if you didn't think of this as being here.
 *
 * @param filename an array of strings to load textures from.
 */
void Object::terrainTexture( const char** filename ) {
  
  tick.tock();
  glBindVertexArray( _vao );
  
  Texture **textures = (Texture **) malloc( sizeof(Texture*) * 5 );
  textures[0] = new Texture( GL_TEXTURE_2D );
  textures[1] = new Texture( GL_TEXTURE_2D );
  textures[2] = new Texture( GL_TEXTURE_2D );
  textures[3] = new Texture( GL_TEXTURE_2D );
  textures[4] = new Texture( GL_TEXTURE_2D );
  
  textures[0]->load( filename[0] );
  textures[1]->load( filename[1] );
  textures[2]->load( filename[2] );
  textures[3]->load( filename[3] );
  textures[4]->load( filename[4] );
  tick.tock();
  fprintf( stderr, "Texture loads: %lu\n", tick.delta() );
  
  textures[0]->buffer();
  textures[1]->buffer();
  textures[2]->buffer();
  textures[3]->buffer();
  textures[4]->buffer();
  tick.tock();
  fprintf( stderr, "Texture buffering: %lu\n", tick.delta() );
  
  textures[0]->bind( GL_TEXTURE0 );
  textures[1]->bind( GL_TEXTURE1 );
  textures[2]->bind( GL_TEXTURE2 );
  textures[3]->bind( GL_TEXTURE3 );
  textures[4]->bind( GL_TEXTURE4 );
  
  GLuint gSampler0 = glGetUniformLocation( shader(), "gSampler0" );
  glUniform1i( gSampler0, 0 );
  GLuint gSampler1 = glGetUniformLocation( shader(), "gSampler1" );
  glUniform1i( gSampler1, 1 );
  GLuint gSampler2 = glGetUniformLocation( shader(), "gSampler2" );
  glUniform1i( gSampler2, 2 );
  GLuint gSampler3 = glGetUniformLocation( shader(), "gSampler3" );
  glUniform1i( gSampler3, 3 );
  GLuint gSampler4 = glGetUniformLocation( shader(), "gSampler4" );
  glUniform1i( gSampler4, 4 );
  tick.tock();
  fprintf( stderr, "Texture binding and sending sampler uniforms: %lu\n", tick.delta() );
  
  glBindVertexArray( 0 );
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
  
  if ( which >= _handles.size() ) {
    fprintf(
        stderr,
        "WARNING: Ignoring request to link a uniform (#%u) beyond our handles array [%lu].\n",
        which, _handles.size() );
    return;
  }
  
  // Save the link between the Uniform and the Variable _name.
  _uniformMap[which] = name;
  
  if ( DEBUG )
    fprintf( stderr, "Linking enum[%u] with %s for object %s\n", which,
             name.c_str(), this->_name.c_str() );
  
  _handles[which] = glGetUniformLocation( shader(), name.c_str() );
  if ( DEBUG )
    fprintf( stderr, "Linking handles[%d] to %s; got %d.\n", which,
             name.c_str(), _handles[which] );
  
}

/**
 * Send a Uniform to the shader.
 * @param which The uniform to send.
 */
void Object::send( Object::UniformEnum which ) {
  switch ( which ) {
  
  case Object::IS_TEXTURED:
    glUniform1i( _handles[Object::IS_TEXTURED], (_isTextured) ? 1 : 0 );
    break;
    
  case Object::OBJECT_CTM:
    glUniformMatrix4fv( _handles[Object::OBJECT_CTM], 1, GL_TRUE,
                        this->_trans.otm() );
    break;
    
  case Object::MORPH_PCT:
    glUniform1f( _handles[Object::MORPH_PCT], this->morphPercentage() );
    
    break;
    
  case Object::TEX_SAMPLER:
    glUniform1i( _handles[Object::TEX_SAMPLER], _textureID );
    break;
    
  default:
    throw std::invalid_argument( "Unknown Uniform Handle Enumeration." );
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
  glUseProgram( newShader );
  
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
  Object::propagate();
}

/**
 * Scene-graph changes are not automatically applied to children.
 * For efficiency reasons, you need to call propagate() manually.
 */
void Object::propagate( void ) {
  
  //fprintf( stderr, "\n" );
  //fprintf( stderr, "propagate called on %s\n", _name.c_str() );
  
  std::list< Object* >::iterator it;
  
  //std::cerr << "Calling CALCCTM:\n";
  //Update my Object's CTM...
  this->_trans.calcCTM();
  
  //send my otm as the ptm to all of my children.
  for ( it = _list.begin(); it != _list.end(); ++it ) {
    (*it)->_trans.ptm( this->_trans.otm() );
    //Tell that child to update his CTM and propagate.
    (*it)->propagate();
  }
  
  //std::cerr << "{" << _name << "::otm:" << this->_trans.otm() << "}\n";
  
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
int Object::numberOfPoints( void ) {
  return _vertices.size();
}

/**
 * Retrieve a vec3 containing the maximum x,y,z values found in this Object.
 * @return vec3( maxX, maxY, maxZ )
 */
Angel::vec3 Object::getMax( void ) {
  
  Angel::vec3 max = Angel::vec3( -INFINITY, -INFINITY, -INFINITY );
  for (size_t i = 0; i < _vertices.size(); ++i ) {
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
  
  Angel::vec3 min = Angel::vec3( INFINITY, INFINITY, INFINITY );
  for (size_t i = 0; i < _vertices.size(); ++i ) {
    if ( _vertices[i].x < min.x ) min.x = _vertices[i].x;
    if ( _vertices[i].y < min.y ) min.y = _vertices[i].y;
    if ( _vertices[i].z < min.z ) min.z = _vertices[i].z;
  }

  return min;

}
