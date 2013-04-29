/**
 * @file scanMatching.cpp
 * @date 2013-04-23
 * @author Zach Maybury
 * @brief FIXME: Documentation from Zach
 * @details FIXME: Documentation from Zach
 */

#include "scanMatching.hpp"
#include "Object.hpp"
#include "Util.hpp"
#include <stdexcept>
#include <algorithm>
#include <cfloat>
#include <vector>

bool comparePoints(Point1 * a, Point1 * b){
	if(a->vertex.y>b->vertex.y){
		return true;
	}else if(a->vertex.x>b->vertex.x){
		return true;
	}else if(a->vertex.z>b->vertex.z){
		return true;
	}else{
		return false;
	}
}

Point1 Point1::operator= (Point1 param) {
  Point1 temp;
  temp.vertex = param.vertex;
  temp.normal = param.normal;
  temp.color = param.color;
  return (temp);
}


void ScanMatch::scanMatch(){
	sortTriangles(srcTriangles);
	sortTriangles(destTriangles);

	//Go from top to bottom in both models, after difference in triangles is greater than some number,
	//spawn triangles to make up the difference, then continue down height.
	scanFromTop();
}

void ScanMatch::scanQuarters(){
	sortTriangles(srcTriangles);
	sortTriangles(destTriangles);

	std::vector< Triangle* > srcQuadrantOne;
	std::vector< Triangle* > srcQuadrantTwo;
	std::vector< Triangle* > srcQuadrantThree;
	std::vector< Triangle* > srcQuadrantFour;

	std::vector< Triangle* > destQuadrantOne;
	std::vector< Triangle* > destQuadrantTwo;
	std::vector< Triangle* > destQuadrantThree;
	std::vector< Triangle* > destQuadrantFour;

	for(size_t i=0; i<srcTriangles.size();i++){
		if((srcTriangles[i]->a->vertex.x < widthScale/2.0)&&(srcTriangles[i]->a->vertex.z < depthScale/2.0)){
			srcQuadrantOne.push_back(srcTriangles[i]);
		}else if((srcTriangles[i]->a->vertex.x >= widthScale/2.0)&&(srcTriangles[i]->a->vertex.z < depthScale/2.0)){
			srcQuadrantTwo.push_back(srcTriangles[i]);
		}else if((srcTriangles[i]->a->vertex.x < widthScale/2.0)&&(srcTriangles[i]->a->vertex.z >= depthScale/2.0)){
			srcQuadrantThree.push_back(srcTriangles[i]);
		}else{
			srcQuadrantFour.push_back(srcTriangles[i]);
		}
	}

	for(size_t i=0; i<destTriangles.size();i++){
		if((destTriangles[i]->a->vertex.x < widthScale/2.0)&&(destTriangles[i]->a->vertex.z < depthScale/2.0)){
			destQuadrantOne.push_back(destTriangles[i]);
		}else if((destTriangles[i]->a->vertex.x >= widthScale/2.0)&&(destTriangles[i]->a->vertex.z < depthScale/2.0)){
			destQuadrantTwo.push_back(destTriangles[i]);
		}else if((destTriangles[i]->a->vertex.x < widthScale/2.0)&&(destTriangles[i]->a->vertex.z >= depthScale/2.0)){
			destQuadrantThree.push_back(destTriangles[i]);
		}else{
			destQuadrantFour.push_back(destTriangles[i]);
		}
	}

	srcTriangles.clear();
	destTriangles.clear();
	scanQuarterFromTop(srcQuadrantOne,destQuadrantOne);
	scanQuarterFromTop(srcQuadrantTwo,destQuadrantTwo);
	scanQuarterFromTop(srcQuadrantThree,destQuadrantThree);
	scanQuarterFromTop(srcQuadrantFour,destQuadrantFour);

}

