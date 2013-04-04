#include <SOIL.h>
#include <pthread.h>
#include "platform.h"
#include <cstdio>
#include "LoadTexture.hpp"

txload::txload( const char *filename, GLint TextEnum, GLint gShader,
                const char *uniform, int num ) {
  this->filename = filename;
  this->TextEnum = TextEnum;
  this->gShader = gShader;
  this->uniform = uniform;
  this->num = num;
}

void *load_texture( void *arg ) {
  
  GLint handle;
  txload *ptr = (txload *) arg;
  
  fprintf( stderr, "{%s, %u, %u, %s, %d}\n", ptr->filename, ptr->TextEnum,
           ptr->gShader, ptr->uniform, ptr->num );
  
  handle = SOIL_load_OGL_texture(
      ptr->filename,
      SOIL_LOAD_AUTO,
      SOIL_CREATE_NEW_ID,
      SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB
      | SOIL_FLAG_COMPRESS_TO_DXT );
  fprintf( stderr, "Handle returned from SOIL: %d\n", handle );
  if ( handle == -1 ) {
    fprintf( stderr, "Some really bad shit just happened, sorry.\n" );
    return NULL;
  }
  
  GLint gSampler = glGetUniformLocation( ptr->gShader, ptr->uniform );
  fprintf( stderr, "For %s, got handle %d\n", ptr->uniform, gSampler );
  glUniform1i( gSampler, ptr->num );
  
  glActiveTexture( ptr->TextEnum );
  glBindTexture( GL_TEXTURE_2D, handle );
  glEnable( GL_TEXTURE_2D );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
  
  return NULL;
  
}
