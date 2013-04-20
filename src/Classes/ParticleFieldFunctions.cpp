#include "ParticleFieldFunctions.hpp"
#include "ParticleSystem.hpp"
#include "vec.hpp"
#include "mat.hpp"

#include <cmath>

using Angel::vec2;
using Angel::vec3;
using Angel::vec4;


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

vec3 ParticleFieldFunctions::up(vec4 pos)
{
  return vec3(0.0,0.01,0.0);
}


//FIXME DOCS PARAMETERS
vec3 ParticleFieldFunctions::tornado(vec4 pos)
{

	vec4 retVal;

	double theta = getTheta(pos);

	retVal = vec4( -sin(theta)/100.0,
			0.001,
			cos(theta)/100.0,
			1.0 ) ;


	retVal = retVal * Angel::RotateY(-M_PI/4.0);

	return xyz(retVal);

}



typedef struct s_attractor{

	float power;
	vec3 position;
	float range;


} attractor;

vec3 ParticleFieldFunctions::flame(vec4 pos)
{

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


/* // jet 1?
vec3 ParticleFieldFunctions::idk(vec4 pos)
{
  return vec3( (sin(pos.x)+pos.y)/1200.0,
               (cos(pos.z)+pos.y)/1200.0,
               pos.y/1000.0) ;

}
 */
