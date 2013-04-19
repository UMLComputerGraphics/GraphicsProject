/**
 * @file meshMapping.cpp
 * @date 2013-03-30
 * @author Zach Maybury
 * @brief FIXME: Documentation from Zach
 * @details FIXME: Documentation from Zach
 */

#include "meshMapping.hpp"
#define MAXBIN (1)

SquareMap* createSquareMap(float xstart, float xend, float ystart, float yend, float zstart, float zend){
	return new SquareMap(xstart,xend,ystart,yend, zstart, zend);
}

void subdivideSquare(Square* square){
			square->bottomLeft = subdivideBottomLeft(square);
			square->topLeft = subdivideTopLeft(square);

			square->bottomRight = subdivideBottomRight(square);
			square->topRight = subdivideTopRight(square);
}

void populateSrcSquare(SquareMap* squareMap, std::vector< Angel::vec4 > points){
	for(int i=0; i<int(points.size());i+=3){
		if(isInside(0.0,0.0,squareMap->xStart,squareMap->zStart,squareMap->xEnd,squareMap->zStart,float(points[i].x),float(points[i].z))){
			//front
			for(int j=0; j<3; j++){
				squareMap->front->srcTriangles.push_back(points[i+j]);
			}
		}else if(isInside(0.0,0.0,squareMap->xStart,squareMap->zEnd,squareMap->xEnd,squareMap->zEnd,float(points[i].x),float(points[i].z))){
			//back
			for(int j=0; j<3; j++){
				squareMap->back->srcTriangles.push_back(points[i+j]);
			}
		}else if(isInside(0.0,0.0,squareMap->xStart,squareMap->zStart,squareMap->xStart,squareMap->zEnd,float(points[i].x),float(points[i].z))){
			//left
			for(int j=0; j<3; j++){
				squareMap->left->srcTriangles.push_back(points[i+j]);
			}
		}else{
			//right
			for(int j=0; j<3; j++){
				squareMap->right->srcTriangles.push_back(points[i+j]);
			}
		}
	}
	squareMap->front->srcBinSize = squareMap->front->srcTriangles.size()/3;
	squareMap->back->srcBinSize = squareMap->back->srcTriangles.size()/3;
	squareMap->left->srcBinSize = squareMap->left->srcTriangles.size()/3;
	squareMap->right->srcBinSize = squareMap->right->srcTriangles.size()/3;
}

void populateDestSquare(SquareMap* squareMap, std::vector< Angel::vec4 > points){
	for(size_t i=0; i<points.size();i+=3){
		if(isInside(0.0,0.0,squareMap->xStart,squareMap->zStart,squareMap->xEnd,squareMap->zStart,float(points[i].x),float(points[i].z))){
			//front
			for(int j=0; j<3; j++){
				squareMap->front->destTriangles.push_back(points[i+j]);
			}
		}else if(isInside(0.0,0.0,squareMap->xStart,squareMap->zEnd,squareMap->xEnd,squareMap->zEnd,float(points[i].x),float(points[i].z))){
			//back
			for(int j=0; j<3; j++){
				squareMap->back->destTriangles.push_back(points[i+j]);
			}
		}else if(isInside(0.0,0.0,squareMap->xStart,squareMap->zStart,squareMap->xStart,squareMap->zEnd,float(points[i].x),float(points[i].z))){
			//left
			for(int j=0; j<3; j++){
				squareMap->left->destTriangles.push_back(points[i+j]);
			}
		}else{
			//right
			for(int j=0; j<3; j++){
				squareMap->right->destTriangles.push_back(points[i+j]);
			}
		}
	}
	squareMap->front->destBinSize = squareMap->front->destTriangles.size()/3;
	squareMap->back->destBinSize = squareMap->back->destTriangles.size()/3;
	squareMap->left->destBinSize = squareMap->left->destTriangles.size()/3;
	squareMap->right->destBinSize = squareMap->right->destTriangles.size()/3;
}


float triangleArea(float x1, float y1, float x2, float y2, float x3, float y3){
   return std::abs((x1*(y2-y3) + x2*(y3-y1)+ x3*(y1-y2))/2.0);
}

