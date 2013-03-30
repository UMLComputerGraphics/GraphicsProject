/**
 * @file modelFunctions.cpp
 * @date 2013-03-15
 * @author Zach Maybury
 * @brief FIXME: Documentation needed from Zach.
 * @details Functions related to morphing.
 */

#include <vector>
#include "modelFunctions.hpp"
#include "Object.hpp"
#include <stdexcept>

float threeDimensionalDistance( vec4 points1, vec4 points2 ) {
  return sqrt(
      pow( (points1.x - points2.x), 2 ) + pow( (points1.y - points2.y), 2 )
      + pow( (points1.z - points2.z), 2 ) );
}

int findTriangleWithMinimumDistance( Object* largerModel, Object* smallerModel,
                                     int index ) {
  int minIndex = -1;
  float minDistance = INFINITY;
  
  for ( int i = 0; i < smallerModel->numberOfPoints(); i += 3 ) {
    float distance = 0.0;
    for ( int j = 0; j < 3; j++ ) {
      distance += threeDimensionalDistance( smallerModel->_vertices[i + j],
                                            largerModel->_vertices[index + j] );
    }
    if ( distance < minDistance ) {
      minDistance = distance;
      minIndex = i;
    }
  }
  
  if (minIndex >= 0) return minIndex;
  else throw std::logic_error( "Unable to identify the minimum index." );
}

float threeDimensionalDistanceFromCenter(vec4 points1, vec4 points2){
	return sqrt(pow((points1.x - points2.x),2) + pow((points1.y - points2.y),2) + pow((points1.z - points2.z),2));
}

vec4 findCenter(vec4 point1, vec4 point2, vec4 point3){
	int x,y,z,w;
	x = (point1.x + point2.x + point3.x)/3;
	y = (point1.y + point2.y + point3.y)/3;
	z = (point1.z + point2.z + point3.z)/3;
	w = (point1.w + point2.w + point3.w)/3;
	return vec4(x,y,z,w);
}

int findTriangleWithMinimumDistanceFromCenter(Object* largerModel, Object* smallerModel, int index){
  int minIndex = -1;
  float minDistance = INFINITY;

		for(int i=0; i<smallerModel->numberOfPoints();i+= 3){
			float distance = threeDimensionalDistanceFromCenter(findCenter(smallerModel->_vertices[i],smallerModel->_vertices[i+1],smallerModel->_vertices[i+2]),findCenter(largerModel->_vertices[index],largerModel->_vertices[index+1],largerModel->_vertices[index+2]));

			if(distance < minDistance){
				minDistance = distance;
				minIndex = i;
			}
		}

		return minIndex;
}

void makeModelsSameSize(Object* model1, Object* model2){
	if(model1->numberOfPoints()>model2->numberOfPoints()){
		while(model2->numberOfPoints() < (model1->numberOfPoints())){
			int index = findTriangleWithMinimumDistance(model1,model2,model2->numberOfPoints());
			for(int i=0; i<3; i++){
					model2->_vertices.push_back(vec4(model2->_vertices[index+i]));
					model2->_normals.push_back(vec3(model2->_normals[index+i]));
					model2->_colors.push_back(vec4(model2->_colors[index+i]));
			}
			printf("Size: %d",model2->numberOfPoints());
		}
	}else if(model1->numberOfPoints()<model2->numberOfPoints()){
		while(model1->numberOfPoints() < model2->numberOfPoints()){
			int index = findTriangleWithMinimumDistance(model2,model1,model1->numberOfPoints());
			for(int i=0; i<3; i++){
				model1->_vertices.push_back(vec4(model1->_vertices[index+i]));
				model1->_normals.push_back(vec3(model1->_normals[index+i]));
				model1->_colors.push_back(vec4(model1->_colors[index+i]));
			}
		}
	}else{
		printf("Models already the same size\n");
	}
}

void matchInitialPoints(Object* model1, Object* model2){
	if(model1->numberOfPoints()>model2->numberOfPoints()){
		for(int i=0; i<model2->numberOfPoints();i+=3){
			int matchIndex = 0;
			float minDistance = 9999999.0;
			for(int j=i; j<model1->numberOfPoints();j+=3){
				float distance = 0.0;
				for(int l=0; l<3; l++){
					distance += threeDimensionalDistance(model1->_vertices[j],model2->_vertices[i]);
				}
				if(distance<minDistance){
					matchIndex = j;
					minDistance = distance;
				}
			}
			for(int m=0; m<3; m++){
				vec4 tempPoints = model1->_vertices[i+m];
				vec3 tempNormals = model1->_normals[i+m];
				vec4 tempColors = model1->_colors[i+m];
				model1->_vertices[i+m] = model1->_vertices[matchIndex+m];
				model1->_normals[i+m] = model1->_normals[matchIndex+m];
				model1->_colors[i+m] = model1->_colors[matchIndex+m];
				model1->_vertices[matchIndex+m] = tempPoints;
				model1->_normals[matchIndex+m] = tempNormals;
				model1->_colors[matchIndex+m] = tempColors;
			}
		}
	}else{
		for(int i=0; i<model1->numberOfPoints();i+=3){
			int matchIndex = 0;
			float minDistance = 9999999.0;
			for(int j=i; j<model2->numberOfPoints();j+=3){
				float distance = 0.0;
				for(int l=0; l<3; l++){
					distance += threeDimensionalDistance(model2->_vertices[j],model1->_vertices[i]);
				}
				if(distance<minDistance){
					matchIndex = j;
					minDistance = distance;
				}
			}
			for(int m=0; m<3; m++){
				vec4 tempPoints = model2->_vertices[i+m];
				vec3 tempNormals = model2->_normals[i+m];
				vec4 tempColors = model2->_colors[i+m];
				model2->_vertices[i+m] = model2->_vertices[matchIndex+m];
				model2->_normals[i+m] = model2->_normals[matchIndex+m];
				model2->_colors[i+m] = model2->_colors[matchIndex+m];
				model2->_vertices[matchIndex+m] = tempPoints;
				model2->_normals[matchIndex+m] = tempNormals;
				model2->_colors[matchIndex+m] = tempColors;
			}
		}
	}
}
