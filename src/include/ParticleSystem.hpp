/**
 * @file ParticleSystem.hpp
 * @author Chris Compton <christopher_compton@student.uml.edu>
 * @date 2013-02-24
 * @brief TODO: Documentation from CC/NVV
 * @details TODO: Documentation from CC/NVV
 */

#ifndef __PARTICLESYSTEM_H
#define __PARTICLESYSTEM_H

#include <string>
#include <vector>
#include "mat.hpp"
#include "Object.hpp"
#include "Particle.hpp"
#include "Camera.hpp"
#include "Engine.hpp"
#include "vec.hpp"
#include "TransCache.hpp"
#include "ParticleFieldFunctions.hpp"
#include "UserVectorField.hpp"

using Angel::vec2;
using Angel::vec3;
using Angel::vec4;
using Angel::mat4;
using std::string;
using std::vector;


typedef Particle* ParticleP;

typedef enum { PS_NONE, PS_LINE, PS_CIRCLE, PS_SPHERE, PS_HEMI_D, PS_HEMI_U, PS_CUBE, PS_EXTERNAL } PS_SHAPE;

class ParticleSystem : public Object {
  
 public:

  // constructor/destructor  
  ParticleSystem( int particleAmt, const std::string &name, GLuint shader );
  ~ParticleSystem( void );

  // this will fill the system with newborn particles
  void  fillSystemWithParticles(void);

  // Getters and Setters
  float getLifespan( void ) const ;
  float getMaxLife( void ) const ;
  float getMinLife( void ) const ;
  int   getNumParticles( void ) const ;
  int   getNumParticlesActual( void ) const ;
  int   getNumParticlesVisible( void ) const ;

  void  setNumParticles( int newNumParticles );
  void  setSlaughterHeight( float );
  void  setLifespan( float minLifespan, float maxLifespan );
  void  setEmitterRadius(float);

  virtual void draw( bool doDraw = true );

  void  setVectorField(vec3 (*vectorFieldFunc)(vec4, Parameters* ) );
  void  setColorFunc(vec4 (*colorFunc)(float, vec4) );
  
  static float rangeRandom( float min, float max );

  // methods to pause/unpause the system
  void pauseTheSystem(void);
  void unpauseTheSystem(void);
  void togglePause(void);
  void setPause(bool);
  void setEnableTheSystem( bool );
  bool getEnableTheSystem( void );

  //overrides for Engine functionality
  virtual void buffer( GLenum usage = GL_DYNAMIC_DRAW );
  virtual void sceneToRaytracer( RayTracer &rt );
  virtual void bufferToRaytracer( RayTracer &rt );

  //TransCache _emitterTrans ; // this transcache represnts the position of the emitter

  PS_SHAPE getEmitterShape(void) const ; 
  PS_SHAPE getSystemShape(void) const ; 

  void setEmitterShape(PS_SHAPE); 
  void setSystemShape(PS_SHAPE); 

  void setFuncParams(Parameters*);

  void setRespawnFlag( bool flag );

  Parameters* getFuncParams();


  void setNumToAddEachFrame(unsigned);

  void switchUVF( UserVectorField *newUVF );
  UserVectorField* uvf();


 private:  


  /* private methods*/

  /* The Do-All function.  Will do everything needed in order
   to have the particles behave according to our specification
   on each call to Draw(). That's the idea, anyway. */
  void  update();
  void  updateNumParticles( int );

  float generateLifespan();

  vec4  getSpawnPos(PS_SHAPE);
  vec4  getRandomCircularSpawnPoint(void);
  vec4  getRandomCubeSpawnPoint(void);
  vec4  getRandomSphericalSpawnPoint(void);
  vec4  getRandomHemisphericalSpawnPoint(void);
  vec4  getRandomLinearSpawnPoint(void);

  vec4  getRandomSphericalSpawnPointHelper(float);


  void  respawnParticle(Particle &p);

  Particle *newRandomParticle(void);

  void  addParticle(void);
  void  addSomeParticles( int );
  void  addOneParticleAtOrigin( void );

  void  removeParticle( void );


  /* member variables */

  /* particle control variables */
  vector<ParticleP> _particles;
  unsigned          _numParticles;
  float             _minLife;
  float             _maxLife;

  /* system control variables */
  bool     _pauseTheSystem;
  bool     _enableTheSystem;
  float    _slaughterHeight;
  unsigned _fillSpeedLimit;

  /* emitter control */
  float      _emitterRadius;
  PS_SHAPE   _emitterShape ;
  PS_SHAPE   _systemShape  ;

public:
  TransCache _emitterLoc   ;

private:
  Parameters* _funcParams;
  UserVectorField* _uvf;

  Angel::vec3 (*_vecFieldFunc)(Angel::vec4 posIn, Parameters* functionParams);
  Angel::vec4 (*_colorFunc)(float lifePct, Angel::vec4 posIn);

  unsigned _numToAddEachFrame;
  int      _cachedVisibleCount;

};

#endif
