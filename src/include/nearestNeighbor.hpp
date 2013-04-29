/**
 * @file nearestneighbor.hpp
 * @date 2013-04-26
 * @author Zach Maybury
 * @brief FIXME: Documentation needed from Zach.
 * @details Headers for functions related to morphing.
 */

#ifndef __NEARESTNEIGHBOR_HPP
#define __NEARESTNEIGHBOR_HPP

#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include "platform.h"
#include "vec.hpp"
#include "mat.hpp"
#include "Scene.hpp"
#include "bipartiteGraph.hpp"
#include "modelFunctions.hpp"
#include "scaleModel.hpp"
#include "scanMatching.hpp"

struct Square {
	float xMin,xMax,yMin,yMax;
	Angel::vec2 center;
	Triangle* srcMatch;
	Triangle* dstMatch;

	Square(float Xmin,float Xmax,float Ymin, float Ymax){
		xMin = Xmin;
		xMax = Xmax;
		yMin = Ymin;
		yMax = Ymax;
		center = getSquareCenter(xMin,xMax,yMin,yMax);
		srcMatch = NULL;
		dstMatch = NULL;
	}
	Angel::vec2 getSquareCenter(float xMin, float xMax, float yMin, float yMax);
};

struct SquareHorizontalMeshMap {
	std::vector<Square*> squares;
	std::vector< Triangle* > srcTriangles;
	std::vector< Triangle* > destTriangles;
	std::vector< Angel::vec4 > finalSrcVerts;
	std::vector< Angel::vec3 > finalSrcNorms;
	std::vector< Angel::vec4 > finalSrcColrs;
	std::vector< Angel::vec4 > finalDstColrs;
	std::vector< Angel::vec3 > finalDstNorms;
	std::vector< Angel::vec4 > finalDstVerts;

	std::vector<Angel::vec4 > srcVerts;
	std::vector<Angel::vec3 > srcNorms;
	std::vector<Angel::vec4 > srcColrs;
	std::vector<Angel::vec4 > destVerts;
	std::vector<Angel::vec3 > destNorms;
	std::vector<Angel::vec4 > destColrs;

