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

#ifdef __APPLE__ // include Mac OS X versions of headers
#include <Magick++.h>
#else // non-Mac OS X operating systems
#include <ImageMagick/Magick++.h>
#endif // __APPLE__

/**
 * Texture as a class represents the state and data needed to
 * accurately track and bind a single Texture in OpenGL.
 * Authored primarily by Etay Meiri, small modifications by John Huston.
 *
 * @author Etay Meiri, John Huston
 * @date 2013-03-30
 */
class Texture {
public:

  /**
   * Initialize a new texture of a given type.
   * @param textureTarget The type of texture to initialize, e.g, TEXTURE_2D.
   */
  Texture( GLenum textureTarget );

  /**
   * Default destructor.
   */
  ~Texture();

  /**
   * Perform the actual loading and conversion of the data.
   * @param filename The filename to load as a texture.
   * @return bool: true if we succeed, false otherwise.
   */
  bool load( const std::string& filename );

  /**
   * Registers the texture with OpenGL.
   */
  void buffer( void );

  /**
   * Binds this texture to a specific Texture Unit.
   * @param textureUnit The Texture Unit to bind to.
   */
  void bind( GLenum textureUnit );

private:
  /** The type of texture we are, e.g, TEXTURE_2D. **/
  GLenum _textureTarget;
  /** A handle to the Texture object on the GPU. **/
  GLuint _textureObj;
  /** A pointer to the ImageMagick image object for the loaded data. **/
  Magick::Image *_image;
  /** Raw RGBA buffer object that we will load into the GPU. **/
  Magick::Blob _blob;
  
};

#endif
