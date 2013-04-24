/**
 * @file model.cpp
 * @authors Ed Angel, John Huston, Chris Compton, Nick St.Pierre
 * @date 2013-03-14
 * @brief Functions related to constructing simple geometry.
 */

#include "Util.hpp"
#include "globals.h"
#include "Object.hpp"
#include "Timer.hpp"
#include "vec.hpp"
#include <cmath>
using namespace Angel;

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using std::vector;
using std::fstream;
using std::ifstream;
using std::istringstream;
using std::string;
using std::ios;
using std::cerr;
using std::endl;

/**
 * Adds another vertex to the specified object.
 *
 * @param obj The object to add the vertex to.
 * @param the_point The 4d spatial coordinate of the vertex.
 * @param the_color The vec4 specifying the RGBA color value of the vertex.
 * @param the_normal The vec3 that specifies the normal for this vertex.
 */
void createPoint( Object *obj, Point4 const &the_point, Color4 const &the_color,
                  vec3 const &the_normal ) {
  
  obj->_vertices.push_back( the_point );
  obj->_colors.push_back( the_color );
  obj->_normals.push_back( the_normal );
  
}

/**
 * Creates a triangle primitive from three spatial coordinates.
 *
 * @param obj The object to add the triangle to.
 * @param a The location of the first vertex.
 * @param b The location of the second vertex.
 * @param c The location of the third vertex.
 * @param color An index for the color to use for the triangle:
 * { Red, Green, Blue, Yellow, Pink, White }
 */
void triangle( Object *obj, const Point4& a, const Point4& b, const Point4& c,
               const int color ) {
  
  static vec4 base_colors[] = { vec4( 1.0, 0.0, 0.0, 1.0 ), /* R */
                                vec4( 0.0, 1.0, 0.0, 1.0 ), /* G */
                                vec4( 0.0, 0.0, 1.0, 1.0 ), /* B */
                                vec4( 1.0, 1.0, 0.0, 1.0 ), /* Y */
                                vec4( 0.4, 0.1, 0.8, 1.0 ), /* P */
                                vec4( 1.0, 1.0, 1.0, 1.0 ) /* W */
  };
  
  // Initialize temporary vectors along the quad's edge to
  //   compute its face normal
  
  vec3 u = normalize( vec3( b.x - a.x, b.y - a.y, b.z - a.z ) );
  vec3 v = normalize( vec3( c.x - b.x, c.y - b.y, c.z - b.z ) );
  
  vec3 normal = normalize( cross( u, v ) );
  
  createPoint( obj, a, base_colors[color], normal );
  createPoint( obj, b, base_colors[color], normal );
  createPoint( obj, c, base_colors[color], normal );
  
}

/**
 * Ed Angel utility function: Used to normalize a vector.
 * TODO: Is this a redundant version of Angel::normalize?
 *
 * @param p
 * @return
 */
Point4 unit( const Point4 &p ) {
  Point4 c;
  double d = 0.0;
  
  for ( int i = 0; i < 3; i++ )
    d += p[i] * p[i];
  
  d = sqrt( d );
  
  if ( d > 0.0 ) for ( int i = 0; i < 3; i++ )
    c[i] = p[i] / d;
  
  c[3] = 1.0;
  
  return c;
  
}

/**
 * Used in building the Sierpinski gasket: Takes the coordinate for a triangle and
 * splits it into several smaller triangles.
 *
 * @param obj The object to add the triangles to.
 * @param a The first spatial coordinate for the triangle.
 * @param b The second spatial coordinate for the triangle.
 * @param c The third spatial coordinate for the triangle.
 * @param timesToRecurse The number of times to subdivide.
 * @param color An index for the color to use for the triangle:
 * { Red, Green, Blue, Yellow, Pink, White }
 */
void divideTriangle( Object *obj, const Point4& a, const Point4& b,
                     const Point4& c, int timesToRecurse, int color ) {
  
  Point4 v1, v2, v3;
  
  if ( timesToRecurse > 0 ) {
    v1 = unit( a + b );
    v2 = unit( a + c );
    v3 = unit( b + c );
    
    divideTriangle( obj, a, v2, v1, timesToRecurse - 1, color );
    divideTriangle( obj, c, v3, v2, timesToRecurse - 1, color );
    divideTriangle( obj, b, v1, v3, timesToRecurse - 1, color );
    divideTriangle( obj, v1, v2, v3, timesToRecurse - 1, color );
  } else triangle( obj, a, b, c, color );
}

