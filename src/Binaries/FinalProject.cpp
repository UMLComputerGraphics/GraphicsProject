#ifndef WITHOUT_QT
#include "mainwindow.h"
#include <QApplication>
#endif

#include "MONOLITH.hpp"
#include "OpenGL.h"


int main( int argc, char **argv ) {

  fprintf(
      stderr,
      "Error: Your project is unfinished and you're going to fail and everyone is going to laugh at you and it's a really bad day sorry\n" );

   MONOLITH monolith(argc, argv);
   
    #ifndef WITHOUT_QT

    #ifdef __APPLE__
    monolith.run();
    #else
    monolith.start();
    #endif //__APPLE__
   
    QApplication a(argc, argv);
   MainWindow w;
   w.setWindowTitle("First Graphics Demo");
   QObject::connect(w.slider, SIGNAL(valueChanged(int)), &monolith, SLOT(ParticleAdd()));
   w.show();
   a.exec();

   #ifdef __APPLE__
   /* PULL THE TRIGGER */
   glutMainLoop();
   #endif __APPLE__
    
   #else
    monolith.run();
    #endif // WITHOUT_QT

   return EXIT_SUCCESS;

}
