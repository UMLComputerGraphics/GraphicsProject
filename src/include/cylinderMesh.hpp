/**
 * @file cylinderMesh.hpp
 * @date 2013-04-23
 * @author Zach Maybury
 * @brief FIXME: Documentation from Zach
 * @details FIXME: Documentation from Zach
 */

#ifndef __CYLINDERMESH_HPP
#define __CYLINDERMESH_HPP

#include <string>
#include <vector>
#include <cmath>
#include "platform.h"
#include "vec.hpp"
#include "mat.hpp"
#include "Scene.hpp"

std::vector< Angel::vec4 > createCylinder(float height, float width);

#endif