/**
 * Creates a tetrahedron using four triangles. (12 vertices.)
 *
 * @param obj The object to add the Tetrahedron to/
 * @param a The first spatial coordinate for the tetrahedron.
 * @param b The second spatial coordinate for the tetrahedron.
 * @param c The third spatial coordinate for the tetrahedron.
 * @param d The fourth spatial coordinate for the tetrahedron.
 */
void tetra( Object *obj, const Point4& a, const Point4& b, const Point4& c,
            const Point4& d ) {
  
  triangle( obj, a, b, c, 0 );
  triangle( obj, a, c, d, 1 );
  triangle( obj, a, d, b, 2 );  // old   triangle( a, d, b, 2 );
  triangle( obj, d, c, b, 3 );
  
}

/**
 * Forms a Sierpinski Pyramid object given four 4D _vertices in space.
 * @param obj The object to add the geometry to.
 * @param a The first coordinate.
 * @param b The second coordinate.
 * @param c The third coordinate.
 * @param d The fourth coordinate.
 * @param count The number of recursions to perform to construct the gasket.
 */
void sierpinskiPyramid( Object *obj, const Point4& a, const Point4& b,
                        const Point4& c, const Point4& d, int count ) {
  if ( count > 0 ) {
    Point4 v0 = (a + b) / 2.0;
    Point4 v1 = (a + c) / 2.0;
    Point4 v2 = (a + d) / 2.0;
    Point4 v3 = (b + c) / 2.0;
    Point4 v4 = (c + d) / 2.0;
    Point4 v5 = (b + d) / 2.0;
    sierpinskiPyramid( obj, a, v0, v1, v2, count - 1 );
    sierpinskiPyramid( obj, v0, b, v3, v5, count - 1 );
    sierpinskiPyramid( obj, v1, v3, c, v4, count - 1 );
    sierpinskiPyramid( obj, v2, v5, v4, d, count - 1 );
  } else {
    tetra( obj, a, b, c, d );   // draw tetrahedron at end of recursion
  }
}

/**
 * Given a quadrilateral, splits it up into smaller quadrilaterals.
 * Used in the generation of spheres!
 * FIXME: Nick St.Pierre (Documentation!)
 *
 * @param obj The object to add the geometry to.
 * @param a The first spatial coordinate.
 * @param b The second spatial coordinate.
 * @param c The third spatial coordinate.
 * @param d The fourth spatial coordinate.
 * @param timesToRecurse The number of subdivisions to make.
 * @param color An index for the color to use for the triangle:
 * { Red, Green, Blue, Yellow, Pink, White }
 */
void recursiveModelGen( Object *obj, const Point4& a, const Point4& b,
                        const Point4& c, const Point4& d, int timesToRecurse,
                        int color ) {
  
  divideTriangle( obj, a, b, c, timesToRecurse, color );
  divideTriangle( obj, d, c, b, timesToRecurse, color );
  divideTriangle( obj, a, d, b, timesToRecurse, color );
  divideTriangle( obj, a, d, c, timesToRecurse, color );
}

/**
 * Creates a white sphere.
 *
 * @param obj The object to add the geometry to.
 */
void sphere( Object *obj ) {
  
  static const Point4 initialSpherePoints[4] = { Point4( 0.0, 0.0, 1.0, 1.0 ),
                                                 Point4( 0.0, 0.942809,
                                                         -0.333333, 1.0 ),
                                                 Point4( -0.816497, -0.471405,
                                                         -0.333333, 1.0 ),
                                                 Point4( 0.816497, -0.471405,
                                                         -0.333333, 1.0 ) };
  
  recursiveModelGen( obj, initialSpherePoints[0], initialSpherePoints[1],
                     initialSpherePoints[2], initialSpherePoints[3], 4, 5 );
  
}

/**
 * Create a quadrilateral from four _vertices and four _colors.
 * @param obj The object to add the geometry to.
 * @param a The first spatial point.
 * @param b The second spatial point.
 * @param c The third spatial point.
 * @param d The fourth spatial point.
 * @param A The color of the first point.
 * @param B The color of the second point.
 * @param C The color of the third point.
 * @param D The color of the fourth point.
 */
