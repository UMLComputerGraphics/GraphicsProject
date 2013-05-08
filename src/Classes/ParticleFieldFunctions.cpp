#include "ParticleFieldFunctions.hpp"
#include "ParticleSystem.hpp"
#include "vec.hpp"
#include "mat.hpp"
#include "UserVectorField.hpp"
#include <cmath>

using Angel::vec2;
using Angel::vec3;
using Angel::vec4;

typedef struct s_attractor{

	float power;
	vec3 position;
	float range;

} attractor;

double getTheta(vec4 pos)
{

	vec2 reference = vec2(1.0,0.0) ;
	vec2 planar_angle = vec2(pos.x, pos.z) ;

	float dotPdt = dot( reference, planar_angle ) ;

	double theta = acos(dotPdt/(length(reference)*length(planar_angle)));


	if ( pos.z < 0.0 )
	{
		theta = 2 * M_PI - theta ;
	}

	return theta ;

}



vec3 ParticleFieldFunctions::fixed(vec4 pos)
{
  return vec3(0.0,0.0,0.0);
}


vec3 ParticleFieldFunctions::up(vec4 pos, Parameters* parameters)
{
  return vec3(0.0,0.005,0.0);
}

//FIXME DOCS PARAMETERS
vec3 ParticleFieldFunctions::tornado(vec4 pos, Parameters* parameters)
{
	TornadoParameters *theParams = (TornadoParameters *) parameters;

	float a = theParams->a() ;
	float b = theParams->b() ;
	float c = theParams->c() ;

	vec4 retVal;

	double theta = getTheta(pos);

	retVal = vec4( -sin(theta)/a /*100.0*/,
			0.001,
			cos(theta)/b /*100.0*/,
			1.0 ) ;


	retVal = retVal * Angel::RotateY(-M_PI/c /*4.0*/);

	return xyz(retVal);

}

vec3 ParticleFieldFunctions::flame(vec4 pos, Parameters* theParameters)
{
	FlameParameters* parameters = (FlameParameters* ) theParameters;
	vec3 retVal ;

	//float steepness = ParticleSystem::rangeRandom(2,100) ;

	double scale = parameters->scl();
	retVal.x = pos.x*scale;
	retVal.z = pos.z*scale;
	retVal.y = 5*(pos.x*pos.x + pos.z*pos.z);

	//attractor code!!!
	attractor atr_top ;

	atr_top.power    = parameters->pwr() ;
	atr_top.position = parameters->atrPos() ;
	atr_top.range =	parameters->rng();
	// get the distance from the attractor
	vec3 atrDist = atr_top.position - xyz(pos) ;

	float distanceSquare = length(atrDist) * length(atrDist);

	if ( distanceSquare >= 0.1 ){
		atrDist /= distanceSquare ;
	}
	atrDist *= atr_top.power ;
	
	if( length(atr_top.position -xyz(pos)) < atr_top.range)
	{
		retVal.x = retVal.x + atrDist.x ;
		retVal.y = retVal.y + atrDist.y ;
		retVal.z = retVal.z + atrDist.z ;
	}

	// make it so that normalize doesn't try to normalize really small values
	retVal = 1000 * retVal;

	// make sure we don't get really small values anyway
	if (retVal.x <= 0.001 && retVal.y <= 0.001 && retVal.z <= 0.001)
	{
		return retVal;
	}
	
	return 0.002 * normalize(retVal);

}

vec3 ParticleFieldFunctions::flameold(vec4 pos) {

	vec3 retVal ;

	//double theta = getTheta(pos);

	double scale = sin(0.8+(pos.y*M_PI*10))/40.0;
	retVal.x = pos.x*scale;
	retVal.z = pos.z*scale;
	if ( ! pos.y >= 0.5 )
	{
		retVal *= -1.0 ;
	}
	retVal.y = ParticleSystem::rangeRandom(0.0015,0.0025) ;

	//attractor code!!!
	attractor atr_top ;

	atr_top.power    = 0.05 ;
	atr_top.position = vec3(0.0, 0.15, 0.0) ;
	atr_top.range =	.02;
	// get the distance from the attractor
	vec3 woah = atr_top.position - xyz(pos) ;

	float distanceSquare = length(woah) * length(woah);

	if ( distanceSquare >= 0.05 ){
		woah /= distanceSquare ;
	}
	woah *= atr_top.power ;
	/*
	if( length(atr_top.position -xyz(pos)) < atr_top.range)
	{
		retVal.x = retVal.x + woah.x ;
		retVal.y = retVal.y + woah.y ;
		retVal.z = retVal.z + woah.z ;
	}
	*/
	return retVal;

}
TornadoParameters::TornadoParameters( float a, float b, float c ) :
		_a(a),
		_b(b),
		_c(c)
{

}

float TornadoParameters::a( void )
{
	return _a;
}
float TornadoParameters::b( void )
{
	return _b;
}
float TornadoParameters::c( void )
{
	return _c;
}

FlameParameters::FlameParameters( vec3 theAtrPos, double theScl, float thePwr, float theRng) :
		_atrPos(theAtrPos),
		_scl(theScl),
		_pwr(thePwr),
		_rng(theRng)
{
	
}	

vec3 FlameParameters::atrPos(void)
{
	return _atrPos;
}

double FlameParameters::scl(void)
{
	return _scl;
}

float FlameParameters::pwr(void)
{
	return _pwr;
}

float FlameParameters::rng(void)
{
	return _rng;
}

/* only constructor */
UserParameters::UserParameters (UserVectorField* value) : _uvf(value)
{

}

UserVectorField *UserParameters::uvf()
{
    return _uvf;
}

//#ifdef EXPRTK
///**
//   Revert:
//   The userSupplied callback should not accept strings as an input.
//   The userSupplied callback is an example and should do one thing and one thing only:
//   It should return the value of its function.

//   If you supply it strings and re-compile every time, it will be too slow to use.
//   Compile once, evaluate many times is the paradigm, here.
//**/
//Angel::vec3 ParticleFieldFunctions::userSupplied( Angel::vec4 pos ) {
//  static UserVectorField *uvf = NULL;
//  if (uvf == NULL) {
//    uvf = new UserVectorField();
//  }
//  return uvf->eval( pos );
//}

/**
   Try this, instead.
   Make a UVF in advance, and then call this callback,
   passing a reference to the UVF you'd like to use.

   You can manage /that/ UVF object inside of QT if you'd like.
   If you get the Parameters system working, the UVF can be a parameter.
**/
Angel::vec3 ParticleFieldFunctions::userSupplied( Angel::vec4 pos, Parameters* parameters )
{
    UserParameters* theparams = (UserParameters*) parameters;
    return theparams->uvf()->eval( pos );
}
//#endif

/* // jet 1?
vec3 ParticleFieldFunctions::idk(vec4 pos)
{
  return vec3( (sin(pos.x)+pos.y)/1200.0,
               (cos(pos.z)+pos.y)/1200.0,
               pos.y/1000.0) ;

}
 */
