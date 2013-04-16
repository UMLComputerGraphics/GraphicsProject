#include "mainwindow.h"
#include <QApplication>
#include "OpenGL.h"
#include "MONOLITH.hpp"

int main( int argc, char **argv ) {

  fprintf(
      stderr,
      "Error: Your project is unfinished and you're going to fail and everyone is going to laugh at you and it's a really bad day sorry\n" );
   #ifdef __APPLE__
   MONOLITH monolith(argc, argv);
   monolith.run();
   QApplication a(argc, argv);
   MainWindow w;
   w.setWindowTitle("First Graphics Demo");
   w.show();
   a.exec();
   /* PULL THE TRIGGER */
   glutMainLoop();
   

#else
  
    
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("First Graphics Demo");
    w.show();
    
    MONOLITH monolith(argc, argv);
    monolith.start();
   
    
    a.exec();
    #endif
    
    return EXIT_SUCCESS;

}