	SquareHorizontalMeshMap(std::vector<Angel::vec4 > SrcVerts,std::vector<Angel::vec3 > SrcNorms,std::vector<Angel::vec4 > SrcColrs,std::vector<Angel::vec4 > DestVerts,std::vector<Angel::vec3 > DestNorms,std::vector<Angel::vec4 > DestColrs,Angel::vec3 srcMin, Angel::vec3 srcMax, Angel::vec3 dstMin, Angel::vec3 dstMax, int xDivisions, int yDivisions){
		xDivisions--;
		yDivisions--;

		srcVerts = SrcVerts;
		srcNorms = SrcNorms;
		srcColrs = SrcColrs;
		destVerts = DestVerts;
		destNorms = DestNorms;
		destColrs = DestColrs;

		for(size_t i=0; i<srcVerts.size(); i+=3){
			srcTriangles.push_back(new Triangle(
					srcVerts[i],srcNorms[i],srcColrs[i],
					srcVerts[i+1],srcNorms[i+1],srcColrs[i+1],
					srcVerts[i+2],srcNorms[i+2],srcColrs[i+2]
			));
		}
		for(size_t i=0; i<destVerts.size(); i+=3){
			destTriangles.push_back(new Triangle(
					destVerts[i],destNorms[i],destColrs[i],
					destVerts[i+1],destNorms[i+1],destColrs[i+1],
					destVerts[i+2],destNorms[i+2],destColrs[i+2]
			));
		}

		//std::cout << "Triangles1: " << srcTriangles.size() << std::endl;
		//std::cout << "Triangles2: " << destTriangles.size() << std::endl;
		sortTriangles(srcTriangles);
		sortTriangles(destTriangles);

		float xMax = std::max(srcMax.x,dstMax.x);
		float xMin = std::max(srcMin.x,dstMin.x);
		float yMax = std::max(srcMax.z,dstMax.z);
		float yMin = std::max(srcMin.z,dstMin.z);

		float xIncrement = (xMax-xMin)/(xDivisions);
		float yIncrement = (floor(yMax+1)-floor(yMin))/(yDivisions);
		for(float i=floor(yMax+1); i>floor(yMin)-.01; i-=yIncrement){
			for(float j=xMin;j<xMax;j+=xIncrement){
				//std::cout << j << " " << j+xIncrement << " " << i-yIncrement << " " << i << std::endl;
				squares.push_back(new Square(j,j+xIncrement,i-yIncrement,i));
			}
		}
		//std::cout << "Square: " << squares.size() << " " << xIncrement << " " << yIncrement << std::endl;
		//Match squares to nearest neighbor in src and dst
		for(size_t i=0;i<squares.size();i++){
			int minIndex = -1;
			float minDistance = INFINITY;
			for(size_t j=0; j<srcTriangles.size();j++){
				float distance = twoDimensionalXZDistance(srcTriangles[j]->a->vertex,squares[i]->center)+twoDimensionalDistance(srcTriangles[j]->b->vertex,squares[i]->center)+twoDimensionalDistance(srcTriangles[j]->c->vertex,squares[i]->center);
				if(distance < minDistance){
					minDistance = distance;
					minIndex = j;
					//std::cout << minIndex << " " << minDistance << std::endl;
				}
			}
			squares[i]->srcMatch = srcTriangles[minIndex];
			srcTriangles[minIndex]->covered = true;
			//std::cout << minIndex << " " << minDistance << std::endl;
		}

		for(size_t i=0;i<squares.size();i++){
			int minIndex = -1;
			float minDistance = INFINITY;
			for(size_t j=0; j<destTriangles.size();j++){
				float distance = twoDimensionalXZDistance(destTriangles[j]->a->vertex,squares[i]->center)+twoDimensionalDistance(destTriangles[j]->b->vertex,squares[i]->center)+twoDimensionalDistance(destTriangles[j]->c->vertex,squares[i]->center);
				if(distance < minDistance){
					minDistance = distance;
					minIndex = j;
					//std::cout << minIndex << " " << minDistance << std::endl;
				}
			}
			//std::cout << squares[i]->center << std::endl;
			squares[i]->dstMatch = destTriangles[minIndex];
			destTriangles[minIndex]->covered = true;
		}
		copyToFinal();
	}

	void copyToFinal();
	void sortTriangles(std::vector< Triangle* >& triangles);
	int findTriangleWithMinimumDistance(std::vector< Triangle* > larger, std::vector< Triangle* > smaller, int index);
};

struct SquareVerticalMeshMap {
	std::vector<Square*> squares;
	std::vector< Triangle* > srcTriangles;
	std::vector< Triangle* > destTriangles;
	std::vector< Angel::vec4 > finalSrcVerts;
	std::vector< Angel::vec3 > finalSrcNorms;
	std::vector< Angel::vec4 > finalSrcColrs;
	std::vector< Angel::vec4 > finalDstColrs;
	std::vector< Angel::vec3 > finalDstNorms;
	std::vector< Angel::vec4 > finalDstVerts;

