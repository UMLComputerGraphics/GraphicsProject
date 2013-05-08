#ifndef __UserVectorField_HPP
#define __UserVectorField_HPP

//#ifdef EXPRTK

#include <string>
#include "vec.hpp"
#include "exprtk/exprtk.hpp"

/**
  This class represents a generic User Defined Function.
  It is NOT a callback, nor is it meant to instantiate one.
**/
class UserVectorField {
public:
  UserVectorField( const std::string &fx = "0x + 0y + 0z + 0",
		   const std::string &fy = "0x + 0y + 0z + 0.01",
		   const std::string &fz = "0x + 0y + 0z + 0" );

  Angel::vec3 eval( Angel::vec4 input );
  void set( const std::string &f, size_t i );
  void setAll( const std::string &fx, const std::string &fy, const std::string &fz );
  void setAll( const std::string f[3] );

private:
  Angel::vec4 _input;
  std::string _f[3];
  exprtk::symbol_table<GLfloat> _table;
  exprtk::expression<GLfloat> _ef[3];
  exprtk::parser<GLfloat> _parser;

};

//#endif // ifdef EXPRTK
#endif // ifndef __UserVectorField_HPP

