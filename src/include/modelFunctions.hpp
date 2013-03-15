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

void makeModelsSameSize(Object* model1, Object* model2);
int findTriangleWithMinimumDistance(Object* largerModel, Object* smallerModel, int index);
float threeDimensionalDistance(vec4 points1, vec4 points2);

#endif
