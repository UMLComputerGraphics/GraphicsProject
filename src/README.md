Laziness
==

- This README is hideously out of date.


- This is (mostly) README file for John Huston's, Nick St.Pierre's and 
  Chris Compton's Graphics I project for Fall 2012. It needs to
  be updated for Spring 2013!

License
==
TBD.

Authors
==

- John Fallon -- Graphical User Interface

- Greg Giannone -- Modelling, Shaders, Bugfixes

- John Huston -- Engine, Bugfixes, Wii Support, Integration

- Franck Kamayou -- Graphical User Interface

- Zach Maybury -- Model Morphing Logistics

- Eric McCann -- Integration, Logistics, Bugfixes, 
                 Raytracing, Lighting, Kinect Support, Git Guru

- Hoanh Nguyen -- Raytracing, Lighting

- Nick St.Pierre -- Particle Systems, Math, Sound

- Luan Tran -- Graphical User Interface (Linux Support)

- Nick VerVoort -- Particle Systems, Modelling


Third-Party Libraries
==

- OpenGL, GLEW, Freeglut, and the usual suspects.

- Libmagick++: Image loader, utilized for converting JPEG tetxures to RGBA.

- LibWiiC/++: Wii Remote driver library, see Gabrielle Randelli below.

- exprtk: C++ Mathematical Expression Library, Arash Parttow, http://www.partow.net/programming/exprtk/index.html

- fModEX: Sound library, http://www.fmod.org/

- QT: Multi-platform GUI library


Credits and Acknowledgements
==

  - Etay Mieri, olgdev.atspace.co.uk:
    	 Invaluable tutorials on OpenGL, and 99.9% of our Texture class.

  - Daniel Beard, Paul Boxley and Paul Martz:
    	   Information and code for generating random terrain.
	   The code we eventually implemented is heavily modified
	   from Daniel Beard's implementation, using Boxley and Martz's 
	   writeups as guidelines.

  - Ed Angel:
      Author of "Interactive Computer Graphics: A Top-Down Approach,"
      Provided the basis of our executable's main skeleton, 
      and 99.9% of our vec and mat classes, from his book's code.

  - Gabriele Randelli:
      Lead developer of the WiiC/PP library, which itself has been
      developed out of the (now defunct) WiiUse library, authored by
      Michael Laforest.

  - Dan Lecocq:
      For a really great blog post on how to draw a mesh with
      GL_TRIANGLE_STRIPs. http://dan.lecocq.us/wordpress/2009/12/25/triangle-strip-for-grids-a-construction/

  - Simon Murray:
      Founder of www.goodtextures.com from which we obtained
      all of our terrain textures.


Outdated HSC Readme
===

- This is our Graphics I final project for Prof. Grinstein,
  at UMass Lowell, Fall 2012.

- It has been written ground-up without utilizing the fixed-pipeline
  API of OpenGL. 

- It implements a fully Object-Oriented scene graph, independent and
  arbitrary cameras (realized via splitscreen), and multiple methods
  of input.

- The project features prominently random terrain generation. This is
  performed using the Diamond-Square algorithm; see below for additional
  reading on the technique.

- This project utilizes a lot of code snippets, libraries and ideas from
  a variety of authors. They are credited above!

- To build the project, pushd Flythrough && make;
  To run, pushd Flythrough && ./fly;

- Controls (as of Mon 03 Dec 2012)
  w,a,s,d: Forward, back, strafe left and right (As in most commercial FPS games)
  q,e: Fly up, Fly down.
  mouse: Look around.
  click and move mouse: Rotate left or right.
  right-click: Open a menu that allows you to toggle mouselook between fixed-y and free-y rotations.

  z,x,c,v,b: Various perspective modes;
  	     z: Perspective
	     x: Ortho2D
	     b: Identity

  l: Generate new terrain.

  +,-: Add or remove cameras.

  left,right: Cycle between the active scene objects.

  F1,F2,F3,F4: Change the rendering mode of the active scene object.
  	       F1: GL_POINTS
	       F2: GL_LINE_LOOP
	       F3: GL_TRIANGLE_STRIP
	       F4: GL_TRIANGLES

  Wiimote Minus Button: Enable/Calibrate Gyroscope
  Keyboard '~':         Calibrate Gyroscope, Reset Camera.

  Wiimote Balance Board Button: Begin Tare (Calibrate to Zero.)

- wiic/pp library utilizes the LGPL license,
  The source code is included in our source tree here for convenience,
  with minor alterations.
