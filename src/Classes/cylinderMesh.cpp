/**
 * @file cylinderMesh.cpp
 * @date 2013-04-23
 * @author Zach Maybury
 * @brief FIXME: Documentation from Zach
 * @details FIXME: Documentation from Zach
 */

#include "globals.h" // for pi
#include "cylinderMesh.hpp"
#include "Object.hpp"
#include <stdexcept>
#include <algorithm>
#include <cfloat>
#include <vector>


std::vector< Angel::vec4 > createCylinder(float height, float width){
	std::vector< Angel::vec4 > vertices;
	float radius=width/2.0;
	int slices=10;
	int halfLength=height/2.0;
	for(int i=0; i<slices; i++) {
		float theta = ((float)i)*2.0*M_PI;
		float nextTheta = ((float)i+1)*2.0*M_PI;
		vertices.push_back(vec4(0.0,halfLength,0.0,0.0));
		vertices.push_back(vec4(radius*cos(theta), halfLength, radius*sin(theta)));
		vertices.push_back(vec4(radius*cos(nextTheta), halfLength, radius*sin(nextTheta)));

		vertices.push_back(vec4(radius*cos(theta), halfLength, radius*sin(theta)));
		vertices.push_back(vec4(radius*cos(nextTheta), halfLength, radius*sin(nextTheta)));
		vertices.push_back(vec4(radius*cos(nextTheta), -halfLength, radius*sin(nextTheta)));

		vertices.push_back(vec4(radius*cos(nextTheta), halfLength, radius*sin(nextTheta)));
		vertices.push_back(vec4(radius*cos(nextTheta), -halfLength, radius*sin(nextTheta)));
		vertices.push_back(vec4(radius*cos(theta), -halfLength, radius*sin(theta)));

		vertices.push_back(vec4(radius*cos(nextTheta), -halfLength, radius*sin(nextTheta)));
		vertices.push_back(vec4(radius*cos(theta), -halfLength, radius*sin(theta)));
		vertices.push_back(vec4(0.0, -halfLength, 0.0));
	}
	return vertices;
}
