/**
 * @file TextureMan.hpp
 * @author John Huston
 * @date 2013-03-30
 * @brief Texture Management Header
 * @details @see TextureManagement class!
 */

#ifndef __TEXTURE_MAN_HPP
#define __TEXTURE_MAN_HPP

#include "Engine.hpp"
#include "OpenGL.h"

/**
 * Simple Alias for the Texture Unit allocation map.
 */
typedef std::map< unsigned, Texture* > TexMap;

/**
 * TextureManagement keeps track of which Texture Units have been assigned to which Textures.
 * @author John Huston
 * @date 2013-03-30
 */
class TextureManagement {

public:
  /**
   * Default constructor. Nothing special.
   */
  TextureManagement();

  /**
   * Default destructor. Does not violate the principle of least surprise.
   */
  ~TextureManagement();

/**
 * getTexUnit retrieves the OpenGL constant needed to
 * associate or bind with the nth texture unit.
 * @param n The texture unit to retrieve the binding constant for.
 * @return The binding constant for the nth texture unit.
 */
  static unsigned getTexUnit( unsigned n );

  /**
   * Returns the number of texture units in use.
   * @return The number of in-use texture units.
   */
  unsigned numUsed( void );

  /**
   * Query OpenGL for the number of maximum texture units supported.
   * @return The number of simultaneous texture units we can use.
   */
  static unsigned maxTextures( void );

  /**
   * assign takes a texture pointer and binds it to the first
   * available texture unit, and returns the texture unit it
   * bound it to.
   * @param newTexture The texture to bind.
   * @return The GLenum Texture Unit we bound it to.
   */
  unsigned assign( Texture *newTexture );

  void rebind( void );

private:
  /**
   * A mapping of indexed texture units (0-n) and Texture objects.
   */
  TexMap _texUnits;

  /**
   * The maximum number of Texture Units this machine can support.
   */
  static unsigned maxTexUnits;
  // Note: No underscore, because this is not a member variable per-se!

};

#endif
