#ifndef __PARTICLE_FIELD_FUNCTIONS
#define __PARTICLE_FIELD_FUNCTIONS

#include "vec.hpp"

using Angel::vec3;
using Angel::vec4;

class Parameters
{
public:
private:
};

class FlameParameters : public Parameters
{
public:
    FlameParameters( vec3 theAtrPos = vec3(0.0, 0.45, 0.0), 
									double theScl = 0.01, float thePwr = 0.1,
									 float theRng = 0.24);
    vec3 atrPos(void);
    double scl(void);
    float pwr(void);
    float rng(void);

private:
    vec3 _atrPos;
    double _scl;
    float _pwr;
    float _rng;
};

class TornadoParameters : public Parameters
{
public:

private:

};

class UserParameters : public Parameters
{
public:
	 UserParameters(std::string *value);
	
	std::string* functions (void);
private:
	std::string* _functions;	
};

namespace ParticleFieldFunctions
{

  Angel::vec3 tornado(Angel::vec4 /*, Parameters parameters*/);

  Angel::vec3 flame(Angel::vec4, Parameters* theParameters);

  Angel::vec3 flameold(Angel::vec4 /*,Parameters value = NULL*/);

  Angel::vec3 fixed(Angel::vec4 /*, Parameters value = NULL */);

  Angel::vec3 up(Angel::vec4 /*, Parameters value = NULL */);

  Angel::vec3 userSupplied( Angel::vec4, Parameters* theParameters);
}

#endif
