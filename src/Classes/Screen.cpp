#include "Screen.hpp"
#include "Cameras.hpp"
#include "vec.hpp"
using Angel::vec2;

Screen::Screen( int x, int y ) {
  this->size( x, y );
}

Screen::Screen( const vec2 &startSize ) {
  this->size( startSize );
}

void Screen::size( int x, int y ) {
  size( vec2( x, y ) );
}

void Screen::size( const vec2 &newSize ) {
  _size = newSize;
  _center = newSize / 2;
  _camList.resize( _size.x, _size.y );
}

const vec2 &Screen::size( void ) {
  return _size;
}

int Screen::width( void ) {
  return _size.x;
}

int Screen::height( void ) {
  return _size.y;
}

const vec2 &Screen::center( void ) {
  return _center;
}

int Screen::midpointX( void ) {
  return _center.x;
}

int Screen::midpointY( void ) {
  return _center.y;
}
