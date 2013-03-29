#ifndef __MESHMAPPING_HPP
#define __MESHMAPPING_HPP

#include <string>

struct Square {
	float xStart;
	float xEnd;
	float yStart;
	float yEnd;
	Square *topLeft;
	Square *topCenter;
	Square *topRight;
	Square *middleLeft;
	Square *middleCenter;
	Square *middleRight;
	Square *bottomLeft;
	Square *bottomCenter;
	Square *bottomRight;


	Square(float xstart, float xend, float ystart, float yend){
		xStart = xstart;
		xEnd = xend;
		yStart = ystart;
		yEnd = yend;
		topLeft=NULL;
		topCenter=NULL;
		topRight=NULL;
		middleLeft=NULL;
		middleCenter=NULL;
		middleRight=NULL;
		bottomLeft=NULL;
		bottomCenter=NULL;
		bottomRight=NULL;
	}
};

struct SquareMap {
	Square *topLeft;
	Square *topCenter;
	Square *topRight;
	Square *middleLeft;
	Square *middleCenter;
	Square *middleRight;
	Square *bottomLeft;
	Square *bottomCenter;
	Square *bottomRight;

	SquareMap(float xstart, float xend, float ystart, float yend){
		bottomLeft = new Square(xstart,xstart+(xend-xstart)/3,ystart,ystart+(yend-ystart)/3);
		middleLeft = new Square(xstart,xstart+(xend-xstart)/3,ystart+(yend-ystart)/3,ystart+2*(yend-ystart)/3);
		topLeft = new Square(xstart,xstart+(xend-xstart)/3,ystart+2*(yend-ystart)/3, yend);

		bottomCenter = new Square(xstart+(xend-xstart)/3,xstart+2*(xend-xstart)/3,ystart,ystart+(yend-ystart)/3);
		middleCenter = new Square(xstart+(xend-xstart)/3,xstart+2*(xend-xstart)/3,ystart+(yend-ystart)/3,ystart+2*(yend-ystart)/3);
		topCenter = new Square(xstart+(xend-xstart)/3,xstart+2*(xend-xstart)/3,ystart+2*(yend-ystart)/3, yend);

		bottomRight = new Square(xstart+2*(xend-xstart)/3,xend,ystart,ystart+(yend-ystart)/3);
		middleRight = new Square(xstart+2*(xend-xstart)/3,xend,ystart+(yend-ystart)/3,ystart+2*(yend-ystart)/3);
		topRight = new Square(xstart+2*(xend-xstart)/3,xend,ystart+2*(yend-ystart)/3, yend);
	}
};

SquareMap* createSquareMap(float xstart, float xend, float ystart, float yend);
void subdivideSquare(Square square);
Square* subdivideBottomLeft(Square square);
Square* subdivideMiddleLeft(Square square);
Square* subdivideTopLeft(Square square);
Square* subdivideBottomCenter(Square square);
Square* subdivideMiddleCenter(Square square);
Square* subdivideTopCenter(Square square);
Square* subdivideBottomRight(Square square);
Square* subdivideMiddleRight(Square square);
Square* subdivideTopRight(Square square);


#endif
