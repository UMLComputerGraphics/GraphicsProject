/**
 @file partTest.hpp
 @author Nicholas St.Pierre
 @authors John Huston, Nicholas VerVoort, Chris Compton
 @date 2013-02-23
 @details This is a tech demo for a particle system
 */

#ifndef _partTest_hpp
#define _partTest_hpp

#include "MONOLITH.hpp"
#include "ParticleSystem.hpp"
#include <QObject>
#include <QtGui>

#endif /* defined(____partTest__) */

class partTest: public MONOLITH {

public:
    
    partTest();
    
    ~partTest();
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
    
public slots:

signals:
    
private:
    const static int _numberOfParticles = 4000;
    static ParticleSystem *_funInTheSin;
    static double _theta;
    static GLuint _thetaLoc;
    
};