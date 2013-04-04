/**
 * @file meshMapping.cpp
 * @date 2013-03-30
 * @author Zach Maybury
 * @brief FIXME: Documentation from Zach
 * @details FIXME: Documentation from Zach
 */

#include "meshMapping.hpp"

SquareMap* createSquareMap(float xstart, float xend, float ystart, float yend){
	return new SquareMap(xstart,xend,ystart,yend);
}

void subdivideSquare(Square square){
			square.bottomLeft = subdivideBottomLeft(square);
			square.topLeft = subdivideTopLeft(square);

			square.bottomRight = subdivideBottomRight(square);
			square.topRight = subdivideTopRight(square);
}

Square* subdivideBottomLeft (Square square){
	return new Square(square.xStart,square.xStart+(square.xEnd-square.xStart)/3,square.yStart,square.yStart+(square.yEnd-square.yStart)/3);
}

Square* subdivideTopLeft (Square square){
	return new Square(square.xStart,square.xStart+(square.xEnd-square.xStart)/3,square.yStart+2*(square.yEnd-square.yStart)/3, square.yEnd);
}

Square* subdivideBottomRight (Square square){
	return new Square(square.xStart+2*(square.xEnd-square.xStart)/3,square.xEnd,square.yStart,square.yStart+(square.yEnd-square.yStart)/3);
}

Square* subdivideTopRight (Square square){
	return new Square(square.xStart+2*(square.xEnd-square.xStart)/3,square.xEnd,square.yStart+2*(square.yEnd-square.yStart)/3, square.yEnd);
}
