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
#include "InitShader.hpp"
#include "glut_callbacks.h"
#include "ObjLoader.hpp"

/* particle system goodness */
#include "ParticleFieldFunctions.hpp"
#include "ColorFunctions.hpp"

/*Morphing Items */
#include "modelFunctions.hpp"
#include "nearestNeighbor.hpp"
#include "bipartiteGraph.hpp"
#include "scanMatching.hpp"
#include "scaleModel.hpp"

#include <boost/bind.hpp>

#include "RayTracer.h"

#ifndef WITHOUT_QT
/* Qt */
#include <QObject>
//if this is included, then qopengl is linked against the qt open gl guts... which breaks everything.
//    need to have the window's class import monolith.hpp and call non-QTGui-referencing set functions from the QTGUI handlers
//#include <QtGui>  


#ifdef __APPLE__
class MONOLITH: public QObject {
#else
#include <QThread>
class MONOLITH: public QThread {
#endif // __APPLE__
    Q_OBJECT
#else
class MONOLITH {
#endif // WITHOUT_QT
public:
    
    
    // Constructor and destructor for MONOTLITH
    MONOLITH( int argc, char* argv[] );
    
    ~MONOLITH();

//    /**
//     * Initialization: load and compile shaders, initialize camera(s), load models.
//     */
//
//    void init(void);
    
    /**
     * Cleans up our scene graph.
     */
    void cleanup(void);
    
    
    /**
     * Apply animations and whatever else your heart desires.
     */
    static void monolith_idle(void);
    
    /**
     * This will initialize and run MONOLITH
     */
    void run();

    /*
     * to win it
     */
    void init();
    
    /**
     * SHOW ENGINE WHERE OUR GOAT IS //ralphy may allusion
     */
    void raytraceStatusChanged(bool newstatus);

    
#ifndef WITHOUT_QT
public slots:

    void slotParticleAdd(int value);
    void slotFreezeParticles(bool isEnabled);
    void slotMorphPercentage(int value);
    void slotEnableMorphing(bool isEnabled);
    void slotEnableRaytracing(bool enabled);
    void slotEnableParticleSystem(bool isEnabled);
    void slotMorphToWineBottle(void);
    void slotMorphToWhiskyBottle(void);
    void slotParticleFieldFunction(int index);

signals:
    void sigMorphPercentage(int value);
#endif

private:
    
    /**
     * A simple animation callback.
     * Rotates the object about its Y axis,
     * as it orbits the object around a point about the Y axis.
     * @param obj The object to animate.
     */
    static void simpleRotateAnim( TransCache &obj );
    static void candleMeltAnim( TransCache &obj );
    static void candleTopMeltDown( TransCache &obj );
    void aRomanticEvening();

    void display();

    GLuint shader[4];
    
    Scene *rootScene;
    Screen *primScreen;
    Object *bottle;
    
    bool extinguish;

    ParticleSystem *ps;
    bool _morphIsEnabled;

    GLfloat *lightPositions;
    GLfloat *lightAmbient;
    GLfloat *lightDiffuse;
    GLfloat *lightSpecular;
    GLint numLights;
 
    char **_argv;
    int _argc;
    
    boost::thread zipo;

};

#endif
