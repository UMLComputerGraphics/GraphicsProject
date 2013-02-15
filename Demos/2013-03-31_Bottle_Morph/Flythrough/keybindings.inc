void keylift( unsigned char key, int x, int y ) {

  if (myScreen.camList.NumCameras() < 1) return;
  Camera &cam = *(myScreen.camList.Active());

  switch( key ) {
  case 'w':
    cam.Stop( Camera::Forward );
    break;
  case 's':
    cam.Stop( Camera::Backward );
    break;
  case 'a':
    cam.Stop( Camera::Left );
    break;
  case 'd':
    cam.Stop( Camera::Right );
    break;
  case 'q':
    cam.Stop( Camera::Up );
    break;
  case 'e':
    cam.Stop( Camera::Down );
    break;
  }
}

void keyboard_ctrl( int key, int x, int y ) {

  switch (key) {
    //Cycle between Active Objects ...
  case GLUT_KEY_LEFT:
    theScene.Prev();
    break;
  case GLUT_KEY_RIGHT:
    theScene.Next();
    break;

    //Change the Draw Mode ...
  case GLUT_KEY_F1:
    theScene.Active()->Mode( GL_POINTS );
    break;
  case GLUT_KEY_F2:
    theScene.Active()->Mode( GL_LINE_STRIP );
    break;
  case GLUT_KEY_F3:
    theScene.Active()->Mode( GL_TRIANGLE_STRIP );
    break;
  case GLUT_KEY_F4:
    theScene.Active()->Mode( GL_TRIANGLES );
    break;
  }

  // If there are no Cameras, don't muck around with this section.
  if (myScreen.camList.NumCameras() < 1) return;

  switch( key ) {
  case GLUT_KEY_PAGE_UP:
    myScreen.camList.Prev();
    break;

  case GLUT_KEY_PAGE_DOWN:
    myScreen.camList.Next();
    break;
  }
}

void mouse( int button, int state, int x, int y ) {

  if (myScreen.camList.NumCameras() < 1) return;

  if ( state == GLUT_DOWN ) {
    switch( button ) {
    case 3: myScreen.camList.Active()->dFOV( 1 ); break;
    case 4: myScreen.camList.Active()->dFOV( -1 ); break;
    }
  }

}


void mouseroll( int x, int y ) {

  if ((x != myScreen.MidpointX()) || (y != myScreen.MidpointY())) {
    if (myScreen.camList.NumCameras() > 0)
      myScreen.camList.Active()->roll( x - myScreen.MidpointX() );
    glutWarpPointer( myScreen.MidpointX(), myScreen.MidpointY() );
  }

}

void mouselook( int x, int y ) {

  if ((x != myScreen.MidpointX()) || (y != myScreen.MidpointY())) {
    double dx = ((double)x - myScreen.MidpointX());
    double dy = ((double)y - myScreen.MidpointY());

	if (abs(dx) > 100) dx = 0.0;
	if (abs(dy) > 100) dy = 0.0;
    
    if (myScreen.camList.NumCameras() > 0) {
      myScreen.camList.Active()->pitch( -dy );
      myScreen.camList.Active()->yaw( dx, fixed_yaw );
    }    

    glutWarpPointer( myScreen.MidpointX(), myScreen.MidpointY() );
  }
  
}