void ScanMatch::scanQuarterFromTop(std::vector<Triangle*> source, std::vector<Triangle*> destination){
	float lastMatchEnd = float(heightScale)+.01;
	std::vector< Triangle* > srcPartTriangles;
	std::vector< Triangle* > destPartTriangles;
	std::vector< Triangle* > srcFinalTriangles;
	std::vector< Triangle* > destFinalTriangles;
	int differenceTolerance = 1;
	for(float i=lastMatchEnd; i>=0.6; i-=.001){
		//std::cout << i << std::endl;
		for(size_t j=0; j<source.size(); j++){
			//loop through triangles to see how many are in this segment
			if(((source[j]->a->vertex.y < lastMatchEnd)&&(source[j]->a->vertex.y > i)&&(source[j]->covered == false))){
				source[j]->covered = true;
				srcPartTriangles.push_back(source[j]);
			}
		}
		for(size_t j=0; j<destination.size(); j++){
			//loop through triangles to see how many are in this segment
			if(((destination[j]->a->vertex.y < lastMatchEnd)&&(destination[j]->a->vertex.y > i)&&(destination[j]->covered == false))){
				destination[j]->covered = true;
				destPartTriangles.push_back(destination[j]);
			}
		}
		if((abs(srcPartTriangles.size()-destPartTriangles.size()) > differenceTolerance)&&((srcPartTriangles.size() != 0)&&(destPartTriangles.size() != 0))){
			//std::cout << "Difference Threshold Achieved " << i << ": " << srcPartTriangles.size() << " " << destPartTriangles.size() << std::endl;
			//Make vertices in this range match (bipartite maybe with birthing)
			matchInitial(srcPartTriangles,destPartTriangles);
			makeSameSize(srcPartTriangles,destPartTriangles);
			//std::cout << "Difference Threshold Achieved " << i << ": " << srcPartTriangles.size() << " " << destPartTriangles.size() << std::endl << std::endl;
			addToTriangles(srcFinalTriangles,srcPartTriangles);
			addToTriangles(destFinalTriangles,destPartTriangles);
			lastMatchEnd = i;
			srcPartTriangles.clear();
			destPartTriangles.clear();
		}
	}
	//add bottoms
	for(size_t j=0; j<source.size(); j++){
		//loop through triangles to see how many are in this segment
		if(((source[j]->a->vertex.y < 0.6)&&(source[j]->covered == false))){
			source[j]->covered = true;
			srcPartTriangles.push_back(source[j]);
		}
	}
	for(size_t j=0; j<destination.size(); j++){
		//loop through triangles to see how many are in this segment
		if(((destination[j]->a->vertex.y < 0.6)&&(destination[j]->covered == false))){
			destination[j]->covered = true;
			destPartTriangles.push_back(destination[j]);
		}
	}

	matchInitial(srcPartTriangles,destPartTriangles);
	makeSameSize(srcPartTriangles,destPartTriangles);
	addToTriangles(srcFinalTriangles,srcPartTriangles);
	addToTriangles(destFinalTriangles,destPartTriangles);
	srcPartTriangles.clear();
	destPartTriangles.clear();

	addToTriangles(srcTriangles,srcFinalTriangles);
	addToTriangles(destTriangles,destFinalTriangles);
}

