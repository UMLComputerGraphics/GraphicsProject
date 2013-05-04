


#include <cv.h>
#include <highgui.h>

#include <stdio.h>
#include <string.h>
#include <GL/glut.h>
#include <GL/glaux.h>

#include "display.h"

#define VIEWPORT_WIDTH              640
#define VIEWPORT_HEIGHT             480
#define KEY_ESCAPE                  27


CvCapture* g_Capture;
GLint g_hWindow;

GLvoid InitGL();
GLvoid OnDisplay();
GLvoid OnReshape(GLint w, GLint h);
GLvoid ReSizeGLScene(GLsizei width, GLsizei height)	;
void keyboard (unsigned char key, int x, int y);
void OnIdle();
float xm=0,ym=0,zm=-30;

void keyboard_s (int key, int x, int y);

void captureIMG()
{
    // Capture next frame
	IplImage *image = cvQueryFrame(g_Capture);

	// Convert to RGB
	cvCvtColor(image, image, CV_BGR2RGB);

	// Create Texture
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, image->width, image->height, GL_RGB, GL_UNSIGNED_BYTE, image->imageData);
    //glTexImage2D(
    glutPostRedisplay();
}


int GLmain(int argc, char* argv[])
{

	// Create GLUT Window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(VIEWPORT_WIDTH, VIEWPORT_HEIGHT);

	g_hWindow = glutCreateWindow("Video Texture");

	// Create OpenCV camera capture
	// If multiple cameras are installed, please choose correct index
	///g_Capture = cvCreateCameraCapture(0);
	//assert(g_Capture);

	// Initialize OpenGL
	InitGL();

	glutMainLoop();

	return 0;
}

GLvoid InitGL()
{  

    
	glutDisplayFunc(OnDisplay);
	glutReshapeFunc(ReSizeGLScene);
	glutKeyboardFunc(keyboard);
    glutSpecialFunc(keyboard_s);

	//glutIdleFunc(OnIdle);
    
    glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
	glEnable(GL_TEXTURE_2D);
	glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);

    

	glClearColor (0.0, 0.0, 0.0, 0.0);


}

GLvoid OnDisplay(void)
{
    //captureIMG();
    // Update View port
    IplImage *image2 = cvQueryFrame(captureL);
    IplImage *image=cvCloneImage(image2);
    if(image)
    {cvCvtColor(image, image, CV_BGR2RGB);

	// Create Texture
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, image->width, image->height, GL_RGB, GL_UNSIGNED_BYTE, image->imageData);

    cvCvtColor(image, image, CV_RGB2BGR);
    }
	cvReleaseImage(&image);

	glutPostRedisplay();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D);

    
	/*// Set Projection Matrix
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, VIEWPORT_WIDTH, VIEWPORT_HEIGHT, 0);
    */

	// Switch to Model View Matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

    glTranslatef(0,0,-100);
    //glutSolidCube(5);
    
	// Draw a textured quad
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-55,-45,0);
	glTexCoord2f(1, 0.0f); glVertex3f(55,-45,0);
	glTexCoord2f(1, 1); glVertex3f(55,45,0);
	glTexCoord2f(0.0f, 1); glVertex3f(-55,45,0);
	glEnd();


    glLoadIdentity();


    glTranslatef(xm,ym,zm);
    
    float l=5;
    /*glBegin(GL_QUADS);									// Draw A Quad
		glVertex3f( l, l,-l);					// Top Right Of The Quad (Top)
		glVertex3f(-l, l,-l);					// Top Left Of The Quad (Top)
		glVertex3f(-l, l, l);					// Bottom Left Of The Quad (Top)
		glVertex3f( l, l, l);					// Bottom Right Of The Quad (Top)
		glColor3f(0.0f,1.0f,0.0f);						// Set The Color To Orange
		glVertex3f( l,-l, l);					// Top Right Of The Quad (Bottom)
		glVertex3f(-l,-l, l);					// Top Left Of The Quad (Bottom)
		glVertex3f(-l,-l,-l);					// Bottom Left Of The Quad (Bottom)
		glVertex3f( l,-l,-l);					// Bottom Right Of The Quad (Bottom)
		glColor3f(0.0f,1.0f,0.0f);						// Set The Color To Orange
		glVertex3f( l, l, l);					// Top Right Of The Quad (Front)
		glVertex3f(-l, l, l);					// Top Left Of The Quad (Front)
		glVertex3f(-l,-l, l);					// Bottom Left Of The Quad (Front)
		glVertex3f( l,-l, l);					// Bottom Right Of The Quad (Front)
		glColor3f(0.0f,1.0f,0.0f);						// Set The Color To Orange
		glVertex3f( l,-l,-l);					// Top Right Of The Quad (Back)
		glVertex3f(-l,-l,-l);					// Top Left Of The Quad (Back)
		glVertex3f(-l, l,-l);					// Bottom Left Of The Quad (Back)
		glVertex3f( l, l,-l);					// Bottom Right Of The Quad (Back)
		glColor3f(0.0f,1.0f,0.0f);						// Set The Color To Orange
		glVertex3f(-l, l, l);					// Top Right Of The Quad (Left)
		glVertex3f(-l, l,-l);					// Top Left Of The Quad (Left)
		glVertex3f(-l,-l,-l);					// Bottom Left Of The Quad (Left)
		glVertex3f(-l,-l, l);					// Bottom Right Of The Quad (Left)
		glColor3f(0.0f,1.0f,0.0f);						// Set The Color To Orange
		glVertex3f( l, l,-l);					// Top Right Of The Quad (Right)
		glVertex3f( l, l, l);					// Top Left Of The Quad (Right)
		glVertex3f( l,-l, l);					// Bottom Left Of The Quad (Right)
		glVertex3f( l,-l,-l);	
	glEnd();											// Done Drawing The Quad
glColor3f(1.0f,1.0f,1.0f);*/						// Set The Color To Violet

    auxSolidTeapot(10);
	glFlush();
	glutSwapBuffers();


}


GLvoid ReSizeGLScene(GLsizei width, GLsizei height)		// Resize And Initialize The GL Window
{
	if (height==0)										// Prevent A Divide By Zero By
	{
		height=1;										// Making Height Equal One
	}

	glViewport(0,0,width,height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
}

void OnIdle()
{
	// Capture next frame
	//cvQueryFrame(captureL);

    
    
}


void keyboard (unsigned char key, int x, int y)
{
        
    switch (key)
    {
  
       

		 case 'o': case 'O':
            ym+=5;
        break;
		 case 'p': case 'P':
            ym-=5;
        break;
    }
}



/**********************************************************
 *
 * SUBROUTINE keyboard(int,int,int)
 *
 * Used to handle the keyboard input (not ASCII Characters)
 * 
 *********************************************************/

void keyboard_s (int key, int x, int y)
{
    
    switch (key)
    {
        case GLUT_KEY_UP:
            zm-=5;
        break;
        case GLUT_KEY_DOWN:
            zm+=5;
        break;
        case GLUT_KEY_LEFT:
            xm-=5;
        break;
        case GLUT_KEY_RIGHT:
            xm+=5;
        break;
        
    }
}

