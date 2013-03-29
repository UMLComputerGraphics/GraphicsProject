#include "meshMapping.hpp"

SquareMap* createSquareMap(float xstart, float xend, float ystart, float yend){
	return new SquareMap(xstart,xend,ystart,yend);
}

void subdivideSquare(Square square){
			square.bottomLeft = subdivideBottomLeft(square);
			square.middleLeft = subdivideMiddleLeft(square);
			square.topLeft = subdivideTopLeft(square);

			square.bottomCenter = subdivideBottomCenter(square);
			square.middleCenter = subdivideMiddleCenter(square);
			square.topCenter = subdivideTopCenter(square);

			square.bottomRight = subdivideBottomRight(square);
			square.middleRight = subdivideMiddleRight(square);
			square.topRight = subdivideTopRight(square);
}

Square* subdivideBottomLeft (Square square){
	return new Square(square.xStart,square.xStart+(square.xEnd-square.xStart)/3,square.yStart,square.yStart+(square.yEnd-square.yStart)/3);
}

Square* subdivideMiddleLeft (Square square){
	return new Square(square.xStart,square.xStart+(square.xEnd-square.xStart)/3,square.yStart+(square.yEnd-square.yStart)/3,square.yStart+2*(square.yEnd-square.yStart)/3);
}

Square* subdivideTopLeft (Square square){
	return new Square(square.xStart,square.xStart+(square.xEnd-square.xStart)/3,square.yStart+2*(square.yEnd-square.yStart)/3, square.yEnd);
}

Square* subdivideBottomCenter (Square square){
	return new Square(square.xStart+(square.xEnd-square.xStart)/3,square.xStart+2*(square.xEnd-square.xStart)/3,square.yStart,square.yStart+(square.yEnd-square.yStart)/3);
}

Square* subdivideMiddleCenter (Square square){
	return new Square(square.xStart+(square.xEnd-square.xStart)/3,square.xStart+2*(square.xEnd-square.xStart)/3,square.yStart+(square.yEnd-square.yStart)/3,square.yStart+2*(square.yEnd-square.yStart)/3);
}

Square* subdivideTopCenter (Square square){
	return new Square(square.xStart+(square.xEnd-square.xStart)/3,square.xStart+2*(square.xEnd-square.xStart)/3,square.yStart+2*(square.yEnd-square.yStart)/3, square.yEnd);
}

Square* subdivideBottomRight (Square square){
	return new Square(square.xStart+2*(square.xEnd-square.xStart)/3,square.xEnd,square.yStart,square.yStart+(square.yEnd-square.yStart)/3);
}

Square* subdivideMiddleRight (Square square){
	return new Square(square.xStart+2*(square.xEnd-square.xStart)/3,square.xEnd,square.yStart+(square.yEnd-square.yStart)/3,square.yStart+2*(square.yEnd-square.yStart)/3);
}

Square* subdivideTopRight (Square square){
	return new Square(square.xStart+2*(square.xEnd-square.xStart)/3,square.xEnd,square.yStart+2*(square.yEnd-square.yStart)/3, square.yEnd);
}