void ScanMatch::matchInitial(std::vector<Triangle*>& model1, std::vector<Triangle*>& model2){
	if(model1.size()>model2.size()){
		for(size_t i=0; i<model2.size();i++){
			int matchIndex = 0;
			float minDistance = INFINITY;
			for(size_t j=0; j<model1.size();j++){
				vec4 point1,point2,point3;
				findOptimalOrientation(model2[i]->a->vertex,model2[i]->b->vertex,model2[i]->c->vertex,model1, j, point1, point2, point3);
				float distance = threeDimensionalDistance(point1,model1[j]->a->vertex)+threeDimensionalDistance(point2,model1[j]->b->vertex)+threeDimensionalDistance(point3,model1[j]->c->vertex);

				if(distance<minDistance){
					matchIndex = j;
					minDistance = distance;
				}
			}
			Triangle* tempTriangle = model1[i];
			model1[i] = model1[matchIndex];
			model1[matchIndex] = tempTriangle;
		}
	}else{
		for(size_t i=0; i<model1.size();i++){
			int matchIndex = 0;
			float minDistance = INFINITY;
			for(size_t j=0; j<model2.size();j++){
				vec4 point1,point2,point3;
				findOptimalOrientation(model1[i]->a->vertex,model1[i]->b->vertex,model1[i]->c->vertex,model2, j, point1, point2, point3);
				float distance = threeDimensionalDistance(point1,model2[j]->a->vertex)+threeDimensionalDistance(point2,model2[j]->b->vertex)+threeDimensionalDistance(point3,model2[j]->c->vertex);

				if(distance<minDistance){
					matchIndex = j;
					minDistance = distance;
				}
			}
			Triangle* tempTriangle = model2[i];
			model2[i] = model2[matchIndex];
			model2[matchIndex] = tempTriangle;
		}
	}
}

void ScanMatch::scanFromTop(){
	//std::cout << "SrcY " << srcTriangles[0]->a->vertex.y << " DestY " << destTriangles[0]->a->vertex.y << std::endl;
	float lastMatchEnd = float(heightScale)+.01;
	std::vector< Triangle* > srcPartTriangles;
	std::vector< Triangle* > destPartTriangles;
	std::vector< Triangle* > srcFinalTriangles;
	std::vector< Triangle* > destFinalTriangles;
	int differenceTolerance = 1;
	for(float i=lastMatchEnd; i>=0.6; i-=.001){
		//std::cout << i << std::endl;
		for(size_t j=0; j<srcTriangles.size(); j++){
			//loop through triangles to see how many are in this segment
			if(((srcTriangles[j]->a->vertex.y < lastMatchEnd)&&(srcTriangles[j]->a->vertex.y > i)&&(srcTriangles[j]->covered == false))){
				srcTriangles[j]->covered = true;
				srcPartTriangles.push_back(srcTriangles[j]);
			}
		}
		for(size_t j=0; j<destTriangles.size(); j++){
			//loop through triangles to see how many are in this segment
			if(((destTriangles[j]->a->vertex.y < lastMatchEnd)&&(destTriangles[j]->a->vertex.y > i)&&(destTriangles[j]->covered == false))){
				destTriangles[j]->covered = true;
				destPartTriangles.push_back(destTriangles[j]);
			}
		}
		if((abs(srcPartTriangles.size()-destPartTriangles.size()) > differenceTolerance)&&((srcPartTriangles.size() != 0)&&(destPartTriangles.size() != 0))){
			//std::cout << "Difference Threshold Achieved " << i << ": " << srcPartTriangles.size() << " " << destPartTriangles.size() << std::endl;
			//Make vertices in this range match (bipartite maybe with birthing)
			matchInitial(srcPartTriangles,destPartTriangles);
			makeSameSize(srcPartTriangles,destPartTriangles);
			//std::cout << "Difference Threshold Achieved " << i << ": " << srcPartTriangles.size() << " " << destPartTriangles.size() << std::endl << std::endl;
			addToTriangles(srcFinalTriangles,srcPartTriangles);
			addToTriangles(destFinalTriangles,destPartTriangles);
			lastMatchEnd = i;
			srcPartTriangles.clear();
			destPartTriangles.clear();
		}
	}
	//add bottoms
	for(size_t j=0; j<srcTriangles.size(); j++){
		//loop through triangles to see how many are in this segment
		if(((srcTriangles[j]->a->vertex.y < 0.6)&&(srcTriangles[j]->covered == false))){
			srcTriangles[j]->covered = true;
			srcPartTriangles.push_back(srcTriangles[j]);
		}
	}
	for(size_t j=0; j<destTriangles.size(); j++){
		//loop through triangles to see how many are in this segment
		if(((destTriangles[j]->a->vertex.y < 0.6)&&(destTriangles[j]->covered == false))){
			destTriangles[j]->covered = true;
			destPartTriangles.push_back(destTriangles[j]);
		}
	}

	matchInitial(srcPartTriangles,destPartTriangles);
	makeSameSize(srcPartTriangles,destPartTriangles);
	addToTriangles(srcFinalTriangles,srcPartTriangles);
	addToTriangles(destFinalTriangles,destPartTriangles);
	srcPartTriangles.clear();
	destPartTriangles.clear();



	srcTriangles.clear();
	destTriangles.clear();
	srcTriangles = srcFinalTriangles;
	destTriangles = destFinalTriangles;
}

