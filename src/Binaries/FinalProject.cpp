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
  
#ifdef WITHOUT_QT // main driver for both platforms without Qt
  monolith.run();
#else // end main drive for both platforms without Qt.

#ifdef __APPLE__ // monolith.run has to be here for OS X. Do not edit! (@franckamayou)
  monolith.run(); 
#endif // __APPLE__
  QApplication a( argc, argv );
  MainWindow w;
  w.setWindowTitle( "First Graphics Demo" );
  /*
  For connecting more variables and sliders we only need to expose the slider variables
  that already exist in maindwindow in the .h. Then do the linking functions here. The functions used
  as slots should probably have an internal variable to keep track of what the previous value was so
  that the proper modification can be made.
  */
  //QObject::connect(w.slider, SIGNAL(valueChanged(int)), &monolith, SLOT(ParticleAdd()));
  QObject::connect(&w, SIGNAL(sigChangeNumberOfParticles(int)), &monolith, SLOT(slotParticleAdd(int)));
  QObject::connect(&w, SIGNAL(sigFreezeParticles(bool)), &monolith, SLOT(slotFreezeParticles(bool)));
  QObject::connect(&w, SIGNAL(sigMorphPercentage(int)), &monolith, SLOT(slotMorphPercentage(int)));
  //QObject::connect(&monolith, SIGNAL(sigMorphPercentage(int)), &w, SLOT(sigMorphPercentageOut(int)));
  QObject::connect(&w, SIGNAL(sigEnableMorphing(bool)), &monolith, SLOT(slotEnableMorphing(bool)));
  QObject::connect(&w, SIGNAL(sigEnableRaytracing(bool)), &monolith, SLOT(slotEnableRaytracing(bool)));
  QObject::connect(&w, SIGNAL(sigEnableParticleSystem(bool)), &monolith, SLOT(slotEnableParticleSystem(bool)));
  QObject::connect(&w, SIGNAL(sigSetMaxAcceleration(int)), &monolith, SLOT(slotMaxAcceleration(int)));
  QObject::connect(&w, SIGNAL(sigSetFrictionMagnitude(int)), &monolith, SLOT(slotFriction(int)));
	QObject::connect(&w, SIGNAL(sigCameraSpeed(int)), &monolith, SLOT(slotSpeed(int)));
  w.show();

#ifndef __APPLE__ // monolith.start has to be here for Linux. 
   monolith.start();
#endif

  a.exec();
#ifdef __APPLE__ // glutMainloop has to be called here for OS X. Do not edit (@franckamayou)
  glutMainLoop();
#endif

#endif // WITHOUT_QT
  return EXIT_SUCCESS;
  
}
