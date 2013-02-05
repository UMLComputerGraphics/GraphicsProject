#ifndef __PLATFORM_H
#define __PLATFORM_H

#include "OpenGL.h"

#ifdef __APPLE__
/* ALL HAIL STEVE JOBS GLORIOUS MAC MASTER RACE UBER ALLES */
#define glGenVertexArrays( X, Y ) glGenVertexArraysAPPLE( X, Y )
#define glBindVertexArray( X ) glBindVertexArrayAPPLE( X )
#define GLEW_INIT() ;
#define glutLeaveMainLoop() exit( EXIT_SUCCESS )
#else
/* Peasant Code */
#define GLEW_INIT()				\
	 glewExperimental = GL_TRUE;		\
	 glewInit();
#endif
	 
#endif