void quad( Object *obj, const Point4 &a, const Point4 &b, const Point4 &c,
           const Point4 &d, const Color4 &A, const Color4 &B, const Color4 &C,
           const Color4 &D ) {
  
  // Initialize temporary vectors along the quad's edge to                                                                                 
  //   compute its face normal                                                                                                             
  vec4 u = b - a;
  vec4 v = c - b;
  
  vec3 normal = normalize( cross( u, v ) );
  
  createPoint( obj, a, A, normal );
  createPoint( obj, b, B, normal );
  createPoint( obj, c, C, normal );
  
  createPoint( obj, a, A, normal );
  createPoint( obj, c, C, normal );
  createPoint( obj, d, D, normal );
  
}

void quad( Object *obj, const Point4 &a, const Point4 &b, const Point4 &c,
           const Point4 &d, const Color4 &COLOR ) {

  quad( obj, a, b, c, d, COLOR, COLOR, COLOR, COLOR );

}

/**
 * @def QUAD
 * A macro to help quickly call quad() with correct parameters.
 * @see quad().
 */
#define QUAD(A,B,C,D)							\
  quad( obj, vertices[A], vertices[B], vertices[C], vertices[D],	\
	colors[A], colors[B], colors[C], colors[D] );

/**
 * Create a cube of a given size fixed at the origin, using the
 * eight _colors specified.
 *
 * @param obj The object to add the geometry to.
 * @param size The size of the cube to create.
 * @param colors An array of eight _colors for the vertices.
 */
void cube( Object *obj, const GLfloat &size, const Color4 colors[8] ) {
  
  const GLfloat lower = -(size / 2);
  const GLfloat upper = (size / 2);
  
  const Point4 vertices[8] = { Point4( lower, lower, upper, 1.0 ), Point4(
      lower, upper, upper, 1.0 ),
                               Point4( upper, upper, upper, 1.0 ), Point4(
                                   upper, lower, upper, 1.0 ),
                               Point4( lower, lower, lower, 1.0 ), Point4(
                                   lower, upper, lower, 1.0 ),
                               Point4( upper, upper, lower, 1.0 ), Point4(
                                   upper, lower, lower, 1.0 ) };
  
  QUAD( 1, 0, 3, 2 );
  QUAD( 2, 3, 7, 6 );
  QUAD( 3, 0, 4, 7 );
  QUAD( 6, 5, 1, 2 );
  QUAD( 4, 5, 6, 7 );
  QUAD( 5, 4, 0, 1 );
  
}

/**
 * Creates a cube of a given size fixed at the origin,
 * using all eight primary _colors.
 *
 * @param obj The object to add the geometry to.
 * @param size The size of the cube to create.
 */
void colorCube( Object *obj, GLfloat size ) {
  
  static const Color4 vertex_colors[8] = { Color4( 0.0, 0.0, 0.0, 1.0 ),// black
                                           Color4( 1.0, 0.0, 0.0, 1.0 ),  // red
                                           Color4( 1.0, 1.0, 0.0, 1.0 ),// yellow
                                           Color4( 0.0, 1.0, 0.0, 1.0 ),// green
                                           Color4( 0.0, 0.0, 1.0, 1.0 ), // blue
                                           Color4( 1.0, 0.0, 1.0, 1.0 ),// magenta
                                           Color4( 1.0, 1.0, 1.0, 1.0 ),// white
                                           Color4( 0.0, 1.0, 1.0, 1.0 ) // cyan
      };
  
  cube( obj, size, vertex_colors );
  
}

/**
 * Calculate the vector normal to the triangle formed by three _vertices.
 * @param a First vertex.
 * @param b Second vertex.
 * @param c Third vertex.
 *
 * @return The vector normal to the plane formed by the triangle a,b,c.
 */
vec3 calcNormal( Point4 &a, Point4 &b, Point4 &c ) {
  return cross( c - b, b - a );
}

// The following are helper macros for landGen.
// They help you simulate a 2D array of height values
// inside of a 1D array.
/**
 * @def OFFSET_AT(X, Z)
 * Gives the one-dimensional index offset for
 * a buffer given the terrain's X,Z coordinates.
 */
#define OFFSET_AT(X,Z) ((X)*S+(Z))
/**
 * Gives the vertex for the terrain data at (X,Z).
 */
#define VERTEX_AT(X,Z) (vec.at(OFFSET_AT(X,Z)))
/**
 * Returns the height as a float for these terrain coordinates.
 */
#define HEIGHT_AT(X,Z) (VERTEX_AT(X,Z).y)
/**
 * Returns the color vector for the terrain data at (X,Z).
 */
#define COLOR_AT(X,Z) (col.at(OFFSET_AT(X,Z)))
/**
 * Returns the Texture UV (vec2) for the terrain at (X,Z).
 */
