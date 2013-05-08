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

#include "soundHelper.hpp" // fmod headers and helpers


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
    void monolith_idle(void);
    
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

    /*
     * I have to do it this way, because the only way monolith (glut) and the mainwindow (Qt)
     * communicate is through final project. so they can only communicate value through
     * functions.
     *
     * @franckamayou
     */
    /**
     * @brief defaultNumberOfParticles setter
     * @param value
     */
    void defaultNumberOfParticles(int value);

    /**
     * @brief defaultNumberOfParticles getter
     * @return the default Number of particles
     */
    int defaultNumberOfParticles();


    RectangularMapping * _rectangularMapping;
    ScaleModel * _scaleModel;
    bool _morphMatchCalculated;
    bool _VRCameraControl;
    bool _VRMoveLeft;
    bool _VRMoveUp;
    bool _VRMoveRight;
    bool _VRMoveDown;
    bool _VRMoving;
    Angel::vec2 _VRCameraCoordinates;
    void vrlook( Camera *VRCamera, const Angel::vec2 &NewTheta,
                  const Angel::vec2 &MovementRates );

    
#ifndef WITHOUT_QT
    void setMorphPercentageCallback(boost::function<void(int)> cb);

public slots:

    void slotParticleAdd(int value);
    void slotFreezeParticles(bool isEnabled);
    void slotMorphPercentage(int value);
    void slotEnableMorphing(bool isEnabled);
    void slotEnableMorphMatching(bool isEnabled);
    void slotEnableVRCameraControl(bool isEnabled);
    void slotVRCameraCoordinates(float, float);
    void slotEnableRaytracing(bool enabled);
    void slotEnableParticleSystem(bool isEnabled);
    void slotMorphToWineBottle(void);
    void slotMorphToWhiskyBottle(void);
    //void slotParticleFieldFunction(int index); Deprecated for now.
    void slotUpdateVectorField(std::string, std::string, std::string);
    void slotMaxAcceleration(int num);
    void slotFriction(int num);
    void slotSpeed(int num);
    void slotMaxSpeed(int num);
    void slotUpdateFlameVecFunc(float[3], double, float, float);
    void slotUpdateFlameVecFunc( void );
    void slotUpdateTornadoVecFunc( float, float, float );
    void slotUpdateTornadoVecFunc( void );
    void slotCurrentView(int num) ;
    void slotSetParticleLife( float, float );
    void slotPauseMusic(bool isPaused);
    void slotMusicVolume( int );
    void slotPartColorFunc( int );

signals:
    void sigMorphPercentage(int value);
    void sigEnableParticlesMelted( bool );
    void sigUpdateUdfMessage( QString );
#endif

private:
    // Beware all those who dare venture here
    RayTracer rt;
    
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
    bool flicker;

    ParticleSystem *ps;
    bool _morphIsEnabled;

    GLint numLights;
 
    char **_argv;
    int _argc;
    
    int _defaultNumberOfParticles ;

    boost::thread zipo;

    boost::function<void(int)> _percentageCallback;


    /* if this is misplaced, don't hate, I don't understand what's what */

    FMOD::System *fSystem;
    FMOD::Sound  *foreverEndless, *fire;
    FMOD::Channel *radio, *flame;
    
    Light *_l1;
    Light *_l2;

};

#endif
