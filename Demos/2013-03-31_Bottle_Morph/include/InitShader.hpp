#ifndef __INIT_SHADER_H
#define __INIT_SHADER_H

namespace Angel {
  GLuint InitShader(const char* vShaderFile, const char* fShaderFile);

  GLuint InitShader(const char* vShaderFile, const char* gShaderFile, const char* fShaderFile);
}

#endif
