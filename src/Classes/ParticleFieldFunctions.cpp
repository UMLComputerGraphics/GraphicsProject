#include "ParticleFieldFunctions.hpp"
#include "ParticleSystem.hpp"
#include "vec.hpp"
#include "mat.hpp"

#include "exprtk/exprtk.hpp"
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

	//float steepness = ParticleSystem::rangeRandom(2,100) ;

	double scale = 0.01;
	retVal.x = pos.x*scale;
	retVal.z = pos.z*scale;
	retVal.y = 5*(pos.x*pos.x + pos.z*pos.z);

	//attractor code!!!
	attractor atr_top ;

	atr_top.power    = 0.075 ;
	atr_top.position = vec3(0.0, 0.3, 0.0) ;
	atr_top.range =	0.15;
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

	return 0.002*normalize(retVal);

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

Angel::vec3 ParticleFieldFunctions::userSupplied( Angel::vec4 pos ) {
	static bool compiled = false;
	static std::string expressions[3];
	static exprtk::expression<GLfloat> expression[3];
	static exprtk::parser<GLfloat> parser;
	static exprtk::symbol_table<GLfloat> symbol_table;
	static vec4 *input = NULL;

	if (!compiled) {
	  input = new vec4;
		expressions[0] = "0x + 0y + 0z + 0";
		expressions[1] = "0x + 0y + 0z + 0";
		expressions[2] = "0x + 0y + 0z + 0.1";

		symbol_table.add_variable("x",input->x);
		symbol_table.add_variable("y",input->y);
		symbol_table.add_variable("z",input->z);
		symbol_table.add_constants();

		expression[0].register_symbol_table(symbol_table);
		expression[1].register_symbol_table(symbol_table);
		expression[2].register_symbol_table(symbol_table);

		parser.compile(expressions[0], expression[0]);
		parser.compile(expressions[1], expression[1]);
		parser.compile(expressions[2], expression[2]);

		compiled = true;
	}
	*input = pos;
	Angel::vec3 res;
	res.x = expression[0].value();
	res.y = expression[1].value();
	res.z = expression[2].value();
	return res;
}


/* // jet 1?
vec3 ParticleFieldFunctions::idk(vec4 pos)
{
  return vec3( (sin(pos.x)+pos.y)/1200.0,
               (cos(pos.z)+pos.y)/1200.0,
               pos.y/1000.0) ;

}
 */
