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


using Angel::vec2;
using Angel::vec3;
using Angel::vec4;
using Angel::mat4;
using std::string;
using std::vector;

typedef Particle* ParticleP;

typedef enum {

  PARTICLE_CUBE,
  PARTICLE_SPHERE,
  PARTICLE_FLAME

} PARTICLE_SYSTEM_SHAPE;

class ParticleSystem : public Object {
  
public:
  
  ParticleSystem( int particleAmt, const std::string &name, GLuint shader );
  ~ParticleSystem( void );

  //particle creation methods

  void  addParticle(void);
  void  addOneParticleAtOrigin( void );
  void  addSomeParticles( int );
  void  fillSystemWithParticles( void );

  Particle *newRandomParticle(void);

  // Getters and Setters
  float getLifespan( void );
  float getMaxLife( void );
  float getMinLife( void );
  int   getNumParticles( void );

  void  setSlaughterHeight( float );
  void  setLifespan( float minLifespan, float maxLifespan );
  void  setNumParticles( int newNumParticles );
  void  setEmitterRadius(float);

  bool  getParticleSpace(void) const;
  void  setParticleSpace(bool);

  /* The Do-All function.  Will do everything needed in order
   to have the particles behave according to our specification
   on each call to Draw(). That's the idea, anyway. */
  void  update();

  virtual void buffer( void );
  virtual void draw( void );

  void  setVectorField(vec3 (*vectorFieldFunc)(vec4) );

  
  static float rangeRandom( float min, float max );

private:  

  float generateLifespan();
  vec4  getRandomCircularSpawnPoint(void);
  vec4 getRandomHemisphericalSpawnPoint(void);
  void respawnParticle(Particle &p);

  vector<ParticleP> particles;
  unsigned numParticles;   // Number of particles that each instance of ParticleSystem will manage
  float  minLife;
  float  maxLife;
  float  _emitterRadius;
  bool   pauseTheSystem;

  float  _slaughterHeight;
  
  bool   updateRequired;

  bool   _useGlobalParticleSpace;

  unsigned _fillSpeedLimit;

  vec3 (*_vecFieldFunc)(vec4 posIn);

};

#endif
