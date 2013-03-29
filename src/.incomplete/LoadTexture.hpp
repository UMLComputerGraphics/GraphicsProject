#include <SOIL.h>
#include <pthread.h>
#include "platform.h"

class txload {
public:
  const char *filename;
  GLuint TextEnum;
  GLuint gShader;
  const char *uniform;
  int num;

  txload( const char *filename, GLuint TextEnum, GLuint gShader,
          const char *uniform, int num );
};

void *load_texture( void *arg );
