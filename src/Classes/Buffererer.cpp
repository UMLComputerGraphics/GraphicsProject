/*
 * Buffererer.cpp
 *
 *  Created on: May 7, 2013
 *      Author: eric
 */

#include "Buffererer.h"

////////// BuffererererererererManager

// There ain't room enough in this town for two of these
BufferererManager *BufferererManager::instance( void ) {
  static BufferererManager *_instance;
  if ( _instance == NULL ) _instance = new BufferererManager();
  return _instance;
}

// Keep track of instances of the buffererers, so we can keep the right number of them around
void BufferererManager::add(Buffererer *instance)
{
  {
    // need to lock in add and remove, because it's the buffererer's job to kill themselves
    boost::mutex::scoped_lock l(birthlock);
    int s = _instances.size();
    if (s==0)
    {
      _maxTextures = glutGet(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS);
      gprint(PRINT_INFO, "maxtextures = %d\n", _maxTextures);
    }
    gprint(PRINT_DEBUG, "Starting the  = %dth buffererer\n", s+1);
    _instances[s] = instance;
  }
}

/////////// Bufferererererererererererererer

Buffererer::~Buffererer()
{
  // TODO Auto-generated destructor stub
}

