/**
 * @file FinalProject.cpp
 * @authors Graphics II Class, Spring 2013
 * @date 2013-04-23
 * @brief TODO
 * @details TODO
 */

#ifndef WITHOUT_QT
#include "mainwindow.h"
#include <QApplication>
#endif

#include "MONOLITH.hpp"
#include "OpenGL.h"

int main( int argc, char **argv ) {
  
  gprint( PRINT_INFO, "Error: Your project is unfinished and you're "
          "going to fail and everyone is going to laugh at you and "
          "it's a really bad day sorry\n" );
  
  MONOLITH monolith( argc, argv );
  
#ifdef WITHOUT_QT
  monolith.run();
#else
  QApplication a( argc, argv );
  MainWindow w;
  w.setWindowTitle( "First Graphics Demo" );
  /*
  For connecting more variables and sliders we only need to expose the slider variables
  that already exist in maindwindow in the .h. Then do the linking functions here. The functions used
  as slots should probably have an internal variable to keep track of what the previous value was so
  that the proper modification can be made.
  */
  QObject::connect(w.slider, SIGNAL(valueChanged(int)), &monolith, SLOT(ParticleAdd()));
  w.show();
  
#ifndef __APPLE__
  monolith.start();
#endif
  a.exec();
#ifdef __APPLE__
  monolith.run();
#endif
#endif
  return EXIT_SUCCESS;
  
}
