#include "mainwindow.h"
#include <QApplication>
#include "OpenGL.h"
#include "MONOLITH.hpp"

int main( int argc, char **argv ) {

  fprintf(
      stderr,
      "Error: Your project is unfinished and you're going to fail and everyone is going to laugh at you and it's a really bad day sorry\n" );

   MONOLITH monolith;
   monolith.run(argc, argv);
   QApplication a(argc, argv);
   MainWindow w;
   w.setWindowTitle("First Graphics Demo");
   w.show();
   a.exec();
   /* PULL THE TRIGGER */
   glutMainLoop();
   return 1;

}