float ScanMatch::threeDimensionalDistance( vec4 points1, vec4 points2 ) {
  return
      pow( (points1.x - points2.x), 2 ) + pow( (points1.y - points2.y), 2 )
      + pow( (points1.z - points2.z), 2 );
}

void ScanMatch::findOptimalOrientation(vec4 a, vec4 b, vec4 c, std::vector< Triangle* > model, int index, vec4& point1, vec4& point2, vec4& point3){
	//abc
	float distance1 = threeDimensionalDistance(a,model[index]->a->vertex)+threeDimensionalDistance(b,model[index]->b->vertex)+threeDimensionalDistance(c,model[index]->c->vertex);
	//bca
	float distance4 = threeDimensionalDistance(b,model[index]->a->vertex)+threeDimensionalDistance(c,model[index]->b->vertex)+threeDimensionalDistance(a,model[index]->c->vertex);
	//cab
	float distance5 = threeDimensionalDistance(c,model[index]->a->vertex)+threeDimensionalDistance(a,model[index]->b->vertex)+threeDimensionalDistance(b,model[index]->c->vertex);
	float distances[] = {distance1,distance4,distance5};
	float minimum = *std::min_element(distances,distances+3);
	if(minimum==distance1){
		point1=a;point2=b;point3=c;return;
	}else if(minimum==distance4){
		point1=b;point2=c;point3=a;return;
	}else{
		point1=c;point2=a;point3=b;return;
	}
}

int ScanMatch::findTriangleWithMinimumDistance(std::vector< Triangle* > larger, std::vector< Triangle* > smaller, int index){
	int minIndex = -1;
    float minDistance = INFINITY;
	for(size_t i=0; i<smaller.size();i++){
		vec4 point1,point2,point3;
		findOptimalOrientation(smaller[i]->a->vertex,smaller[i]->b->vertex,smaller[i]->c->vertex,larger, index, point1, point2, point3);
		float distance = threeDimensionalDistance(point1,larger[index]->a->vertex)+threeDimensionalDistance(point2,larger[index]->b->vertex)+threeDimensionalDistance(point3,larger[index]->c->vertex);

		if(distance < minDistance){
			minDistance = distance;
			minIndex = i;
			//std::cout << minIndex << " " << minDistance << std::endl;
		}
	}
	//std::cout << minIndex << " " << minDistance << std::endl;
	return minIndex;
}

