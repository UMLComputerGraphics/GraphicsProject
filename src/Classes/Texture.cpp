/**
 * @file Texture.cpp
 * @author John Huston
 * @authors Etay Meiri, John Huston
 * @date 2013-03-28
 * @brief Texturing Module!
 * @details Based -very- heavily on Etay Meiri's work at ogldev.
 **/

#ifdef __APPLE__ // include Mac OS X versions of headers
#include <Magick++.h>
#else // non-Mac OS X operating systems
#include <ImageMagick/Magick++.h>
#endif // __APPLE__
#include <iostream>
#include <cstdlib>
#include "Texture.hpp"
#include "platform.h"
#include "Util.hpp"

/**
 * Initialize a new texture of a given type.
 * @param textureTarget The type of texture to initialize, e.g, TEXTURE_2D.
 */
Texture::Texture( GLenum textureTarget ) {
  _textureTarget = textureTarget;
  _image = NULL;
  _textureObj = -1;
}

/**
 * Default destructor.
 */
Texture::~Texture( void ) {
  if (_image) delete _image;  
}

/**
 * Perform the actual loading and conversion of the data.
 * @param filename The filename to load as a texture.
 * @return bool: true if we succeed, false otherwise.
 */
bool Texture::load( const std::string &filename ) {
  
  try {
    std::string relativePath = Util::getRelativePath( filename.c_str() );
    _image = new Magick::Image( relativePath.c_str() );
    _image->write( &_blob, "RGBA" );
  } catch ( Magick::Error& Error ) {
    std::cerr << "Error loading texture '" << filename.c_str() << "': " << Error.what()
              << std::endl;
    return false;
  }
  return true;
  
}

/**
 * Registers the texture with OpenGL.
 */
void Texture::buffer( void ) {
  
/* We need to bind this texture as active in order to set some properties
 * of it and buffer the data. Therefore, we're going to poll the card
 * to see what the currently active texture(s) are, and re-set them
 * when we're done doing what we need to.
 */

 /* 
  GLenum query;
  int result;
  switch (_textureTarget) {
  case GL_TEXTURE_1D:
    query = GL_TEXTURE_BINDING_1D;
    break;
  case GL_TEXTURE_1D_ARRAY:
    query = GL_TEXTURE_BINDING_1D_ARRAY;
    break;
  default:
  case GL_TEXTURE_2D:
    query = GL_TEXTURE_BINDING_2D;
    break;
  case GL_TEXTURE_2D_ARRAY:
    query =  GL_TEXTURE_BINDING_2D_ARRAY;
    break;
  case GL_TEXTURE_2D_MULTISAMPLE:
    query =  GL_TEXTURE_BINDING_2D_MULTISAMPLE;
    break;
  case GL_TEXTURE_2D_MULTISAMPLE_ARRAY:
    query =    GL_TEXTURE_BINDING_2D_MULTISAMPLE_ARRAY;
    break;
  case GL_TEXTURE_3D:
    query =    GL_TEXTURE_BINDING_3D;
    break;
  case GL_TEXTURE_BUFFER:
    query =   GL_TEXTURE_BINDING_BUFFER;
    break;
  case GL_TEXTURE_CUBE_MAP:
    query =   GL_TEXTURE_BINDING_CUBE_MAP;
    break;
  case GL_TEXTURE_RECTANGLE:
    query =   GL_TEXTURE_BINDING_RECTANGLE;
    break;
  }
  glGetIntegerv( query, &result );
*/

  glGenTextures( 1, &_textureObj );
  glBindTexture( _textureTarget, _textureObj );
  glTexImage2D( _textureTarget, 0, GL_RGB, _image->columns(), _image->rows(),
                -0.5, GL_RGBA, GL_UNSIGNED_BYTE, _blob.data() );
  glTexParameterf( _textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
  glTexParameterf( _textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  
  // Put back the previous texture where we found it.
  // glBindTexture( _textureTarget, result );

}

/**
 * Binds this texture to a specific Texture Unit.
 * @param textureUnit The Texture Unit to bind to.
 */
void Texture::bind( GLenum textureUnit ) {
  glActiveTexture( textureUnit );
  glBindTexture( _textureTarget, _textureObj );
}
