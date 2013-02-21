#include "Screen.hpp"
#include "Cameras.hpp"
#include "vec.hpp"
using Angel::vec2;

Screen::Screen( int x, int y ) {
  this->Size( x, y );
}

Screen::Screen( const vec2 &startSize ) {
  this->Size( startSize );
}
 
void Screen::Size( int x, int y ) {
  Size(vec2( x, y ));
}
		
void Screen::Size( const vec2 &newSize ) {
  size = newSize;
  center = newSize / 2;
  camList.Resize( size.x, size.y );
}
		
const vec2 &Screen::Size( void ) {
  return size;
}
		
int Screen::Width( void ) {
  return size.x;
}
		
int Screen::Height( void ) {
  return size.y;
}
		
const vec2 &Screen::Center( void ) {
  return center;
}
			
int Screen::MidpointX( void ) {
  return center.x;
}
		
int Screen::MidpointY( void ) {
  return center.y;
}
