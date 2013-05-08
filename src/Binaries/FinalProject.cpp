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
#include <boost/bind.hpp>
#include <boost/function.hpp>

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
  w.setWindowTitle( "GUI Interface - Graphics II" );
  w.setGeometry(700, 0, w.width(), w.height());

  // This is where the connection is for the default number of particles.
  monolith.defaultNumberOfParticles(w.getDefaultNumOfParticles());
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
  
  //Watch and learn
  monolith.setMorphPercentageCallback((boost::function<void(int)>)boost::bind(&MainWindow::setMorphPercentageOut, &w, _1));

  QObject::connect(&w, SIGNAL(sigEnableMorphing(bool)), &monolith, SLOT(slotEnableMorphing(bool)));
  QObject::connect(&w, SIGNAL(sigEnableRaytracing(bool)), &monolith, SLOT(slotEnableRaytracing(bool)));
  QObject::connect(&w, SIGNAL(sigEnableParticleSystem(bool)), &monolith, SLOT(slotEnableParticleSystem(bool)));
  //QObject::connect(&w, SIGNAL(sigParticleFieldFunction(int)), &monolith, SLOT(slotParticleFieldFunction(int)));
  QObject::connect(&w, SIGNAL(sigEnableMorphMatching(bool)), &monolith, SLOT(slotEnableMorphMatching(bool)));
  QObject::connect(&w, SIGNAL(sigUpdateVectorField(std::string*)), &monolith, SLOT(slotUpdateVectorField(std::string*)));
  QObject::connect(&w, SIGNAL(sigSetMaxAcceleration(int)), &monolith, SLOT(slotMaxAcceleration(int)));
  QObject::connect(&w, SIGNAL(sigSetFrictionMagnitude(int)), &monolith, SLOT(slotFriction(int)));
  QObject::connect(&w, SIGNAL(sigCameraSpeed(int)), &monolith, SLOT(slotSpeed(int)));
  QObject::connect(&w, SIGNAL(sigSetMaxSpeed(int)), &monolith, SLOT(slotMaxSpeed(int)));
  QObject::connect(&w, SIGNAL(sigFlameVecParams(float[3],double,float,float)),
                   &monolith, SLOT(slotUpdateFlameVecFunc(float[3], double, float, float)));
  QObject::connect(&w, SIGNAL(sigChangeCurrentView(int)), &monolith, SLOT(slotCurrentView(int))) ;
  QObject::connect(&w, SIGNAL(sigFlameVecParams()), &monolith, SLOT(slotUpdateFlameVecFunc()));
  QObject::connect(&w, SIGNAL(sigTornadoVecParams(float,float,float)),
                   &monolith, SLOT(slotUpdateTornadoVecFunc(float, float, float)));
  QObject::connect(&w, SIGNAL(sigTornadoVecParams()), &monolith, SLOT(slotUpdateTornadoVecFunc()));
  QObject::connect(&w, SIGNAL(sigMorphToWhiskyBottle()),&monolith,SLOT(slotMorphToWhiskyBottle()));
  QObject::connect(&w, SIGNAL(sigMorphToWineBottle()), &monolith, SLOT(slotMorphToWineBottle()));
  QObject::connect(&w, SIGNAL(sigSetParticleLife(float,float)), &monolith, SLOT(slotSetParticleLife(float, float)));
  QObject::connect(&w, SIGNAL(sigMusicVolume(int)), &monolith, SLOT(slotMusicVolume( int )));
  QObject::connect(&monolith, SIGNAL(sigEnableParticlesMelted(bool)), &w, SLOT(on_particleSystemEnabler_toggled_melted(bool)));
  QObject::connect(&monolith, SIGNAL(sigEnableParticlesMelted(bool)), &w, SLOT(slotEnableParticleSystem(bool)));
  QObject::connect(&w, SIGNAL(sigUserDefVecParams(std::string, std::string, std::string)),
                   &monolith, SLOT(slotUpdateVectorField(std::string, std::string, std::string)));
  //  QObject::connect(&monolith, SIGNAL(sigEnableParticles(bool)), &w, SLOT(on_particleSystemEnabler_toggled(bool)));
  QObject::connect(&w, SIGNAL(sigEnableVRCameraControl(bool)), &monolith, SLOT(slotEnableVRCameraControl(bool)));
  QObject::connect(&w, SIGNAL(sigVRCameraCoordinates(float,float)), &monolith, SLOT(slotVRCameraCoordinates(float,float)));
  QObject::connect(&w, SIGNAL(sigPartColorFunc(int)), &monolith, SLOT(slotPartColorFunc(int)));
  QObject::connect(&monolith, SIGNAL(sigUpdateUdfMessage(QString)), &w, SLOT(updateUdfMessage(QString)));

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
