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

struct Square {
	float xStart;
	float xEnd;
	float yStart;
	float yEnd;
	Square *topLeft;
	Square *topRight;
	Square *bottomLeft;
	Square *bottomRight;


	Square(float xstart, float xend, float ystart, float yend){
		xStart = xstart;
		xEnd = xend;
		yStart = ystart;
		yEnd = yend;
		topLeft=NULL;
		topRight=NULL;
		bottomLeft=NULL;
		bottomRight=NULL;
	}
};

struct SquareMap {
	Square *topLeft;
	Square *topRight;
	Square *bottomLeft;
	Square *bottomRight;

	SquareMap(float xstart, float xend, float ystart, float yend){
		bottomLeft = new Square(xstart,xstart+(xend-xstart)/2,ystart,ystart+(yend-ystart)/2);
		topLeft = new Square(xstart,xstart+(xend-xstart)/2,ystart+(yend-ystart)/2, yend);

		bottomRight = new Square(xstart+(xend-xstart)/2,xend,ystart,ystart+(yend-ystart)/2);
		topRight = new Square(xstart+(xend-xstart)/2,xend,ystart+(yend-ystart)/2, yend);
	}
};

SquareMap* createSquareMap(float xstart, float xend, float ystart, float yend);
void subdivideSquare(Square square);
Square* subdivideBottomLeft(Square square);
Square* subdivideTopLeft(Square square);
Square* subdivideBottomRight(Square square);
Square* subdivideTopRight(Square square);


#endif
