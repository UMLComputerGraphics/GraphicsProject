/**
 * @file platform.h
 * @author John Huston, Chris Compton
 * @date 2013-03-15
 * @brief Covertly re-defines functions that conflict between Linux and OSX.
 */
#ifndef __PLATFORM_H
#define __PLATFORM_H

#include "OpenGL.h"

#ifdef __APPLE__
#  define glGenVertexArrays( X, Y ) glGenVertexArraysAPPLE( X, Y )
#  define glBindVertexArray( X ) glBindVertexArrayAPPLE( X )
#  define GLEW_INIT() ;
#  define glutLeaveMainLoop() exit( EXIT_SUCCESS )
#else
#define GLEW_INIT()						\
  glewExperimental = GL_FALSE;					\
  GLenum rc = glewInit();					\
  if (rc) gprint( PRINT_ERROR, "%s\n", glewGetErrorString(rc) );
#endif

#endif

/**
 * @def GLEW_INIT()
 * A macro defined to initialize GLEW on Linux,
 * but do nothing on Apple OSX.
 */
