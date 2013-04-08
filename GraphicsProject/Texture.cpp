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
}

/**
 * Default destructor.
 */
Texture::~Texture( void ) {

}

/**
 * Perform the actual loading and conversion of the data.
 * @param filename The filename to load as a texture.
 * @return bool: true if we succeed, false otherwise.
 */
bool Texture::load( const std::string &filename ) {

  try {
    _image = new Magick::Image( Util::getRelativePath(filename.c_str()) );
    _image->write( &_blob, "RGBA" );
  } catch ( Magick::Error& Error ) {
    std::cerr << "Error loading texture '" << Util::getRelativePath(filename.c_str()) << "': " << Error.what()
              << std::endl;
    return false;
  }
  return true;

}

/**
 * Registers the texture with OpenGL.
 */
void Texture::buffer( void ) {

  glGenTextures( 1, &_textureObj );
  glBindTexture( _textureTarget, _textureObj );
  glTexImage2D( _textureTarget, 0, GL_RGB, _image->columns(), _image->rows(),
                -0.5, GL_RGBA, GL_UNSIGNED_BYTE, _blob.data() );
  glTexParameterf( _textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
  glTexParameterf( _textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

}

/**
 * Binds this texture to a specific Texture Unit.
 * @param textureUnit The Texture Unit to bind to.
 */
void Texture::bind( GLenum textureUnit ) {
  glActiveTexture( textureUnit );
  glBindTexture( _textureTarget, _textureObj );
}
