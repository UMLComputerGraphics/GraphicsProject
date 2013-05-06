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
	SquareVerticalMeshMap( std::vector<Angel::vec4 > &SrcVerts,
			       std::vector<Angel::vec3 > &SrcNorms,
			       std::vector<Angel::vec4 > &SrcColrs,
			       std::vector<Angel::vec4 > &DestVerts,
			       std::vector<Angel::vec3 > &DestNorms,
			       std::vector<Angel::vec4 > &DestColrs,
			       Angel::vec3 &srcMin,
			       Angel::vec3 &srcMax,
			       Angel::vec3 &dstMin,
			       Angel::vec3 &dstMax,
			       int xDivisions,
			       int yDivisions ){
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

struct RectangularMapping {
	SquareVerticalMeshMap *squareMap1;
	SquareVerticalMeshMap *squareMap2;
	SquareVerticalMeshMap *squareMap3;
	SquareVerticalMeshMap *squareMap4;
	SquareHorizontalMeshMap *squareMapTop;
	SquareHorizontalMeshMap *squareMapBottom;
    Object* Src;
    Object* Dst;
	std::vector<Angel::vec4> srcPrevVerts;
	std::vector<Angel::vec3> srcPrevNorms;
	std::vector<Angel::vec4> srcPrevColrs;
	std::vector<Angel::vec4> dstPrevVerts;
	std::vector<Angel::vec3> dstPrevNorms;
	std::vector<Angel::vec4> dstPrevColrs;

	RectangularMapping(){
		squareMapBottom = NULL;
		squareMapTop = NULL;
		squareMap1 = NULL;
		squareMap2 = NULL;
		squareMap3 = NULL;
		squareMap4 = NULL;
        Src = NULL;
        Dst = NULL;
	};

	RectangularMapping(Object* model1, Object* model2){
        Src = model1;
        Dst = model2;
		std::vector<Angel::vec4> vertsQ1SRC,colrsQ1SRC,vertsQ1DST,colrsQ1DST;
		std::vector<Angel::vec3> normsQ1SRC,normsQ1DST;
		std::vector<Angel::vec4> vertsQ2SRC,colrsQ2SRC,vertsQ2DST,colrsQ2DST;
		std::vector<Angel::vec3> normsQ2SRC,normsQ2DST;
		std::vector<Angel::vec4> vertsQ3SRC,colrsQ3SRC,vertsQ3DST,colrsQ3DST;
		std::vector<Angel::vec3> normsQ3SRC,normsQ3DST;
		std::vector<Angel::vec4> vertsQ4SRC,colrsQ4SRC,vertsQ4DST,colrsQ4DST;
		std::vector<Angel::vec3> normsQ4SRC,normsQ4DST;
		std::vector<Angel::vec4> vertsTopSRC,colrsTopSRC,vertsTopDST,colrsTopDST;
		std::vector<Angel::vec3> normsTopSRC,normsTopDST;
		std::vector<Angel::vec4> vertsBottomSRC,colrsBottomSRC,vertsBottomDST,colrsBottomDST;
		std::vector<Angel::vec3> normsBottomSRC,normsBottomDST;
		for(size_t i=0; i< model1->_vertices.size(); i+=3){
			if((model1->_vertices[i].x > 0.5)&&(model1->_vertices[i].z>0.5)&&(model1->_vertices[i].y > 0.0)&&(model1->_vertices[i].y < 10.0)){
				for(int m=0; m<3; m++){
					//model1->_vertices[i+m].z = 1;
					vertsQ1SRC.push_back(model1->_vertices[i+m]);
					normsQ1SRC.push_back(model1->_normals[i+m]);
					colrsQ1SRC.push_back(model1->_colors[i+m]);
				}
			}else if((model1->_vertices[i].x <= 0.5)&&(model1->_vertices[i].z>0.5)&&(model1->_vertices[i].y > 0.0)&&(model1->_vertices[i].y < 10.0)){
				for(int m=0; m<3; m++){
					//model1->_vertices[i+m].z = 1;
					vertsQ2SRC.push_back(model1->_vertices[i+m]);
					normsQ2SRC.push_back(model1->_normals[i+m]);
					colrsQ2SRC.push_back(model1->_colors[i+m]);
				}
			}else if((model1->_vertices[i].x > 0.5)&&(model1->_vertices[i].z<=0.5)&&(model1->_vertices[i].y > 0.0)&&(model1->_vertices[i].y < 10.0)){
				for(int m=0; m<3; m++){
					//model1->_vertices[i+m].z = 1;
					vertsQ3SRC.push_back(model1->_vertices[i+m]);
					normsQ3SRC.push_back(model1->_normals[i+m]);
					colrsQ3SRC.push_back(model1->_colors[i+m]);
				}
			}else if((model1->_vertices[i].x <= 0.5)&&(model1->_vertices[i].z<=0.5)&&(model1->_vertices[i].y > 0.0)&&(model1->_vertices[i].y < 10.0)){
				for(int m=0; m<3; m++){
					//model1->_vertices[i+m].z = 1;
					vertsQ4SRC.push_back(model1->_vertices[i+m]);
					normsQ4SRC.push_back(model1->_normals[i+m]);
					colrsQ4SRC.push_back(model1->_colors[i+m]);
				}
			}else if(model1->_vertices[i].y >=10.0){
				for(int m=0; m<3; m++){
					//model1->_vertices[i+m].z = 1;
					vertsTopSRC.push_back(model1->_vertices[i+m]);
					normsTopSRC.push_back(model1->_normals[i+m]);
					colrsTopSRC.push_back(model1->_colors[i+m]);
				}
			}else if(model1->_vertices[i].y <= 0.0){
				for(int m=0; m<3; m++){
					//model1->_vertices[i+m].z = 1;
					vertsBottomSRC.push_back(model1->_vertices[i+m]);
					normsBottomSRC.push_back(model1->_normals[i+m]);
					colrsBottomSRC.push_back(model1->_colors[i+m]);
				}
			}
		}
		for(size_t i=0; i< model2->_vertices.size(); i+=3){
			if((model2->_vertices[i].x > 0.5)&&(model2->_vertices[i].z >0.5)&&(model2->_vertices[i].y > 0.0)&&(model2->_vertices[i].y < 10.0)){
				for(int m=0; m<3; m++){
					//model2->_vertices[i+m].z = 1;
					vertsQ1DST.push_back(model2->_vertices[i+m]);
					normsQ1DST.push_back(model2->_normals[i+m]);
					colrsQ1DST.push_back(model2->_colors[i+m]);
				}
			}else if((model2->_vertices[i].x <= 0.5)&&(model2->_vertices[i].z >0.5)&&(model2->_vertices[i].y > 0.0)&&(model2->_vertices[i].y < 10.0)){
				for(int m=0; m<3; m++){
					//model2->_vertices[i+m].z = 1;
					vertsQ2DST.push_back(model2->_vertices[i+m]);
					normsQ2DST.push_back(model2->_normals[i+m]);
					colrsQ2DST.push_back(model2->_colors[i+m]);
				}
			}else if((model2->_vertices[i].x > 0.5)&&(model2->_vertices[i].z <=0.5)&&(model2->_vertices[i].y > 0.0)&&(model2->_vertices[i].y < 10.0)){
				for(int m=0; m<3; m++){
					//model2->_vertices[i+m].z = 1;
					vertsQ3DST.push_back(model2->_vertices[i+m]);
					normsQ3DST.push_back(model2->_normals[i+m]);
					colrsQ3DST.push_back(model2->_colors[i+m]);
				}
			}else if((model2->_vertices[i].x <= 0.5)&&(model2->_vertices[i].z <=0.5)&&(model2->_vertices[i].y > 0.0)&&(model2->_vertices[i].y < 10.0)){
				for(int m=0; m<3; m++){
					//model2->_vertices[i+m].z = 1;
					vertsQ4DST.push_back(model2->_vertices[i+m]);
					normsQ4DST.push_back(model2->_normals[i+m]);
					colrsQ4DST.push_back(model2->_colors[i+m]);
				}
			}else if(model2->_vertices[i].y >= 10.0){
				for(int m=0; m<3; m++){
					//model2->_vertices[i+m].z = 1;
					vertsTopDST.push_back(model2->_vertices[i+m]);
					normsTopDST.push_back(model2->_normals[i+m]);
					colrsTopDST.push_back(model2->_colors[i+m]);
				}
			}else if(model2->_vertices[i].y <= 0.0){
				for(int m=0; m<3; m++){
					//model2->_vertices[i+m].z = 1;
					vertsBottomDST.push_back(model2->_vertices[i+m]);
					normsBottomDST.push_back(model2->_normals[i+m]);
					colrsBottomDST.push_back(model2->_colors[i+m]);
				}
			}
		}

		Angel::vec3 lowBoundSrc = getMin(vertsQ1SRC);
		Angel::vec3 maxBoundSrc = getMax(vertsQ1SRC);
		Angel::vec3 lowBoundDst = getMin(vertsQ1DST);
		Angel::vec3 maxBoundDst = getMax(vertsQ1DST);
		//create mesh to match to both models
		int meshHeight = 50;
		int meshWidth = 50;
		squareMap1 = new SquareVerticalMeshMap(vertsQ1SRC,normsQ1SRC,colrsQ1SRC,vertsQ1DST,normsQ1DST,colrsQ1DST,lowBoundSrc,maxBoundSrc,lowBoundDst,maxBoundDst,meshWidth,meshHeight);

		lowBoundSrc = getMin(vertsQ2SRC);
		maxBoundSrc = getMax(vertsQ2SRC);
		lowBoundDst = getMin(vertsQ2DST);
		maxBoundDst = getMax(vertsQ2DST);
		//create mesh to match to both models
		meshHeight = 50;
		meshWidth = 50;
		squareMap2 = new SquareVerticalMeshMap(vertsQ2SRC,normsQ2SRC,colrsQ2SRC,vertsQ2DST,normsQ2DST,colrsQ2DST,lowBoundSrc,maxBoundSrc,lowBoundDst,maxBoundDst,meshWidth,meshHeight);

		lowBoundSrc = getMin(vertsQ3SRC);
		maxBoundSrc = getMax(vertsQ3SRC);
		lowBoundDst = getMin(vertsQ3DST);
		maxBoundDst = getMax(vertsQ3DST);
		//create mesh to match to both models
		meshHeight = 50;
		meshWidth = 50;
		squareMap3 = new SquareVerticalMeshMap(vertsQ3SRC,normsQ3SRC,colrsQ3SRC,vertsQ3DST,normsQ3DST,colrsQ3DST,lowBoundSrc,maxBoundSrc,lowBoundDst,maxBoundDst,meshWidth,meshHeight);

		lowBoundSrc = getMin(vertsQ4SRC);
		maxBoundSrc = getMax(vertsQ4SRC);
		lowBoundDst = getMin(vertsQ4DST);
		maxBoundDst = getMax(vertsQ4DST);
		//create mesh to match to both models
		meshHeight = 50;
		meshWidth = 50;
		squareMap4 = new SquareVerticalMeshMap(vertsQ4SRC,normsQ4SRC,colrsQ4SRC,vertsQ4DST,normsQ4DST,colrsQ4DST,lowBoundSrc,maxBoundSrc,lowBoundDst,maxBoundDst,meshWidth,meshHeight);

		lowBoundSrc = getMin(vertsTopSRC);
		maxBoundSrc = getMax(vertsTopSRC);
		lowBoundDst = getMin(vertsTopDST);
		maxBoundDst = getMax(vertsTopDST);
		//create mesh to match to both models
		meshHeight = 25;
		meshWidth = 25;
		//need modified square mest for x/z plane, instead of x/y one currently
		squareMapTop = new SquareHorizontalMeshMap(vertsTopSRC,normsTopSRC,colrsTopSRC,vertsTopDST,normsTopDST,colrsTopDST,lowBoundSrc,maxBoundSrc,lowBoundDst,maxBoundDst,meshWidth,meshHeight);

		lowBoundSrc = getMin(vertsBottomSRC);
		maxBoundSrc = getMax(vertsBottomSRC);
		lowBoundDst = getMin(vertsBottomDST);
		maxBoundDst = getMax(vertsBottomDST);
		//create mesh to match to both models
		meshHeight = 25;
		meshWidth = 25;
		//need modified square mest for x/z plane, instead of x/y one currently
		squareMapBottom = new SquareHorizontalMeshMap(vertsBottomSRC,normsBottomSRC,colrsBottomSRC,vertsBottomDST,normsBottomDST,colrsBottomDST,lowBoundSrc,maxBoundSrc,lowBoundDst,maxBoundDst,meshWidth,meshHeight);


	}
    void copyToObjects(Object* model1, Object* model2);
    void revertToOriginal(Object* model1, Object* model2);
	Angel::vec3 getMin(std::vector<Angel::vec4> points);
	Angel::vec3 getMax(std::vector<Angel::vec4> points);
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
