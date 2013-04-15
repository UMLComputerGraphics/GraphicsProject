#include "mainwindow.h"
#include <QApplication>
#include <QThread>
#include "OpenGL.h"
#include "MONOLITH.hpp"
#include "partTest.hpp"

class monoThread : public QThread
{
public:
    monoThread( int x, char** y )
    {
        first = x ;
        second = y ;
    }
    void run()
    {
        MONOLITH monolith;
        monolith.run(first, second);
        glutMainLoop();
    }
private:
    int first ;
    char** second ;
} ;

int main( int argc, char **argv ) {

  fprintf(
      stderr,
      "Error: Your project is unfinished and you're going to fail and everyone is going to laugh at you and it's a really bad day sorry\n" );

   QApplication a(argc, argv);
   MainWindow w;
   w.setWindowTitle("First Graphics Demo");
   w.show();

   monoThread thread( argc, argv ) ;
   thread.start() ;

   a.exec();
   return 0;

}