	std::vector<Angel::vec4 > srcVerts;
	std::vector<Angel::vec3 > srcNorms;
	std::vector<Angel::vec4 > srcColrs;
	std::vector<Angel::vec4 > destVerts;
	std::vector<Angel::vec3 > destNorms;
	std::vector<Angel::vec4 > destColrs;
	SquareVerticalMeshMap(std::vector<Angel::vec4 > SrcVerts,std::vector<Angel::vec3 > SrcNorms,std::vector<Angel::vec4 > SrcColrs,std::vector<Angel::vec4 > DestVerts,std::vector<Angel::vec3 > DestNorms,std::vector<Angel::vec4 > DestColrs,Angel::vec3 srcMin, Angel::vec3 srcMax, Angel::vec3 dstMin, Angel::vec3 dstMax, int xDivisions, int yDivisions){
		xDivisions--;
		yDivisions--;

		srcVerts = SrcVerts;
		srcNorms = SrcNorms;
		srcColrs = SrcColrs;
		destVerts = DestVerts;
		destNorms = DestNorms;
		destColrs = DestColrs;

		for(size_t i=0; i<srcVerts.size(); i+=3){
			srcTriangles.push_back(new Triangle(
					srcVerts[i],srcNorms[i],srcColrs[i],
					srcVerts[i+1],srcNorms[i+1],srcColrs[i+1],
					srcVerts[i+2],srcNorms[i+2],srcColrs[i+2]
			));
		}
		for(size_t i=0; i<destVerts.size(); i+=3){
			destTriangles.push_back(new Triangle(
					destVerts[i],destNorms[i],destColrs[i],
					destVerts[i+1],destNorms[i+1],destColrs[i+1],
					destVerts[i+2],destNorms[i+2],destColrs[i+2]
			));
		}

		//std::cout << "Triangles1: " << srcTriangles.size() << std::endl;
		//std::cout << "Triangles2: " << destTriangles.size() << std::endl;
		sortTriangles(srcTriangles);
		sortTriangles(destTriangles);

		float xMax = std::max(srcMax.x,dstMax.x);
		float xMin = std::max(srcMin.x,dstMin.x);
		float yMax = std::max(srcMax.y,dstMax.y);
		float yMin = std::max(srcMin.y,dstMin.y);

		float xIncrement = (xMax-xMin)/(xDivisions);
		float yIncrement = (floor(yMax+1)-floor(yMin))/(yDivisions);
		for(float i=floor(yMax+1); i>floor(yMin)-.01; i-=yIncrement){
			for(float j=xMin;j<xMax;j+=xIncrement){
				//std::cout << j << " " << j+xIncrement << " " << i-yIncrement << " " << i << std::endl;
				squares.push_back(new Square(j,j+xIncrement,i-yIncrement,i));
			}
		}
		//std::cout << "Square: " << squares.size() << " " << xIncrement << " " << yIncrement << std::endl;
		//Match squares to nearest neighbor in src and dst
		for(size_t i=0;i<squares.size();i++){
			int minIndex = -1;
			float minDistance = INFINITY;
			for(size_t j=0; j<srcTriangles.size();j++){
				float distance = twoDimensionalDistance(srcTriangles[j]->a->vertex,squares[i]->center)+twoDimensionalDistance(srcTriangles[j]->b->vertex,squares[i]->center)+twoDimensionalDistance(srcTriangles[j]->c->vertex,squares[i]->center);
				if(distance < minDistance){
					minDistance = distance;
					minIndex = j;
					//std::cout << minIndex << " " << minDistance << std::endl;
				}
			}
			squares[i]->srcMatch = srcTriangles[minIndex];
			srcTriangles[minIndex]->covered = true;
			//std::cout << minIndex << " " << minDistance << std::endl;
		}

		for(size_t i=0;i<squares.size();i++){
			int minIndex = -1;
			float minDistance = INFINITY;
			for(size_t j=0; j<destTriangles.size();j++){
				float distance = twoDimensionalDistance(destTriangles[j]->a->vertex,squares[i]->center)+twoDimensionalDistance(destTriangles[j]->b->vertex,squares[i]->center)+twoDimensionalDistance(destTriangles[j]->c->vertex,squares[i]->center);
				if(distance < minDistance){
					minDistance = distance;
					minIndex = j;
					//std::cout << minIndex << " " << minDistance << std::endl;
				}
			}
			//std::cout << squares[i]->center << std::endl;
			squares[i]->dstMatch = destTriangles[minIndex];
			destTriangles[minIndex]->covered = true;
		}
		copyToFinal();
		//copyToBuffers();
	}
	void copyToBuffers();
	void copyToFinal();
	void copyBackToObjects(Object* Src,Object* Dest,std::vector< Square* > squares);
	void sortTriangles(std::vector< Triangle* >& triangles);
	int findTriangleWithMinimumDistance(std::vector< Triangle* > larger, std::vector< Triangle* > smaller, int index);
};

void nearestNeighbor(Object* model1, Object* model2);
void rectangularMapping(Object* model1, Object* model2);
bool compareSquareTriangles(Triangle* a, Triangle* b);
Angel::vec3 getMin(std::vector<Angel::vec4> points);
Angel::vec3 getMax(std::vector< Angel::vec4> points);
void copyToVector(std::vector< Angel::vec4 >& dest, std::vector< Angel::vec4 > src);
void copyToVector(std::vector< Angel::vec3 >& dest, std::vector< Angel::vec3 > src);
Angel::vec4 averageVec(Angel::vec4 a, Angel::vec4 b);


#endif
