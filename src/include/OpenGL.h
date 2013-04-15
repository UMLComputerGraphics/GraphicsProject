/**
 * @file OpenGL.h
 * @date 2013-03-15
 * @author John Huston
 * @brief Core OpenGL includes needed for basic functionality.
 * @details Includes platform-dependent macros that need to be
 * supported by the Makefile in order to function correctly
 * on both Linux and OSX.
 */

#ifndef __OPENGL_H
#define __OPENGL_H

#ifdef __APPLE__  // include Mac OS X verions of headers
#  include <Carbon/Carbon.h>
#  include <OpenGL/OpenGL.h>
#  include <GLUT/glut.h>
#  include <OpenGL/glext.h>
#else // non-Mac OS X operating systems                                     
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <GL/freeglut_ext.h>
#endif  // __APPLE__   
#endif
