/**
 * @file Screen.cpp
 * @author John Huston
 * @date 2013-03-29
 * @brief Screen class implementation
 * @details Screen contains size and positioning
 * information for a logical window.
 */

#include "Screen.hpp"
#include "Cameras.hpp"
#include "vec.hpp"
using Angel::vec2;

/**
 * Initializes the Screen with a width and a height.
 * @param x The width: defaults to 0.
 * @param y The height: defaults to 0.
 */
Screen::Screen( int x, int y ) {
  this->size( x, y );
}

/**
 * Initializes the Screen width a width and a height from a vec2.
 * @param startSize the vec2 initializer in the form of {x,y}.
 */
Screen::Screen( const vec2 &startSize ) {
  this->size( startSize );
}

/**
 * Updates the size of the screen.
 * @param x The new width.
 * @param y The new height.
 */
void Screen::size( int x, int y ) {
  size( vec2( x, y ) );
}

/**
 * Updates the size of the screen.
 * @param newSize The new dimensions, passed in as vec2(x,y).
 */
void Screen::size( const vec2 &newSize ) {
  _size = newSize;
  _center = newSize / 2.0;
  _camList.resize( _size.x, _size.y );
}

/**
 * Retrieve a const vec2 reference for the dimensions of what the screen is.
 * @return vec2( x:width, y:height )
 */
const vec2 &Screen::size( void ) {
  return _size;
}

/**
 * Returns the current width of the screen.
 * @return Width of the screen, in pixels, as an integer.
 */
int Screen::width( void ) {
  return _size.x;
}

/**
 * Returns the current height of the screen.
 * @return Height of the screen, in pixels, as an integer.
 */
int Screen::height( void ) {
  return _size.y;
}

/**
 * Fetches a const vec2 reference for the midpoint of the screen.
 * @return Midpoint of the screen as vec2( midX, midY ).
 */
const vec2 &Screen::center( void ) {
  return _center;
}

/**
 * Fetches the widthwise midpoint of the screen.
 * @return The midpoint of the screen's width as an integer.
 */
int Screen::midpointX( void ) {
  return _center.x;
}

/**
 * Fetches the heightwise midpoint of the screen.
 * @return The midpoint of the screen's height as an integer.
 */
int Screen::midpointY( void ) {
  return _center.y;
}