bool isInside(float x1, float y1, float x2, float y2, float x3, float y3, float x, float y){
   float A = triangleArea (x1, y1, x2, y2, x3, y3);
   float A1 = triangleArea (x, y, x2, y2, x3, y3);
   float A2 = triangleArea (x1, y1, x, y, x3, y3);
   float A3 = triangleArea (x1, y1, x2, y2, x, y);
   return (A == A1 + A2 + A3);
}

void expandSquareMap(SquareMap* squareMap){
	expandMap(squareMap->front,squareMap->front->xStart,squareMap->front->xEnd,squareMap->front->yStart,squareMap->front->yEnd);
	expandMap(squareMap->back, squareMap->back->xStart,squareMap->back->xEnd,squareMap->back->yStart,squareMap->back->yEnd);
	expandMap(squareMap->left, squareMap->left->xStart,squareMap->left->xEnd,squareMap->left->yStart,squareMap->left->yEnd);
	expandMap(squareMap->right, squareMap->right->xStart,squareMap->right->xEnd,squareMap->right->yStart,squareMap->right->yEnd);
}

void expandMap(Square* square, float xStart, float xEnd, float yStart, float yEnd){
	if((square->destBinSize > MAXBIN)||(square->srcBinSize > MAXBIN)){
		for(size_t i=0; i<square->srcTriangles.size(); i+=3){
			if((float(square->srcTriangles[i].x) > square->xStart)&&
			   (float(square->srcTriangles[i].x) <= (square->xStart+(square->xEnd-square->xStart)/2))&&
			   (float(square->srcTriangles[i].y) > square->yStart)&&
			   (float(square->srcTriangles[i].y) <= (square->yStart+(square->yEnd-square->yStart)/2))){
				if(square->bottomLeft == NULL){
					square->bottomLeft = subdivideBottomLeft(square);
				}
				square->bottomLeft->srcTriangles.push_back(square->srcTriangles[i]);
				square->bottomLeft->srcTriangles.push_back(square->srcTriangles[i+1]);
				square->bottomLeft->srcTriangles.push_back(square->srcTriangles[i+2]);
				square->bottomLeft->srcBinSize++;

			}else if((float(square->srcTriangles[i].x) > square->xStart)&&
					(float(square->srcTriangles[i].x) <= (square->xStart+(square->xEnd-square->xStart)/2))&&
					(float(square->srcTriangles[i].y) > (square->yStart+(square->yEnd-square->yStart)/2))&&
					(float(square->srcTriangles[i].y) <= square->yEnd)){
				if(square->topLeft == NULL){
					square->topLeft = subdivideTopLeft(square);
				}
				square->topLeft->srcTriangles.push_back(square->srcTriangles[i]);
				square->topLeft->srcTriangles.push_back(square->srcTriangles[i+1]);
				square->topLeft->srcTriangles.push_back(square->srcTriangles[i+2]);
				square->topLeft->srcBinSize++;

			}else if((float(square->srcTriangles[i].x) > (square->xStart+(square->xEnd-square->xStart)/2))&&
					(float(square->srcTriangles[i].x) <= square->xEnd)&&
					(float(square->srcTriangles[i].y) > square->yStart)&&
					(float(square->srcTriangles[i].y) <= (square->yStart+(square->yEnd-square->yStart)/2))){
				if(square->bottomRight == NULL){
					square->bottomRight = subdivideBottomRight(square);
				}
				square->bottomRight->srcTriangles.push_back(square->srcTriangles[i]);
				square->bottomRight->srcTriangles.push_back(square->srcTriangles[i+1]);
				square->bottomRight->srcTriangles.push_back(square->srcTriangles[i+2]);
				square->bottomRight->srcBinSize++;
			}else{
				if(square->topRight == NULL){
					square->topRight = subdivideTopRight(square);
				}
				square->topRight->srcTriangles.push_back(square->srcTriangles[i]);
				square->topRight->srcTriangles.push_back(square->srcTriangles[i+1]);
				square->topRight->srcTriangles.push_back(square->srcTriangles[i+2]);
				square->bottomRight->srcBinSize++;
			}
		}
		for(size_t i=0; i<square->destTriangles.size(); i+=3){
			if((float(square->destTriangles[i].x) > square->xStart)&&
			   (float(square->destTriangles[i].x) <= (square->xStart+(square->xEnd-square->xStart)/2))&&
			   (float(square->destTriangles[i].y) > square->yStart)&&
			   (float(square->destTriangles[i].y) <= (square->yStart+(square->yEnd-square->yStart)/2))){
				if(square->bottomLeft == NULL){
					square->bottomLeft = subdivideBottomLeft(square);
				}
				square->bottomLeft->destTriangles.push_back(square->destTriangles[i]);
				square->bottomLeft->destTriangles.push_back(square->destTriangles[i+1]);
				square->bottomLeft->destTriangles.push_back(square->destTriangles[i+2]);
				square->bottomLeft->destBinSize++;

			}else if((float(square->destTriangles[i].x) > square->xStart)&&
					(float(square->destTriangles[i].x) <= (square->xStart+(square->xEnd-square->xStart)/2))&&
					(float(square->destTriangles[i].y) > (square->yStart+(square->yEnd-square->yStart)/2))&&
					(float(square->destTriangles[i].y) <= square->yEnd)){
				if(square->topLeft == NULL){
					square->topLeft = subdivideTopLeft(square);
				}
				square->topLeft->destTriangles.push_back(square->destTriangles[i]);
				square->topLeft->destTriangles.push_back(square->destTriangles[i+1]);
				square->topLeft->destTriangles.push_back(square->destTriangles[i+2]);
				square->topLeft->destBinSize++;

			}else if((float(square->destTriangles[i].x) > (square->xStart+(square->xEnd-square->xStart)/2))&&
					(float(square->destTriangles[i].x) <= square->xEnd)&&
					(float(square->destTriangles[i].y) > square->yStart)&&
					(float(square->destTriangles[i].y) <= (square->yStart+(square->yEnd-square->yStart)/2))){
				if(square->bottomRight == NULL){
					square->bottomRight = subdivideBottomRight(square);
				}
				square->bottomRight->destTriangles.push_back(square->destTriangles[i]);
				square->bottomRight->destTriangles.push_back(square->destTriangles[i+1]);
				square->bottomRight->destTriangles.push_back(square->destTriangles[i+2]);
				square->bottomRight->destBinSize++;
			}else{
				if(square->topRight == NULL){
					square->topRight = subdivideTopRight(square);
				}
				square->topRight->destTriangles.push_back(square->destTriangles[i]);
				square->topRight->destTriangles.push_back(square->destTriangles[i+1]);
				square->topRight->destTriangles.push_back(square->destTriangles[i+2]);
				square->topRight->destBinSize++;
			}
		}
		if(square->bottomLeft != NULL){
			expandMap(square->bottomLeft,square->bottomLeft->xStart,square->bottomLeft->xEnd,square->bottomLeft->yStart,square->bottomLeft->yEnd);
		}
		if(square->topLeft != NULL){
			expandMap(square->topLeft,square->topLeft->xStart,square->topLeft->xEnd,square->topLeft->yStart,square->topLeft->yEnd);
		}
		if(square->bottomRight != NULL){
			expandMap(square->bottomRight,square->bottomRight->xStart,square->bottomRight->xEnd,square->bottomRight->yStart,square->bottomRight->yEnd);
		}
		if(square->topRight != NULL){
			expandMap(square->topRight,square->topRight->xStart,square->topRight->xEnd,square->topRight->yStart,square->topRight->yEnd);
		}
	}else{
		return;
	}
}

Square* subdivideBottomLeft (Square* square){
	return new Square(square->xStart,square->xStart+(square->xEnd-square->xStart)/2,square->yStart,square->yStart+(square->yEnd-square->yStart)/2);
}

Square* subdivideTopLeft (Square* square){
	return new Square(square->xStart,square->xStart+(square->xEnd-square->xStart)/2,square->yStart+(square->yEnd-square->yStart)/2, square->yEnd);
}

Square* subdivideBottomRight (Square* square){
	return new Square(square->xStart+(square->xEnd-square->xStart)/2,square->xEnd,square->yStart,square->yStart+(square->yEnd-square->yStart)/2);
}

Square* subdivideTopRight (Square* square){
	return new Square(square->xStart+(square->xEnd-square->xStart)/2,square->xEnd,square->yStart+(square->yEnd-square->yStart)/2, square->yEnd);
}
