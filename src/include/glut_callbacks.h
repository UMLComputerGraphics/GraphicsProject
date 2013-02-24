#ifndef __GLUT_CALLBACKS_H
#define __GLUT_CALLBACKS_H

void keylift( unsigned char key, int x, int y );
void keyboard( unsigned char key, int x, int y );
void keyboard_ctrl( int key, int x, int y );
void mouse( int button, int state, int x, int y );
void mouseroll( int x, int y );
void mouselook( int x, int y );
void resizeEvent( int width, int height );

#endif
