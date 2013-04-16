#include "mainwindow.h"
#include <QApplication>
#include "OpenGL.h"
#include "MONOLITH.hpp"
#include "partTest.hpp"
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
int main( int argc, char **argv ) {

  fprintf(
      stderr,
      "Error: Your project is unfinished and you're going to fail and everyone is going to laugh at you and it's a really bad day sorry\n" );

   MONOLITH monolith;
   monolith.run(argc, argv);
//   partTest partTest;
//   partTest.run(argc, argv);

   QApplication a(argc, argv);
   MainWindow w;
   w.setWindowTitle("First Graphics Demo");
   w.show();

   //We can't have 2 simultaneous mainloops on the same thread, so let's make QT run the lobotomized freeglut mainloop for us.
   glutMainLoopInitialize();

   //PULL THE TRIGGER
   a.exec();
   glutMainLoopDeinitialize();
   return 1;

}
