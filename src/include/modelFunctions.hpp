/**
 * @file modelFunctions.hpp
 * @date 2013-03-15
 * @author Zach Maybury
 * @brief FIXME: Documentation needed from Zach.
 * @details Headers for functions related to morphing.
 */

#ifndef __MODELFUNCTIONS_HPP
#define __MODELFUNCTIONS_HPP

#include <vector>
#include <string>
#include <cmath>
#include "platform.h"
#include "vec.hpp"
#include "mat.hpp"
#include "Scene.hpp"

using Angel::vec3;
using Angel::vec4;

void makeModelsSameSize( Object* model1, Object* model2 );
int findTriangleWithMinimumDistance( Object* largerModel, Object* smallerModel,
                                     int index );
float threeDimensionalDistance( vec4 points1, vec4 points2 );
void matchInitialPoints(Object* model1, Object* model2);
float threeDimensionalDistanceWithNormals(vec4 points1, vec3 normals1, vec4 points2, vec3 normals2);
float threeDimensionalDistanceFromCenter(vec4 points1, vec4 points2);
vec4 findCenter(vec4 point1, vec4 point2, vec4 point3);
int findTriangleWithMinimumDistanceFromCenter(Object* largerModel, Object* smallerModel, int index);

#endif