void ScanMatch::makeSameSize(std::vector<Triangle*>& model1, std::vector<Triangle*>& model2){
	if((model1.size()>model2.size())&&(model2.size() > 0)){
			while(model2.size() < (model1.size())){
				int index = findTriangleWithMinimumDistance(model1,model2,model2.size());
				model2.push_back(model2[index]);
				//find which edge is the match and birth from that edge
				index = model2.size()-1;

				float distanceEdge1 = threeDimensionalDistance(model1[index]->a->vertex,model2[index]->a->vertex)+threeDimensionalDistance(model1[index]->b->vertex,model2[index]->b->vertex)+threeDimensionalDistance(model1[index]->c->vertex,vec4(model2[index]->a->vertex.x/model2[index]->b->vertex.x,model2[index]->a->vertex.y/model2[index]->b->vertex.y,model2[index]->a->vertex.w/model2[index]->b->vertex.w));
				float distanceEdge2 = threeDimensionalDistance(model1[index]->b->vertex,model2[index]->b->vertex)+threeDimensionalDistance(model1[index]->c->vertex,model2[index]->c->vertex)+threeDimensionalDistance(model1[index]->c->vertex,vec4(model2[index]->b->vertex.x/model2[index]->c->vertex.x,model2[index]->b->vertex.y/model2[index]->c->vertex.y,model2[index]->b->vertex.w/model2[index]->c->vertex.w));
				float distanceEdge3 = threeDimensionalDistance(model1[index]->a->vertex,model2[index]->a->vertex)+threeDimensionalDistance(model1[index]->c->vertex,model2[index]->c->vertex)+threeDimensionalDistance(model1[index]->c->vertex,vec4(model2[index]->a->vertex.x/model2[index]->c->vertex.x,model2[index]->a->vertex.y/model2[index]->c->vertex.y,model2[index]->a->vertex.w/model2[index]->c->vertex.w));
				if(distanceEdge1 < distanceEdge2){
					if(distanceEdge1 < distanceEdge3){
						//edge1 match
						//Point1* temp = triangle->b;
						model2[index]->c = new Point1(model2[index]->b->vertex,model2[index]->b->normal,model2[index]->b->color);//new Point1(Angel::vec4(model2[index]->a->vertex.x/model2[index]->b->vertex.x,model2[index]->a->vertex.y/model2[index]->b->vertex.y,model2[index]->a->vertex.w/model2[index]->b->vertex.w),Angel::vec3(model2[index]->a->normal.x/model2[index]->b->normal.x,model2[index]->a->normal.y/model2[index]->b->normal.y,model2[index]->a->normal.z/model2[index]->b->normal.z),Angel::vec4(model2[index]->a->color.x/model2[index]->b->color.x,model2[index]->a->color.y/model2[index]->b->color.y,model2[index]->a->color.w/model2[index]->b->color.w));
						//model2[index]->c = temp;
					}else{
						//edge3 match
						model2[index]->b = new Point1(model2[index]->a->vertex,model2[index]->a->normal,model2[index]->a->color);
					}
				}else{
					if(distanceEdge3 < distanceEdge2){
						//edge3 match
						model2[index]->b = new Point1(model2[index]->a->vertex,model2[index]->a->normal,model2[index]->a->color);
					}else{
						//edge2 match
						model2[index]->a = new Point1(model2[index]->c->vertex,model2[index]->c->normal,model2[index]->c->color);
					}
				}
			}
		}else if((model1.size()<model2.size())&&(model1.size() > 0)){
			while(model1.size() < model2.size()){
				int index = findTriangleWithMinimumDistance(model2,model1,model1.size());
				model1.push_back(model1[index]);
				//find which edge is the match and birth from that edge

				index = model1.size()-1;

				float distanceEdge1 = threeDimensionalDistance(model2[index]->a->vertex,model1[index]->a->vertex)+threeDimensionalDistance(model2[index]->b->vertex,model1[index]->b->vertex)+threeDimensionalDistance(model2[index]->c->vertex,vec4(model1[index]->a->vertex.x/model1[index]->b->vertex.x,model1[index]->a->vertex.y/model1[index]->b->vertex.y,model1[index]->a->vertex.w/model1[index]->b->vertex.w));
				float distanceEdge2 = threeDimensionalDistance(model2[index]->b->vertex,model1[index]->b->vertex)+threeDimensionalDistance(model2[index]->c->vertex,model1[index]->c->vertex)+threeDimensionalDistance(model2[index]->c->vertex,vec4(model1[index]->b->vertex.x/model1[index]->c->vertex.x,model1[index]->b->vertex.y/model1[index]->c->vertex.y,model1[index]->b->vertex.w/model1[index]->c->vertex.w));
				float distanceEdge3 = threeDimensionalDistance(model2[index]->a->vertex,model1[index]->a->vertex)+threeDimensionalDistance(model2[index]->c->vertex,model1[index]->c->vertex)+threeDimensionalDistance(model2[index]->c->vertex,vec4(model1[index]->a->vertex.x/model1[index]->c->vertex.x,model1[index]->a->vertex.y/model1[index]->c->vertex.y,model1[index]->a->vertex.w/model1[index]->c->vertex.w));
				if(distanceEdge1 < distanceEdge2){
					if(distanceEdge1 < distanceEdge3){
						//edge1 match
						//Point1* temp = triangle->b;
						model1[index]->c = new Point1(model1[index]->b->vertex,model1[index]->b->normal,model1[index]->b->color);//new Point1(Angel::vec4(model2[index]->a->vertex.x/model2[index]->b->vertex.x,model2[index]->a->vertex.y/model2[index]->b->vertex.y,model2[index]->a->vertex.w/model2[index]->b->vertex.w),Angel::vec3(model2[index]->a->normal.x/model2[index]->b->normal.x,model2[index]->a->normal.y/model2[index]->b->normal.y,model2[index]->a->normal.z/model2[index]->b->normal.z),Angel::vec4(model2[index]->a->color.x/model2[index]->b->color.x,model2[index]->a->color.y/model2[index]->b->color.y,model2[index]->a->color.w/model2[index]->b->color.w));
						//model2[index]->c = temp;
					}else{
						//edge3 match
						model1[index]->b = new Point1(model1[index]->a->vertex,model1[index]->a->normal,model1[index]->a->color);
					}
				}else{
					if(distanceEdge3 < distanceEdge2){
						//edge3 match
						model1[index]->b = new Point1(model1[index]->a->vertex,model1[index]->a->normal,model1[index]->a->color);
					}else{
						//edge2 match
						model1[index]->a = new Point1(model1[index]->c->vertex,model1[index]->c->normal,model1[index]->c->color);
					}
				}
			}
		}else{
		  gprint( PRINT_DEBUG, "Models already the same size\n" );
		}
}

