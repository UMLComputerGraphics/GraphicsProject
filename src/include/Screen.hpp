/**
 * @file Screen.hpp
 * @author John Huston
 * @date 2013-03-29
 * @brief Screen class header file.
 * @details @see Screen.
 */
#ifndef __SCREEN_HPP
#define __SCREEN_HPP

#include "vec.hpp"
#include "Cameras.hpp"
using Angel::vec2;

/**
 * The Screen class encompasses state necessary for rendering
 * properly to one window, including size, perspective and midpoint info.
 *
 * @author John Huston
 * @date 2013-03-30
 */
class Screen {
  
public:
  
  /**
   * Initializes the Screen with a width and a height.
   * @param x The width: defaults to 0.
   * @param y The height: defaults to 0.
   */
  Screen( int x = 0, int y = 0 );

  /**
   * Initializes the Screen width a width and a height from a vec2.
   * @param startSize the vec2 initializer in the form of {x,y}.
   */
  Screen( const vec2 &startSize );

  /**
   * Updates the size of the screen.
   * @param x The new width.
   * @param y The new height.
   */
  void size( int x, int y );

  /**
   * Updates the size of the screen.
   * @param newSize The new dimensions, passed in as vec2(x,y).
   */
  void size( const vec2 &newSize );

  /**
   * Retrieve a const vec2 reference for the dimensions of what the screen is.
   * @return vec2( x:width, y:height )
   */
  const vec2 &size( void );

  /**
   * Returns the current width of the screen.
   * @return Width of the screen, in pixels, as an integer.
   */
  int width( void );

  /**
   * Returns the current height of the screen.
   * @return Height of the screen, in pixels, as an integer.
   */
  int height( void );

  /**
   * Fetches a const vec2 reference for the midpoint of the screen.
   * @return Midpoint of the screen as vec2( midX, midY ).
   */
  const vec2 &center( void );

  /**
   * Fetches the widthwise midpoint of the screen.
   * @return The midpoint of the screen's width as an integer.
   */
  int midpointX( void );

  /**
   * Fetches the heightwise midpoint of the screen.
   * @return The midpoint of the screen's height as an integer.
   */
  int midpointY( void );

  /**
   * A screen encompasses as its state a list of cameras associated with that screen.
   * On height updates, the Cameras are updated with new viewpoint information.
   */
  Cameras _camList;

private:
  
  /**
   * The current size of the screen.
   */
  vec2 _size;

  /**
   * The current midpoint of the screen.
   */
  vec2 _center;
  
};

#endif
