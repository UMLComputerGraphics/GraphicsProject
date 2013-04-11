/**
 * @file model.hpp
 * @authors Ed Angel, John Huston, Chris Compton, Nick St.Pierre
 * @date 2013-03-14
 * @brief Headers for Functions related to constructing simple geometry.
 */
#ifndef __MODEL_H
#define __MODEL_H

// Needed for model loader
#include "vec.hpp"
#include "Object.hpp"

using Angel::vec4;
using Angel::vec3;

/**
 * Adds another vertex to the specified object.
 *
 * @param obj The object to add the vertex to.
 * @param the_point The 4d spatial coordinate of the vertex.
 * @param the_color The vec4 specifying the RGBA color value of the vertex.
 * @param the_normal The vec3 that specifies the normal for this vertex.
 */
void createPoint( Object *obj, Point4 const &the_point, Color4 const &the_color,
                  vec3 const &the_normal );

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
               const int color );

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
                     const Point4& c, int timesToRecurse, int color );

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
            const Point4& d );

/**
 * Forms a Sierpinski Pyramid object given four 4D points in space.
 * @param obj The object to add the geometry to.
 * @param a The first coordinate.
 * @param b The second coordinate.
 * @param c The third coordinate.
 * @param d The fourth coordinate.
 * @param count The number of recursions to perform to construct the gasket.
 */
void sierpinskiPyramid( Object *obj, const Point4& a, const Point4& b,
                        const Point4& c, const Point4& d, int count );

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
                        int color );

/**
 * Creates a white sphere.
 *
 * @param obj The object to add the geometry to.
 */
void sphere( Object *obj );

/**
 * Create a quadrilateral from four points and four colors.
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
           const Color4 &D );

void quad( Object *obj, const Point4 &a, const Point4 &b, const Point4 &c,
           const Point4 &d, const Color4 &COLOR );

/**
 * Create a cube of a given size fixed at the origin, using the
 * eight colors specified.
 *
 * @param obj The object to add the geometry to.
 * @param size The size of the cube to create.
 * @param colors An array of eight colors for the vertices.
 */
void cube( Object *obj, const GLfloat &size, const Color4 colors[8] );

/**
 * Creates a cube of a given size fixed at the origin,
 * using all eight primary colors.
 *
 * @param obj The object to add the geometry to.
 * @param size The size of the cube to create.
 */
void colorCube( Object *obj, GLfloat size );

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
double landGen( Object *obj, int N, float H );

/**
 * Adds a blue quadrilateral to an already-generated terrain object
 * to create the appearance of water.
 *
 * @param land_obj
 * @param agua_obj
 */
void makeAgua( Object *land_obj, Object *agua_obj );

#endif