void ScanMatch::addToTriangles(std::vector< Triangle* >& parent, std::vector<Triangle*> addition){
	for(size_t i=0; i<addition.size(); i++){
		parent.push_back(addition[i]);
	}
}

void ScanMatch::sortTriangles(std::vector< Triangle* >& triangles){
	//std::cout << triangles.size() <<  std::endl;
	std::sort(triangles.begin(),triangles.end(),compareTriangles);
}

void ScanMatch::copyToBuffers(){
	copyBackToObjects(Src,srcTriangles);
	copyBackToObjects(Dest,destTriangles);
}

void ScanMatch::copyBackToObjects(Object* object,std::vector< Triangle* > triangles){
	std::vector< Angel::vec4 > tempVertices;
	std::vector< Angel::vec3 > tempNormals;
	std::vector< Angel::vec4 > tempColors;

	object->_vertices.clear();
	object->_colors.clear();
	object->_normals.clear();

	for(size_t i=0; i<triangles.size();i++){
		object->_vertices.push_back(triangles[i]->a->vertex);
		object->_normals.push_back(triangles[i]->a->normal);
		object->_colors.push_back(triangles[i]->a->color);
		object->_vertices.push_back(triangles[i]->b->vertex);
		object->_normals.push_back(triangles[i]->b->normal);
		object->_colors.push_back(triangles[i]->b->color);
		object->_vertices.push_back(triangles[i]->c->vertex);
		object->_normals.push_back(triangles[i]->c->normal);
		object->_colors.push_back(triangles[i]->c->color);
	}
}

bool compareTriangles(Triangle* a, Triangle* b){
	//if(comparePoints(a->a,b->a)){
	if(a->a->vertex.y > b->a->vertex.y){
		return true;
	}
	else{
		return false;
	}
}
