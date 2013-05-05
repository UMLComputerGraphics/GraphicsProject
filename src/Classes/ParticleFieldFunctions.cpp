#include "ParticleFieldFunctions.hpp"
#include "ParticleSystem.hpp"
#include "vec.hpp"
#include "mat.hpp"
#ifdef EXPRTK
#include "exprtk/exprtk.hpp"
#endif
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


vec3 ParticleFieldFunctions::up(vec4 pos)
{
  return vec3(0.0,0.01,0.0);
}

//FIXME DOCS PARAMETERS
vec3 ParticleFieldFunctions::tornado(vec4 pos, Parameters*)
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

FlameParameters::FlameParameters( vec3 theAtrPos, 
									double theScl, float thePwr,
									 float theRng) : 
									 _atrPos(theAtrPos),
									_pwr(thePwr), _rng(theRng)
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

#ifdef EXPRTK
class UserVectorField {

public:
    UserVectorField(vec4 pos)
    {
        _table.add_variable("x", pos.x);
        _table.add_variable("y", pos.y);
        _table.add_variable("z", pos.z);
        _table.add_constants();
        _ef[0].register_symbol_table( _table );
        _ef[1].register_symbol_table( _table );
        _ef[2].register_symbol_table( _table );
    }

    void setAllFunctions(const std::string f[3])
    {
        for ( size_t i = 0; i < 3; i++)
        {
            setF_i(i, f[i]);
        }
    }

    void setF_i(  size_t i, const std::string &f_i = "0x + 0y + 0z + 0")
    {
        // TODO check for bad input and set default value.
        _f[i] = f_i;
    }

    void evaluateF_i (size_t i)
    {
        // TODO check if _f[i] is empty...
        _parser.compile( _f[i], _ef[i] );
    }

    void evaluateAllFunctions ()
    {
        for ( size_t i = 0; i < 3; i++)
        {
         evaluateF_i (i);
        }
    }

    vec3 getResultingVec3()
    {
        return vec3( _ef[0].value(), _ef[1].value(), _ef[2].value());
    }

    vec3 doAll( string* f )
   {
        setAllFunctions(f);
        evaluateAllFunctions();
        return getResultingVec3();
    }

private:
    std::string _f[3];
    exprtk::symbol_table<GLfloat> _table;
    exprtk::expression<GLfloat> _ef[3];
    exprtk::parser<GLfloat> _parser;

};


    /* only constructor */
UserParameters::UserParameters (std::string* functions) : _functions(functions)
{
	
}

/* no setter, the only chance to set the parameters is at construction.
        other wise you will get the defaults. */
std::string* UserParameters::functions(void)
{
    return _functions; 
}


vec3 ParticleFieldFunctions::userSupplied( vec4 pos, Parameters* theParameters)
{
	UserParameters* parameters = (UserParameters *) theParameters;
    UserVectorField *uvf = NULL;
    if (uvf == NULL)
    {
        uvf = new UserVectorField(pos);
    }
    return uvf->doAll(parameters->functions());
}
#endif
//#ifdef EXPRTK
//class UserVectorField {
//public:
//  UserVectorField( const std::string &fx = "0x + 0y + 0z + 0",
//		   const std::string &fy = "0x + 0y + 0z + 0.01",
//		   const std::string &fz = "0x + 0y + 0z + 0" ) {
//    _table.add_variable("x",_input.x);
//    _table.add_variable("y",_input.y);
//    _table.add_variable("z",_input.z);
//    _table.add_constants();
    
//    _ef[0].register_symbol_table( _table );
//    _ef[1].register_symbol_table( _table );
//    _ef[2].register_symbol_table( _table );

//    f( fx, 0 );
//    f( fy, 1 );
//    f( fz, 2 );
//  }
  
//  Angel::vec3 f( Angel::vec4 input ) {
//    _input = input;
//    return vec3( _ef[0].value(), _ef[1].value(), _ef[2].value() );
//  }

//  /** use f( "string", index ) to change the f_x, f_y and f_z functions. **/
//  void f( const std::string &f, size_t i ) {
//    _f[i] = f;
//    _parser.compile( _f[i], _ef[i] );
//  }

//private:
//  std::string _f[3];
//  exprtk::expression<GLfloat> _ef[3];
//  exprtk::parser<GLfloat> _parser;
//  exprtk::symbol_table<GLfloat> _table;
//  Angel::vec4 _input;
//};

//Angel::vec3 ParticleFieldFunctions::userSupplied( Angel::vec4 pos ) {
//  static UserVectorField *uvf = NULL;
//  if (uvf == NULL) { uvf = new UserVectorField(); }
//  return uvf->f( pos );
//}
//#endif

/* // jet 1?
vec3 ParticleFieldFunctions::idk(vec4 pos)
{
  return vec3( (sin(pos.x)+pos.y)/1200.0,
               (cos(pos.z)+pos.y)/1200.0,
               pos.y/1000.0) ;

}
 */
