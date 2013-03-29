#include <vector>
#include "modelFunctions.hpp"
#include "Object.hpp"

float threeDimensionalDistance(vec4 points1, vec4 points2){
	return sqrt(pow((points1.x - points2.x),2) + pow((points1.y - points2.y),2) + pow((points1.z - points2.z),2) + pow((points1.w - points2.w),2));
}

float threeDimensionalDistanceWithNormals(vec4 points1, vec3 normals1, vec4 points2, vec3 normals2){
	return sqrt(pow((normals1.x*points1.x - normals2.x*points2.x),2) + pow((normals1.y*points1.y - normals2.y*points2.y),2) + pow((normals1.z*points1.z - normals2.z*points2.z),2));
}

int findTriangleWithMinimumDistance(Object* largerModel, Object* smallerModel, int index){
	int minIndex;
	float minDistance = 9999999.0;

	for(int i=0; i<smallerModel->numberOfPoints();i+= 3){
		float distance = 0.0;
		for(int j=0; j<3; j++){
			distance += threeDimensionalDistanceWithNormals(smallerModel->_points[i+j],smallerModel->_normals[i+j],largerModel->_points[index+j],largerModel->_normals[index+j]);
		}
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
					model2->_points.push_back(vec4(model2->_points[index+i]));
					model2->_normals.push_back(vec3(model2->_normals[index+i]));
					model2->_colors.push_back(color4(model2->_colors[index+i]));
			}
			printf("Size: %d",model2->numberOfPoints());
		}
	}else if(model1->numberOfPoints()<model2->numberOfPoints()){
		while(model1->numberOfPoints() < model2->numberOfPoints()){
			int index = findTriangleWithMinimumDistance(model2,model1,model1->numberOfPoints());
			for(int i=0; i<3; i++){
				model1->_points.push_back(vec4(model1->_points[index+i]));
				model1->_normals.push_back(vec3(model1->_normals[index+i]));
				model1->_colors.push_back(color4(model1->_colors[index+i]));
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
					distance += threeDimensionalDistance(model1->_points[j],model2->_points[i]);
				}
				if(distance<minDistance){
					matchIndex = j;
					minDistance = distance;
				}
			}
			for(int m=0; m<3; m++){
				vec4 tempPoints = model1->_points[i+m];
				vec3 tempNormals = model1->_normals[i+m];
				vec4 tempColors = model1->_colors[i+m];
				model1->_points[i+m] = model1->_points[matchIndex+m];
				model1->_normals[i+m] = model1->_normals[matchIndex+m];
				model1->_colors[i+m] = model1->_colors[matchIndex+m];
				model1->_points[matchIndex+m] = tempPoints;
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
					distance += threeDimensionalDistance(model2->_points[j],model1->_points[i]);
				}
				if(distance<minDistance){
					matchIndex = j;
					minDistance = distance;
				}
			}
			for(int m=0; m<3; m++){
				vec4 tempPoints = model2->_points[i+m];
				vec3 tempNormals = model2->_normals[i+m];
				vec4 tempColors = model2->_colors[i+m];
				model2->_points[i+m] = model2->_points[matchIndex+m];
				model2->_normals[i+m] = model2->_normals[matchIndex+m];
				model2->_colors[i+m] = model2->_colors[matchIndex+m];
				model2->_points[matchIndex+m] = tempPoints;
				model2->_normals[matchIndex+m] = tempNormals;
				model2->_colors[matchIndex+m] = tempColors;
			}
		}
	}
}
