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

Texture::Texture( GLenum textureTarget ) {
  _textureTarget = textureTarget;
  _image = NULL;
}

Texture::~Texture( void ) {
  
}

bool Texture::load( const std::string &filename ) {
  
  try {
    _image = new Magick::Image( filename );
    _image->write( &_blob, "RGBA" );
  } catch ( Magick::Error& Error ) {
    std::cerr << "Error loading texture '" << filename << "': " << Error.what()
              << std::endl;
    return false;
  }
  return true;
  
}

void Texture::buffer( void ) {
  
  glGenTextures( 1, &_textureObj );
  glBindTexture( _textureTarget, _textureObj );
  glTexImage2D( _textureTarget, 0, GL_RGB, _image->columns(), _image->rows(),
                -0.5, GL_RGBA, GL_UNSIGNED_BYTE, _blob.data() );
  glTexParameterf( _textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
  glTexParameterf( _textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  
}

void Texture::bind( GLenum textureUnit ) {
  glActiveTexture( textureUnit );
  glBindTexture( _textureTarget, _textureObj );
}

/*
 GLenum _textureTarget;
 GLuint _textureObj;
 Magick::Image *_image;
 Magick::Blob _blob;
 */
