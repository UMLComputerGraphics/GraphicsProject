//#ifdef EXPRTK
#include "UserVectorField.hpp"
#include "vec.hpp"
#include "exprtk/exprtk.hpp"

/**
  This class represents a generic User Defined Function.
  It is NOT a callback, nor is it meant to instantiate one.
**/
UserVectorField::UserVectorField( const std::string &fx,
				  const std::string &fy,
				  const std::string &fz ) {
  _table.add_variable("x",_input.x);
  _table.add_variable("y",_input.y);
  _table.add_variable("z",_input.z);
  _table.add_constants();
  
  _ef[0].register_symbol_table( _table );
  _ef[1].register_symbol_table( _table );
  _ef[2].register_symbol_table( _table );
  
  setAll( fx, fy, fz );
}

Angel::vec3 UserVectorField::eval( Angel::vec4 input ) {
  _input = input;
  return Angel::vec3( _ef[0].value(), _ef[1].value(), _ef[2].value() );
}

/** use set( "string", index ) to change the f_x, f_y and f_z functions. **/
void UserVectorField::set( const std::string &f, size_t i ) {
  if (i > 2)
    throw std::logic_error( "You can only modify the 0th, 1st, or 2nd functions.\n" );
  
  bool res;
  _f[i] = f;
  res = _parser.compile( _f[i], _ef[i] );
  
  if (!res) {
    throw std::invalid_argument( _parser.error().c_str() );
  }
  /*
  // This code doesn't work due to problems with error_t,
  // But it was supplied by the author. No idea why it isn't working.
  for (std::size_t i = 0; i < _parser.error_count(); ++i) {
  error_t error = _parser.get_error(i);
  printf("Err: %02d Pos: %02d Type: [%s] Msg: %s Expr: %s\n",
  static_cast<int>(i),
  static_cast<int>(error.token.position),
  exprtk::parser_error::to_str(error.mode).c_str(),
  error.diagnostic.c_str(),
  f.c_str());
  }
  */
}

void UserVectorField::setAll( const std::string &fx, const std::string &fy, const std::string &fz ) {
    set( fx, 0 );
    set( fy, 1 );
    set( fz, 2 );
}

/**
   An array of std::string is impractical,
   because it has to be copied across the stack to use.
   Prefer setAll( fx, fy, fz ) above.
**/
void UserVectorField::setAll( const std::string f[3] ) {
  setAll( f[0], f[1], f[2] );
}

/*
  This function is dangerous:
  It is a bad paradigm;
  Setting a new function, compiling it and returning it for every particle for every drawcall
  will be untolerably slow.
  
  Instead,
  Create the UVF class ONCE, set it to the function the user requested,
  then use it in a callback.
*/
/* vec3 doAll( const std::string &f_x = "0x + 0y + 0z + 0",
   const std::string &f_y = "0x + 0y + 0z + 0.01",
   const std::string &f_z = "0x + 0y + 0z + 0") {
   string temp[3] = {f_x, f_y, f_z};
   setAll(temp);
   evaluateAllFunctions();
   return getResultingVec3();
   } */

//#endif
