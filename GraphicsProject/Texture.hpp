/**
 * @file Texture.hpp
 * @author John Huston
 * @authors Etay Meiri, John Huston
 * @date 2013-03-28
 * @brief Texturing Module!
 * @details Based -very- heavily on Etay Meiri's work at ogldev.
 **/

#ifndef __TEXTURE_HPP
#define __TEXTURE_HPP

#include "platform.h"
#include <Magick++.h>

class Texture {
public:
  Texture( GLenum _textureTarget );
  ~Texture();

  bool load( const std::string& filename );
  void bind( GLenum textureUnit );
  void buffer( void );

private:
  GLenum _textureTarget;
  GLuint _textureObj;
  Magick::Image *_image;
  Magick::Blob _blob;

};

#endif