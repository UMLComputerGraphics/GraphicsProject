/**
 * @file scanMatching.hpp
 * @date 2013-04-23
 * @author Zach Maybury
 * @brief FIXME: Documentation from Zach
 * @details FIXME: Documentation from Zach
 */

#ifndef __SCANMATCHING_HPP
#define __SCANMATCHING_HPP

#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include "platform.h"
#include "vec.hpp"
#include "mat.hpp"
#include "Scene.hpp"
#include "Object.hpp"

struct Point1 {
	Angel::vec4 vertex;
	Angel::vec3 normal;
	Angel::vec4 color;

	Point1(Angel::vec4 vert,Angel::vec3 norm,Angel::vec4 col){
		vertex = vert;
		normal = norm;
		color = col;
	}
	Point1(){};
	Point1 operator = (Point1);

};

bool comparePoints(Point1* a, Point1* b);

struct Triangle {
	Point1* a;
	Point1* b;
	Point1* c;
	bool covered;
	bool srcMatched,dstMatched;

	Triangle(Angel::vec4 aVerts,Angel::vec3 aNorms,Angel::vec4 aCols,
			Angel::vec4 bVerts,Angel::vec3 bNorms,Angel::vec4 bCols,
			Angel::vec4 cVerts,Angel::vec3 cNorms,Angel::vec4 cCols){
		std::vector<Point1*> items;
		items.push_back(new Point1(aVerts,aNorms,aCols));
		items.push_back(new Point1(bVerts,bNorms,bCols));
		items.push_back(new Point1(cVerts,cNorms,cCols));
		std::sort(items.begin(),items.end(),comparePoints);
		a = items[0];
		b = items[1];
		c = items[2];
		covered = false;
		srcMatched = false;
		dstMatched = false;
	}
};

struct ScanMatch {
	std::vector< Triangle* > srcTriangles;
	std::vector< Triangle* > destTriangles;
	Object* Src;
	Object* Dest;
	int heightScale;
	int widthScale;
	int depthScale;

	ScanMatch(Object* src, Object* dest, int height, int width, int depth){
		Src = src;
		Dest = dest;
		heightScale = height;
		widthScale = width;
		depthScale = depth;
		for(size_t i=0; i<Src->_vertices.size(); i+=3){
			srcTriangles.push_back(new Triangle(
				Src->_vertices[i],Src->_normals[i],Src->_colors[i],
				Src->_vertices[i+1],Src->_normals[i+1],Src->_colors[i+1],
				Src->_vertices[i+2],Src->_normals[i+2],Src->_colors[i+2]
			));
		}
		for(size_t i=0; i<Dest->_vertices.size(); i+=3){
			destTriangles.push_back(new Triangle(
				Dest->_vertices[i],Dest->_normals[i],Dest->_colors[i],
				Dest->_vertices[i+1],Dest->_normals[i+1],Dest->_colors[i+1],
				Dest->_vertices[i+2],Dest->_normals[i+2],Dest->_colors[i+2]
			));
		}
	}
	void scanMatch();
	void scanQuarters();
	void scanQuarterFromTop(std::vector<Triangle*> source, std::vector<Triangle*> destination);
	void sortTriangles(std::vector< Triangle* >& triangles);
	void findOptimalOrientation(vec4 a, vec4 b, vec4 c, std::vector< Triangle* > model, int index, vec4& point1, vec4& point2, vec4& point3);
	int findTriangleWithMinimumDistance(std::vector< Triangle* > larger, std::vector< Triangle* > smaller, int index);
	void copyBackToObjects(Object* object, std::vector< Triangle* > triangles);
	void copyToBuffers();
	void scanFromTop();
	void makeSameSize(std::vector<Triangle*>& model1, std::vector<Triangle*>& model2);
	void matchInitial(std::vector<Triangle*>& model1, std::vector<Triangle*>& model2);
	void addToTriangles(std::vector< Triangle* >& parent, std::vector<Triangle*> addition);
	float threeDimensionalDistance( vec4 points1, vec4 points2 );

};
bool compareTriangles(Triangle* a, Triangle* b);

#endif
