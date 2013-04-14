/**
 * @file TextureMan.cpp
 * @author John Huston
 * @date 2013-03-30
 * @brief Texture Management Header
 * @details @see TextureManagement class!
 */

#include "Engine.hpp"
#include "TextureMan.hpp"
#include <stdexcept>

unsigned TextureManagement::maxTexUnits = 0;

TextureManagement::TextureManagement( void ) {
  // Peekaboo: Nothing here for you.
}

TextureManagement::~TextureManagement( void ) {
  // Toodle-hoo: Empty destructor for you.
}

unsigned TextureManagement::maxTextures( void ) {
  if (TextureManagement::maxTexUnits > 0)
    return TextureManagement::maxTexUnits;
  else {
    int max;
    glGetIntegerv( GL_MAX_TEXTURE_IMAGE_UNITS, &max );
    if (max > 0) {
      TextureManagement::maxTexUnits = max;
      return max;
    } else {
      throw std::out_of_range( "GL_MAX_TEXTURE_IMAGE_UNITS was negative!" );
    }
  }
}

unsigned TextureManagement::getTexUnit( unsigned n ) {
  // Yes, GL_TEXTUREn definitions are guaranteed to be contiguous
  // By the OpenGL refspec.
  return ((GL_TEXTURE0) + n);
}

unsigned TextureManagement::numUsed( void ) {
  return _texUnits.size();
}

unsigned TextureManagement::assign( Texture *newTexture ) {
  TexMap::iterator it;
  GLenum assignedTexUnit;
  for ( size_t i = 0; i < maxTextures(); ++i ) {
    it = _texUnits.find( i );
    if (it == _texUnits.end()) {
      // This key not found. That means this slot is open!
      assignedTexUnit = getTexUnit(i);
      newTexture->bind( assignedTexUnit );
      _texUnits[i] = newTexture;

      // FIXME: Mac OSX Hack
      rebind();

      // Return which texture unit we bound this texture to.
      return i;
    }
  }

  throw std::out_of_range( "No texture units available, oh no!" );
}

void TextureManagement::rebind( void ) {
  TexMap::iterator it;
  for (it = _texUnits.begin(); it != _texUnits.end(); ++it) {
    it->second->bind( getTexUnit( it->first ) );
  }
}