#define TEX_UV_AT(X,Z) (txy.at(OFFSET_AT(X,Z)))
/**
 * Returns the Normal vec3 for the terrain at (X,Z).
 */
#define NORMAL_AT(X,Z) (nor.at(OFFSET_AT(X,Z)))

/**
 * Use the diamond-square terrain generation algorithm to generate a
 * triangle strip that resembles terrain with oceans, mountains, etc.
 *
 * @param obj The object to add the geometry to.
 * @param N The size of the terrain: Will be n^2 x n^2 evenly spaced vertices.
 * @param H The height 'randomness' factor.
 *
 * @return The maximum height actually achieved in this terrain generation.
 */
double landGen( Object *obj, int N, float H ) {
  
  Timer Tick;
  const int S = pow( 2, N ) + 1;
  std::vector< Point4 > &vec = obj->_vertices;
  std::vector< Point4 > &col = obj->_colors;
  //std::vector< vec3 > &nor = obj->_normals;
  std::vector< unsigned int > &drawIndex = obj->_indices;
  std::vector< Angel::vec2 > &txy = obj->_texUVs;
  
  gprint( PRINT_DEBUG, "Beginning landGen() terrain generation.\n" );
  // the range (-h -> h) for the average offset
  // This determines the jaggedness of the peaks and valleys.
  // A smaller initial value will create smaller peaks and shallow valleys for
  // a look more similar to rolling hill tops.
  double CH = 0.5;
  double magnitude = (H * (2 - pow( 2, -(N) )));
  gprint( PRINT_DEBUG, "landGen theoretical magnitude: %f\n", magnitude );
  
  /* Initialize all _vertices in the vector to have their X,Z (and w) coordinates. */
  if ( vec.size() ) vec.clear();
  vec.reserve( S * S );
  for ( int i = 0; i < S; ++i )
    for ( int j = 0; j < S; ++j )
      vec.push_back( vec4( j - (S / 2), 0, i - (S / 2), 1 ) );
  
  /* Initialize our color vectors. */
  if ( col.size() ) col.clear();
  col.reserve( S * S );
  for ( int i = 0; i < S; ++i )
    for ( int j = 0; j < S; ++j )
      col.push_back( vec4( 0.5, 0.5, 0.5, 1.0 ) );
  
  if ( txy.size() ) txy.clear();
  txy.reserve( S * S );
  for ( int z = 0; z < S; ++z )
    for ( int x = 0; x < S; ++x )
      // Set coordinates so that the texture applied to 
      // smaller model coords.
      // Prevent texture from being stretched to fit model.
      txy.push_back(
          vec2( fmod( 100 * (float) x / (float) S, 100 ),
                fmod( 100 * (float) z / (float) S, 100 ) ) );
  
  // Initialize the corners of the grid
  HEIGHT_AT( 0, 0 ) = 0;
  HEIGHT_AT( S-1, 0 ) = 0;
  HEIGHT_AT( 0, S-1 ) = 0;
  HEIGHT_AT( S-1, S-1 ) = 0;
  
  // Populate the (x, y, z) values of vec4 according to the
  // Diamond-Square algorithm
  // sideLength is the distance of a single square side or
  // distance of diagonal in diamond.
  gprint( PRINT_DEBUG, "Entering for( sideLength... ) ...\n" );
  for ( int sideLength = S - 1; sideLength >= 2;
      sideLength /= 2, H /= 2.0, CH /= 2.0 ) {
    int halfSide = sideLength / 2;
    // generate new square values
    for ( int x = 0; x < S - 1; x += sideLength ) {
      for ( int z = 0; z < S - 1; z += sideLength ) {
        double avg = (HEIGHT_AT( x, z ) + HEIGHT_AT( x + sideLength, z )
                      + HEIGHT_AT( x, z + sideLength )
                      + HEIGHT_AT( x + sideLength, z + sideLength ))
                     / 4.0;
        
        vec4 color_avg = (COLOR_AT(x,z) + COLOR_AT(x+sideLength,z)
                          + COLOR_AT(x,z+sideLength)
                          + COLOR_AT(x+sideLength,z+sideLength))
                         / 4.0;
        vec4 color_jitter = vec4( Util::jitter( CH ), Util::jitter( CH ), Util::jitter( CH ), 0 );
        
        HEIGHT_AT( x + halfSide, z + halfSide ) = avg + Util::jitter( H );
        COLOR_AT( x + halfSide, z + halfSide ) = color_avg + color_jitter;
      } // for z
    } // for x
    
    // Generate the diamond values
    // Since diamonds are staggered, we only move x by half side
    // NOTE: If the data shouldn't wrap the x < SIZE and y < SIZE
    for ( int x = 0; x < S - 1; x += halfSide ) {
      for ( int z = (x + halfSide) % sideLength; z < S - 1; z += sideLength ) {
        
        // x,z is center of diamond
        // We must use mod and add SIZE for subtraction
        // so that we can wrap around the array to find the corners
        
        double avg = (HEIGHT_AT( (x-halfSide + S) % S, z )
                      + HEIGHT_AT( x + halfSide % S, z )
                      + HEIGHT_AT( x, z + halfSide % S )
                      + HEIGHT_AT( x, (z - halfSide + S) % S ))
                     / 4.0;
        
        vec4 color_avg = (COLOR_AT( (x-halfSide + S) % S, z )
                          + COLOR_AT( x + halfSide % S, z )
                          + COLOR_AT( x, z + halfSide % S )
                          + COLOR_AT( x, (z - halfSide + S) % S ))
                         / 4.0;
        vec4 color_jitter = vec4( Util::jitter( CH ), Util::jitter( CH ), Util::jitter( CH ), 0 );
        
        HEIGHT_AT( x, z ) = avg + Util::jitter( H );
        COLOR_AT( x, z ) = color_avg + color_jitter;
        
        // Wrapping:
        if ( x == 0 ) {
          HEIGHT_AT( S-1, z ) = HEIGHT_AT( x, z );
          COLOR_AT( S-1, z ) = COLOR_AT( x, z );
        }
        if ( z == 0 ) {
          HEIGHT_AT( x, S-1 ) = HEIGHT_AT( x, z );
          COLOR_AT( x, S-1 ) = COLOR_AT( x, z );
        }
      } // for z
    } // for x
  } // for sideLength
  
  //vec.reserve(2*S*S - 2*S);
  // Convert 2D array into 1D array to pass to shaders
  // Size of 1D array is 2*SIZE^2 - 2SIZE for
  // the sake of proper parsing by glTriangleStrips
  if ( drawIndex.size() ) drawIndex.clear();
  drawIndex.reserve( 2 * S * S - 2 * S );
  for ( int i = 0, j = 0; i + 1 < S; i++ ) {
    for ( j = 0; j < S; j++ ) {
      drawIndex.push_back( OFFSET_AT(i,j) );
      drawIndex.push_back( OFFSET_AT(i+1,j) );
    }
    
    /* If we're out of rows to serialize, give up. */
    if ( ++i == S ) break;
    
    for ( ; j > 0; j-- ) {
      drawIndex.push_back( OFFSET_AT(i,j-1) );
      drawIndex.push_back( OFFSET_AT(i+1,j-1) );
    }
  }
  
  Tick.tock();
  gprint( PRINT_INFO, "Landgen took %lu usec, %f msec, %f sec to generate %d vertices.\n",
	 Tick.delta(), Tick.delta() / 1000.0, Tick.delta() / 1000000.0, S * S );
  
  return magnitude;
}

