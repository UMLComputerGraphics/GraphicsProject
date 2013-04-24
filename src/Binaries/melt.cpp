/**
 * @file melt.cpp
 * @author Alexander Chea
 * @date 2013-03-30
 * @brief Melting Candle Demo
 * @details FIXME: Documentation from Alexander Chea.
 */

#include "Engine.hpp"
/* System Headers */
#include <cmath>
#include <cstdio>
#include <sstream>
#include <cstdlib>
#include <time.h>

float positionx[100], positiony[100], positionz[100];
float candleheight = 50, radius = 10, grow = 0;
float angle, randomN;
int slices = 100;
int i, j;
bool melt = false;

GLuint program;
float vrandomN;

void draw( void ) {
  // the flame
  glPushMatrix();
  glTranslated( 0, candleheight + 1, 0 );
  glColor3f( 1, 0.5, 0 );
  glutSolidSphere( (grow * 2) + (grow * (rand() % 10)) / 10, 64, 64 );
  glPopMatrix();
  
  // the thread
  glPushMatrix();
  glColor3f( 1, 1, 1 );
  glLineWidth( 5 );
  glBegin( GL_LINES );
  glVertex3f( 0, 0, 0 );
  glVertex3f( 0, candleheight + 1, 0 );
  glEnd();
  glPopMatrix();
  
  // hemisphere; the top of the candle
  glPushMatrix();
  glColor3f( 1, 0, 0 );
  
  for ( j = 0; j <= slices; j++ ) {
    glBegin( GL_TRIANGLE_STRIP );
    for ( i = 0; i <= (slices / 2); i++ ) {
      glVertex3f(
          radius * cos( (float) (j + 1) / slices * M_PI / 2.0 )
          * cos( (float) 2 * i / slices * M_PI ),
          -radius * sin( (float) (j + 1) / slices * M_PI / 2.0 ) * grow + positiony[i],
          radius * cos( (float) (j + 1) / slices * M_PI / 2.0 )
          * sin( (float) 2 * i / slices * M_PI ) );
      
      glVertex3f(
          radius * cos( (float) j / slices * M_PI / 2.0 )
          * cos( (float) 2 * i / slices * M_PI ),
          -radius * sin( (float) j / slices * M_PI / 2.0 ) * grow + positiony[i],
          radius * cos( (float) j / slices * M_PI / 2.0 )
          * sin( (float) 2 * i / slices * M_PI ) );
    }
    glEnd();
  }
  glPopMatrix();
  
  // the cylinder
  glPushMatrix();
  glBegin( GL_TRIANGLE_STRIP );
  glColor3f( 1, 1, 0 );
  
  for ( i = 0; i <= (slices / 2); i++ ) {
    glVertex3f( positionx[i] * radius, 0, positionz[i] * radius );
    glVertex3f( positionx[i] * radius, positiony[i], positionz[i] * radius );
  }
  glEnd();
  glPopMatrix();
  
  // the bottom of the cylinder
  glPushMatrix();
  glBegin( GL_TRIANGLE_FAN );
  glColor3f( 1, 0, 0 );
  glVertex3f( 0, 0, 0 );
  for ( i = 0; i <= slices; ++i ) {
    angle = 2 * M_PI * i / slices;
    positionx[i] = cos( angle );
    positionz[i] = sin( angle );
    glVertex3f( positionx[i] * radius, 0, positionz[i] * radius );
  }
  glEnd();
  glPopMatrix();
  
  // floor
  glPushMatrix();
  glColor3f( 0.35, 0.29, 0 );
  glBegin( GL_POLYGON );
  glVertex3f( -50, 0, -50 );
  glVertex3f( -50, 0, 50 );
  glVertex3f( 50, 0, 50 );
  glVertex3f( 50, 0, -50 );
  glEnd();
  glPopMatrix();
}

void init( void ) {
  // Load shaders and use the resulting shader program
  program = Angel::InitShader( "shaders/vmelt.glsl", "shaders/fmelt.glsl" );
  
  vrandomN = glGetUniformLocation( program, "vrandomN" );
  
  glUseProgram( program );
}

void enable( void ) {
  glClearColor( 0.0, 0.0, 0.0, 0.0 );
  
  GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
  GLfloat mat_shininess[] = { 50.0 };
  glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular );
  glMaterialfv( GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess );
  
  glEnable( GL_DEPTH_TEST ); //enable the depth testing
  glEnable( GL_LIGHTING ); //enable the lighting
  glEnable( GL_LIGHT0 ); //enable LIGHT0, our Diffuse Light
      
  glEnable( GL_COLOR_MATERIAL );
  glShadeModel( GL_SMOOTH ); //set the shader to smooth shader
}

void camera( void ) {
  gluLookAt( -25, 50, -75, 0, 25, 0, -25, 26, -75 );
}

void display( void ) {
  glClearColor( 0.0, 0.0, 0.0, 1.0 ); //clear the screen to black
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );//clear the color buffer and the depth buffer
  glLoadIdentity();
  camera();
  enable();
  
  // the update loop
  for ( i = 0; i <= slices; ++i ) {
    angle = 2 * M_PI * i / slices;
    positionx[i] = cos( angle );
    positiony[i] = candleheight;
    positionz[i] = sin( angle );
  }
  
  draw();
  
  glutSwapBuffers(); //swap the buffers
  angle++; //increase the angle
  
  if ( melt == true ) {
    randomN = rand() % 10;
    grow += randomN / 1000;
    
    if ( grow >= 0.5 ) {
      grow = 0.5;
      if ( candleheight >= 5 ) glUniform1f( vrandomN, randomN );
      //candleheight -= randomN / 100;
    }
  }
  
}

void reshape( int w, int h ) {
  glViewport( 0, 0, (GLsizei) w, (GLsizei) h );
  glMatrixMode( GL_PROJECTION ); //set the matrix to projection
  glLoadIdentity();
  gluPerspective( 60, (GLfloat) w / (GLfloat) h, 1.0, 1000.0 );
  glMatrixMode( GL_MODELVIEW ); //set the matrix back to model
}

void customkeyboard( unsigned char key, int x, int y ) {
  switch ( key ) {
  case 27:
    exit( 0 );
    break;
  case '[':
    slices--;
    break;
  case ']':
    slices++;
    break;
  case '.':
    candleheight = 50;
    grow = 0;
    break;
  case ' ':
    if ( melt == false ) melt = true;
    else melt = false;
    break;
  }
}

int main( int argc, char **argv ) {
  Util::InitRelativePaths( argc, argv );
  glutInit( &argc, argv );
  
  glutInitDisplayMode( GLUT_DOUBLE | GLUT_DEPTH );
  glutInitWindowSize( 0, 0 );
  glutCreateWindow( "Melting Candle" );
  GLEW_INIT()
  ; // set OpenGL state and initialize shaders
  
  glutFullScreen();
  glutSetCursor( GLUT_CURSOR_NONE );
  
  init();
  glutDisplayFunc( display );
  glutIdleFunc( display );
  glutReshapeFunc( reshape );
  glutKeyboardFunc( customkeyboard );
  
  glutMainLoop();
  return 0;
}
