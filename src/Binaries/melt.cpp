#include "Angel.h"

float xpos = 0, ypos = 0, zpos = 0;
float xrot = 0, yrot = 0, angle = 0.0;
float lastx, lasty;
float orb = 0;

int slices = 100;
float candleheight = 50 ;
float positionx[1000], positiony[1000], positionz[1000];
float crater = candleheight;
float radius = 10;
int i, j;
float grow = 0;
bool melt = FALSE;

void draw (void)
{
	// the update loop
	for (i = 0; i <= slices ; ++i)
	{
		angle = 2 * M_PI * i / slices;
		positionx[i] = cos(angle);
		positiony[i] = candleheight;
		positionz[i] = sin(angle);
	}

	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 50.0 };
	
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);

	// the light bulb
	glPushMatrix();
	glColor3f(1, 1, 1);
	glTranslated(50 * cos(orb), 25, 50 * (sin(orb))); // Sets the sphere relative to the variable orbx so that it orbits the origin
	glutSolidSphere (2, 64, 64);
	glPopMatrix();

	// the flame
	glPushMatrix();
	glTranslated(0, candleheight + 1, 0); // Sets the sphere relative to the variable orbx so that it orbits the origin
	glColor3f(1, 0.5, 0);
	glutSolidSphere (1, 64, 64);
	glPopMatrix();
	
	// the thread
	glPushMatrix();
	glColor3f(1, 1, 1);
	glLineWidth(5); 
	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(0, candleheight + 1, 0);
	glEnd();
	glPopMatrix();


	// the bottom of the cylinder
	glPushMatrix();
	glBegin(GL_TRIANGLE_FAN);
	glColor3f(1, 0, 0);
	glVertex3f(0, 0, 0);
	for (i = 0; i <= slices ; ++i)
	{
		angle = 2 * M_PI * i / slices;
		positionx[i] = cos(angle);
		positionz[i] = sin(angle);
		glVertex3f(positionx[i] * radius, 0, positionz[i] * radius);
	}
	glEnd();
	glPopMatrix();
	
	// the cylinder
	glPushMatrix();
	glBegin(GL_TRIANGLE_STRIP);
	glColor3f(1, 1, 0);
	for (i = 0; i <= (slices / 1.5); i++)
	{
		glVertex3f(positionx[i] * radius,	0				, positionz[i] * radius);
		glVertex3f(positionx[i] * radius,	positiony[i]	, positionz[i] * radius);
	}
	glEnd();
	glPopMatrix();
	
	// floor
	glPushMatrix();
	glColor3f(0.35, 0.29, 0);
	glBegin(GL_POLYGON);
		glVertex3f(-50, 0, -50);
		glVertex3f(-50, 0,  50);
		glVertex3f( 50, 0,  50);
		glVertex3f( 50, 0, -50);
	glEnd();
	glPopMatrix();

	// hemisphere; the top of the candle
	glPushMatrix();
	glColor3f(1, 0, 0);
	for(j = 0; j <= slices; j++)
	{
		glBegin(GL_TRIANGLE_STRIP);
		for(i = 0; i <= (slices / 2); i++)
		{
			glVertex3f(  radius * cos((float) (j + 1) / slices * M_PI / 2.0 ) * cos((float) 2 * i / slices * M_PI),
						-radius * sin((float) (j + 1) / slices * M_PI / 2.0 ) * grow +  positiony[i],
						 radius * cos((float) (j + 1) / slices * M_PI / 2.0 ) * sin((float) 2 * i / slices * M_PI) );

			glVertex3f(  radius * cos((float) j / slices * M_PI / 2.0 ) * cos((float) 2 * i / slices * M_PI),
						-radius * sin((float) j / slices * M_PI / 2.0 ) * grow + positiony[i],
						 radius * cos((float) j / slices * M_PI / 2.0 ) * sin((float) 2 * i / slices * M_PI) );         
		}
		glEnd();
	}
	glPopMatrix();
}

void enable (void)
{
	glClearColor (0.0, 0.0, 0.0, 0.0);
	GLfloat light_position[] = {50 * cos(orb), 25, 50 * sin(orb), 1.0};
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glEnable (GL_DEPTH_TEST); //enable the depth testing
    glEnable (GL_LIGHTING); //enable the lighting
    glEnable (GL_LIGHT0); //enable LIGHT0, our Diffuse Light
	glEnable (GL_COLOR_MATERIAL);
    glShadeModel (GL_SMOOTH); //set the shader to smooth shader
}

