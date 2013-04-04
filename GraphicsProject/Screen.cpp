#include "Screen.hpp"
#include "Cameras.hpp"
#include "vec.hpp"
using Angel::vec2 ;

Screen::Screen(int x, int y) {
  this->Size(x, y) ;
}

Screen::Screen(const vec2 &startSize) {
  this->Size(startSize) ;
}

void Screen::Size(int x, int y) {
  Size(vec2(x, y)) ;
}

void Screen::Size(const vec2 &newSize) {
  _size = newSize ;
  _center = newSize / 2 ;
  _camList.resize(_size.x, _size.y) ;
}

const vec2 &Screen::Size(void) {
  return _size ;
}

int Screen::Width(void) {
  return _size.x ;
}

int Screen::Height(void) {
  return _size.y ;
}

const vec2 &Screen::Center(void) {
  return _center ;
}

int Screen::MidpointX(void) {
  return _center.x ;
}

int Screen::MidpointY(void) {
  return _center.y ;
}
