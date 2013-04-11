/**
 * @file InitShader.hpp
 * @authors Ed Angel, Eric McCann, Nick St.Pierre
 * @date 2013-03-13
 * @brief Provides a wrapper utility for quickly
 * linking against glsl programs.
 **/

#ifndef __INIT_SHADER_H
#define __INIT_SHADER_H

// Fixme: Not Used?
//#define GL_INVALID_SHADER -1

/**
 * Namespace to encompass Ed Angel's functions and classes.
 * @author Ed Angel
 * @date 2013-03-30
 */
namespace Angel {
  /**
   * InitInitShader is a preparation step allowing executables to be invoked from
   * working directories OTHER than the one containing the shaders directory
   *
   * @param binloc argv[0] from main
   *
   * @return The relative path from the working directory to the directory containing shaders folder
   */
  const char* InitInitShader( const char* binloc );

  /**
   * InitShader takes two or three shader sourcefiles and compiles them into a
   * shader program.
   *
   * @param vShaderFile the vertex shader source file
   * @param fShaderFile the fragment shader source file
   * @param gShaderFile the geometry shader source file (omit if not applicable)
   *
   * @return A handle to the compiled glsl program.
   */
  GLuint InitShader( const char* vShaderFile, const char* fShaderFile,
                     const char* gShaderFile );

  /**
   * InitShader takes two or three shader sourcefiles and compiles them into a
   * shader program.
   *
   * @param vShaderFile the vertex shader source file
   * @param fShaderFile the fragment shader source file
   *
   * @return A handle to the compiled glsl program.
   */
  GLuint InitShader( const char* vShaderFile, const char* fShaderFile );

  /** TODO: udate this
   * InitShader takes three shader sourcefiles and compiles them into a
   * shader program.
   *
   * @param vShaderFile the vertex shader source file
   * @param gShaderFile the geometry shader source file
   * @param fShaderFile the fragment shader source file
   *
   * @return A handle to the compiled glsl program.
   */
  GLuint InitShader( const char* vShaderFile, const char* fShaderFile,
                     const char* gShaderFile, GLuint gs_inType, GLuint gs_outType,
                     int gs_numVertOut ) ;


}

#endif