void camera (void)
{
	/*
    glRotatef(xrot,1.0,0.0,0.0);  //rotate our camera on the x-axis (left and right) xrot
    glRotatef(yrot,0.0,1.0,0.0);  //rotate our camera on the y-axis (up and down) yrot
    glTranslated(-xpos, -ypos - 25, -zpos); //translate the screen to the position of our camera
	*/
	gluLookAt(-25, candleheight + 25, -75, 0, candleheight, 0, -25, candleheight + 26, -75);
}

void display (void)
{
	glClearColor (0.0,0.0,0.0,1.0); //clear the screen to black
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear the color buffer and the depth buffer
    glLoadIdentity();  
    camera();
    enable();
    draw();
    glutSwapBuffers(); //swap the buffers
    angle++; //increase the angle
	// The following code allows the spheres to orbit the origin
	orb += 0.01;
    if ( orb > 360.0 ) orb = 0;
}

void reshape (int w, int h)
{
    glViewport (0, 0, (GLsizei)w, (GLsizei)h); //set the viewport to the current window specifications
    glMatrixMode (GL_PROJECTION); //set the matrix to projection
    glLoadIdentity ();
    gluPerspective (60, (GLfloat)w / (GLfloat)h, 1.0, 1000.0); //set the perspective (angle of sight, width, height, , depth)
    glMatrixMode (GL_MODELVIEW); //set the matrix back to model
}

void keyboard (unsigned char key, int x, int y)
{
	float xrotrad, yrotrad;

    if (key == 'q')
		xrot = 0;

    if (key == 'z')
		xrot += 15;

	if (key == 'e')
		yrot = 0;

    if (key == 'c')
		yrot += 15;

    if (key == 'w')
    {
		yrotrad = (yrot / 180 * M_PI);
		xrotrad = (xrot / 180 * M_PI); 
		xpos += float(sin(yrotrad));
		zpos -= float(cos(yrotrad));
		ypos -= float(sin(xrotrad));
    }

    if (key == 's')
    {
		yrotrad = (yrot / 180 * M_PI);
		xrotrad = (xrot / 180 * M_PI); 
		xpos -= float(sin(yrotrad));
		zpos += float(cos(yrotrad));
		ypos += float(sin(xrotrad));
    }

    if (key == 'd')
    {
		yrotrad = (yrot / 180 * M_PI);
		xpos += float(cos(yrotrad));
		zpos += float(sin(yrotrad));
    }

    if (key == 'a')
    {
		yrotrad = (yrot / 180 * M_PI);
		xpos -= float(cos(yrotrad));
		zpos -= float(sin(yrotrad));
    }

	// Esc key
    if (key == 27)
		exit(0);

	if (key == '.')
	{
		candleheight = 50;
		crater = candleheight;
	}

	if (key == '[')
		slices--;

	if (key == ']')
		slices++;

	if (key == '.')
	{
		candleheight = 50;
		crater = candleheight;
		grow = 0;
	}

	if (key == ' ')
	{
		float random;
		random = rand() % 10;
		random = random / 10;
		grow += random / 10;

		if (grow >= 0.5)
		{
			grow = 0.5;

			if ( candleheight >= 5)
				candleheight -= random;
		}
	}
}

void mouseMovement(int x, int y)
{
    int diffx, diffy;
	
	diffx = x - lastx; //check the difference between the current x and the last x position
    diffy = y - lasty; //check the difference between the current y and the last y position
    
	lastx = x; //set lastx to the current x position
    lasty = y; //set lasty to the current y position

    xrot += (float) diffy; //set the xrot to xrot with the addition of the difference in the y position

    yrot += (float) diffx; //set the xrot to yrot with the addition of the difference in the x position
}

int main (int argc, char **argv)
{
    glutInit (&argc, argv);
	
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH); 
    glutInitWindowSize (1600, 900);
    glutInitWindowPosition (100, 100);
	glutCreateWindow ("Melting Candle");

	/*
	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
	glutGameModeString( "1280x720:32@60" );
    glutEnterGameMode();
	*/

	glutFullScreen();
	glutSetCursor(GLUT_CURSOR_NONE);

    glutDisplayFunc (display); 
    glutIdleFunc (display); 
    glutReshapeFunc (reshape);
    glutPassiveMotionFunc(mouseMovement); //check for mouse movement
    glutKeyboardFunc (keyboard);

    glutMainLoop (); 
    return 0;
}