/**
 * Adds a blue quadrilateral to an already-generated terrain object
 * to create the appearance of water.
 *
 * @param land_obj
 * @param agua_obj
 */
void makeAgua( Object *land_obj, Object *agua_obj ) {
  
  /// What should the water's height be?
  float wh = 0.1;
  
  // What should the water's color be?
  const vec4 wc( 0.0, 0.0, 0.8, 0.6 );
  
  // Ensure that the size of agua_obj == land_obj
  int S = sqrt( land_obj->_vertices.size() );
  
  // Push the vertices
  agua_obj->_vertices.push_back( vec4( -S / 2, wh, S / 2, 1 ) );
  agua_obj->_vertices.push_back( vec4( -S / 2, wh, -S / 2, 1 ) );
  agua_obj->_vertices.push_back( vec4( S / 2, wh, S / 2, 1 ) );
  agua_obj->_vertices.push_back( vec4( S / 2, wh, S / 2, 1 ) );
  agua_obj->_vertices.push_back( vec4( -S / 2, wh, -S / 2, 1 ) );
  agua_obj->_vertices.push_back( vec4( S / 2, wh, -S / 2, 1 ) );
  
  agua_obj->_colors.push_back( wc );
  agua_obj->_colors.push_back( wc );
  agua_obj->_colors.push_back( wc );
  agua_obj->_colors.push_back( wc );
  agua_obj->_colors.push_back( wc );
  agua_obj->_colors.push_back( wc );
  
  return;
}
