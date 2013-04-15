/**
 @file MONOLITH.hpp
 @author UMLComputerGraphics <https://github.com/UMLComputerGraphics>
 @date 2013-03-29
 @brief This is a monolith of every component.
 @details Final project rough draft.
 Original engine based on Ed Angel's book code.
 **/

#ifndef _MONOLITH_hpp
#define _MONOLITH_hpp

/* OpenGL and "The Engine" */
#include "Engine.hpp"
/* Utilities and Common */
#include "model.hpp"
#include "modelFunctions.hpp"
#include "InitShader.hpp"
#include "glut_callbacks.h"
#include "ObjLoader.hpp"

#ifndef WITHOUT_QT
/* Qt */
#include "OpenGL.h"
#include <QObject>
#endif

/* System Headers */
#include <cmath>
#include <cstdio>
#include <sstream>
#include <cstdlib>
#include <time.h>

#ifndef WITHOUT_QT
#include <qtimer.h>
#endif

class MONOLITH
#ifndef WITHOUT_QT
    : public QObject
{
    Q_OBJECT
#else
{
#endif

public:
    
    
    // Constructor and destructor for MONOTLITH
    MONOLITH(
#ifndef WITHOUT_QT
        QObject *parent = 0
#endif
        );
    
    ~MONOLITH();

    /**
     * Initialization: load and compile shaders, initialize camera(s), load models.
     */

    void init(void);
    
    /**
     * Cleans up our scene graph.
     */
    void cleanup(void);
    
    
    /**
     * Apply animations and whatever else your heart desires.
     */
    static void idle(void);
    
    /**
     * This will initialize and run MONOLITH
     */
    void run(int &argc, char* argv[]);
    
    
#ifndef WITHOUT_QT
public slots:
    
    void doMainLoop();
signals:
#endif
    
private:

    //this indicates to the main loop hack that it should stop trying to loop using QTimer
#ifndef WITHOUT_QT
    bool isQtWindowDead;
#endif

    /**
     * A simple animation callback.
     * Rotates the object about its Y axis,
     * as it orbits the object around a point about the Y axis.
     * @param obj The object to animate.
     */
    static void simpleRotateAnim( TransCache &obj );
};


#endif
