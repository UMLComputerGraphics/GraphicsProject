/**
 * @file meshMapping.hpp
 * @date 2013-03-30
 * @author Zach Maybury
 * @brief FIXME: Documentation from Zach
 * @details FIXME: Documentation from Zach
 */

#ifndef __MESHMAPPING_HPP
#define __MESHMAPPING_HPP

#include <string>
#include <vector>
#include <cmath>
#include "platform.h"
#include "vec.hpp"
#include "mat.hpp"
#include "Scene.hpp"

using Angel::vec4;

//static int MaxBin = 1;

struct Square {
	float xStart;
	float xEnd;
	float yStart;
	float yEnd;
	int srcBinSize;
	int destBinSize;
	std::vector< Angel::vec4 > srcTriangles;
	std::vector< Angel::vec4 > destTriangles;
	Square *topLeft;
	Square *topRight;
	Square *bottomLeft;
	Square *bottomRight;


	Square(float xstart, float xend, float ystart, float yend){
		xStart = xstart;
		xEnd = xend;
		yStart = ystart;
		yEnd = yend;
		srcBinSize = 0;
		destBinSize = 0;
		topLeft=NULL;
		topRight=NULL;
		bottomLeft=NULL;
		bottomRight=NULL;
	}
};

struct SquareMap {
	Square *front;
	Square *back;
	Square *left;
	Square *right;
	float xStart;
	float xEnd;
	float yStart;
	float yEnd;
	float zStart;
	float zEnd;

	SquareMap(float xstart, float xend, float ystart, float yend, float zstart, float zend){
		xStart = xstart;
		xEnd = xend;
		yStart = ystart;
		yEnd = yend;
		zStart = zstart;
		zEnd = zend;
		front = new Square(xstart, xend, ystart, yend);
		back = new Square(xstart, xend, ystart, yend);
		left = new Square(zstart, zend, ystart, yend);
		right = new Square(zstart, zend, ystart, yend);
	}
};

SquareMap* createSquareMap(float xstart, float xend, float ystart, float yend, float zstart, float zend);
void subdivideSquare(Square* square);
Square* subdivideBottomLeft(Square* square);
Square* subdivideTopLeft(Square* square);
Square* subdivideBottomRight(Square* square);
Square* subdivideTopRight(Square* square);
void populateSrcSquare(SquareMap* squareMap, std::vector< Angel::vec4 > points);
void populateDestSquare(SquareMap* squareMap, std::vector< Angel::vec4 > points);
float triangleArea(float x1, float y1, float x2, float y2, float x3, float y3);
bool isInside(float x1, float y1, float x2, float y2, float x3, float y3, float x, float y);
void expandSquareMap(SquareMap* squareMap);
void expandMap(Square* square, float xmin, float xmax, float ymin, float ymax);



#endif
