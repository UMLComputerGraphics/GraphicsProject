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
#include "bipartiteGraph.hpp"
#include "scaleModel.hpp"
#include "scanMatching.hpp"

using Angel::vec3;
using Angel::vec4;

void makeModelsSameSize( Object* model1, Object* model2 );
void makeSameSize(std::vector<Triangle*> model1, std::vector<Triangle*> model2);
void splitProblemTriangles(Object* model1, Object* model2);
float getTriangleSize(vec4 a, vec4 b, vec4 c);
void segmentModels(Object* model1, vec3 model1Low, vec3 model1High, Object* model2, vec3 model2Low, vec3 model2High);
int findTriangleWithMinimumDistance( Object* largerModel, Object* smallerModel,
                                     int index );
void divideTriangle(Object* model, int index);
void makeModelTopSameSize(std::vector< Angel::vec4 >& model1Vertices,std::vector< Angel::vec3 >& model1Normals,std::vector< Angel::vec4 >& model1Colors,std::vector< Angel::vec2 >& model1Textures, std::vector< Angel::vec4 >& model2Vertices,std::vector< Angel::vec3 >& model2Normals,std::vector< Angel::vec4 >& model2Colors,std::vector< Angel::vec2 >& model2Textures);
float threeDimensionalDistance( vec4 points1, vec4 points2 );
float twoDimensionalDistance( vec4 points1, vec4 points2);
float twoDimensionalDistance( vec4 points1, Angel::vec2 points2);
float twoDimensionalXZDistance( vec4 points1, vec4 points2);
float twoDimensionalXZDistance( vec4 points1, Angel::vec2 points2);
float threeDimensionalDistanceWithOrigin( vec4 points1, vec4 points2);
void matchInitialPoints(Object* model1, Object* model2);
void matchPoints(std::vector< Angel::vec4 >& model1Vertices,std::vector< Angel::vec3 >& model1Normals,std::vector< Angel::vec4 >& model1Colors,std::vector< Angel::vec2 >& model1Textures, std::vector< Angel::vec4 >& model2Vertices,std::vector< Angel::vec3 >& model2Normals,std::vector< Angel::vec4 >& model2Colors,std::vector< Angel::vec2 >& model2Textures);
float threeDimensionalDistanceWithNormals(vec4 points1, vec3 normals1, vec4 points2, vec3 normals2);
float threeDimensionalDistanceFromCenter(vec4 points1, vec4 points2);
vec4 findCenter(vec4 point1, vec4 point2, vec4 point3);
void findOptimalOrientation(vec4 a, vec4 b, vec4 c, Object* model, int index, vec4& point1, vec4& point2, vec4& point3);
void findOptimalOrientation(vec4 a, vec4 b, vec4 c, std::vector< Angel::vec4 > model, int index, vec4& point1, vec4& point2, vec4& point3);
void findOptimalOrientation(vec4 a, vec4 b, vec4 c, Object* model, int index, int& point1, int& point2, int& point3);
void findOptimalOrientation(vec4 a, vec4 b, vec4 c, vec4 d, vec4 e, vec4 f, vec4& point1, vec4& point2, vec4& point3);
void findOptimalOrientation(vec4 a, vec4 b, vec4 c, std::vector< Triangle* > model, int index, vec4& point1, vec4& point2, vec4& point3);
int findTriangleWithMinimumDistanceFromCenter(Object* largerModel, Object* smallerModel, int index);
int findTriangleWithMinimumDistanceFromCenter(std::vector< Angel::vec4 > largerPoints, std::vector< Angel::vec4 > smallerPoints, int index);
int findTriangleWithMinimumDistance(std::vector< Triangle* > larger, std::vector< Triangle* > smaller, int index);
void applyToObjects(Object* model1, Object* model2, std::vector< Angel::vec4 > model1Vertices[3],std::vector< Angel::vec3 > model1Normals[3],std::vector< Angel::vec4 > model1Colors[3],std::vector< Angel::vec2 > model1Textures[3],std::vector< Angel::vec4 > model2Vertices[3],std::vector< Angel::vec3 > model2Normals[3],std::vector< Angel::vec4 > model2Colors[3],std::vector< Angel::vec2 > model2Textures[3], size_t partitionSize);

#endif
