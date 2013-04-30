/**
 * @file modelFunctions.cpp
 * @date 2013-03-15
 * @author Zach Maybury
 * @brief FIXME: Documentation needed from Zach.
 * @details Functions related to morphing.
 */

#include "modelFunctions.hpp"
#include "Object.hpp"
#include "Util.hpp"
#include <stdexcept>
#include <algorithm>
#include <cfloat>
#include <vector>

float twoDimensionalDistance( vec4 points1, vec4 points2){
	return pow( (points1.x - points2.x), 2 ) + pow( (points1.y - points2.y), 2 );
}

float twoDimensionalDistance( vec4 points1, Angel::vec2 points2){
	return pow( (points1.x - points2.x), 2 ) + pow( (points1.y - points2.y), 2 );
}
float twoDimensionalXZDistance( vec4 points1, Angel::vec2 points2){
	return pow( (points1.x - points2.x), 2 ) + pow( (points1.y - points2.y), 2 );
}

float threeDimensionalDistance( vec4 points1, vec4 points2 ) {
  return
      pow( (points1.x - points2.x), 2 ) + pow( (points1.y - points2.y), 2 )
      + pow( (points1.z - points2.z), 2 );
}

int findTriangleWithMinimumDistance( Object* largerModel, Object* smallerModel,
                                     int index ) {
  int minIndex = -1;
  float minDistance = INFINITY;
  for ( size_t i = 0; i < smallerModel->numberOfPoints(); i += 3 ) {
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
	return pow((points1.x - points2.x),2) + pow((points1.y - points2.y),2) + pow((points1.z - points2.z),2);
}

float threeDimensionalDistanceWithOrigin(vec4 points1, vec4 points2){
	if(abs(points1.z) > abs(points2.z)){
		return pow((points1.x - points2.x),2) + pow((points1.y - points2.y),2) + pow((points1.z - points2.z),2) + threeDimensionalDistance(points2, vec4(0.0,0.0,0.0,0.0));
	}else{
		return pow((points1.x - points2.x),2) + pow((points1.y - points2.y),2) + pow((points1.z - points2.z),2) + threeDimensionalDistance(points1, vec4(0.0,0.0,0.0,0.0));
	} 
	
}

vec4 findCenter(vec4 point1, vec4 point2, vec4 point3){
	int x,y,z,w;
	x = (point1.x + point2.x + point3.x)/3;
	y = (point1.y + point2.y + point3.y)/3;
	z = (point1.z + point2.z + point3.z)/3;
	w = (point1.w + point2.w + point3.w)/3;
	return vec4(x,y,z,w);
}

void findOptimalOrientation(vec4 a, vec4 b, vec4 c, vec4 d, vec4 e, vec4 f, vec4& point1, vec4& point2, vec4& point3){
	//abc
	float distance1 = threeDimensionalDistance(a,d)+threeDimensionalDistance(b,e)+threeDimensionalDistance(c,f);
	//bca
	float distance4 = threeDimensionalDistance(b,d)+threeDimensionalDistance(c,e)+threeDimensionalDistance(a,f);
	//cab
	float distance5 = threeDimensionalDistance(c,d)+threeDimensionalDistance(a,e)+threeDimensionalDistance(b,f);
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

void findOptimalOrientation(vec4 a, vec4 b, vec4 c, std::vector< Angel::vec4 > model, int index, vec4& point1, vec4& point2, vec4& point3){
	//abc
	float distance1 = threeDimensionalDistance(a,model[index])+threeDimensionalDistance(b,model[index+1])+threeDimensionalDistance(c,model[index+2]);
	//bca
	float distance4 = threeDimensionalDistance(b,model[index])+threeDimensionalDistance(c,model[index+1])+threeDimensionalDistance(a,model[index+2]);
	//cab
	float distance5 = threeDimensionalDistance(c,model[index])+threeDimensionalDistance(a,model[index+1])+threeDimensionalDistance(b,model[index+2]);
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

void findOptimalOrientation(vec4 a, vec4 b, vec4 c, std::vector< Triangle* > model, int index, vec4& point1, vec4& point2, vec4& point3){
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

void findOptimalOrientation(vec4 a, vec4 b, vec4 c, Object* model, int index, vec4& point1, vec4& point2, vec4& point3){
	//abc
	float distance1 = threeDimensionalDistance(a,model->_vertices[index])+threeDimensionalDistance(b,model->_vertices[index+1])+threeDimensionalDistance(c,model->_vertices[index+2]);
	//bca
	float distance4 = threeDimensionalDistance(b,model->_vertices[index])+threeDimensionalDistance(c,model->_vertices[index+1])+threeDimensionalDistance(a,model->_vertices[index+2]);
	//cab
	float distance5 = threeDimensionalDistance(c,model->_vertices[index])+threeDimensionalDistance(a,model->_vertices[index+1])+threeDimensionalDistance(b,model->_vertices[index+2]);
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

void findOptimalOrientation(vec4 a, vec4 b, vec4 c, Object* model, int index, int& point1, int& point2, int& point3){
	//abc
	float distance1 = threeDimensionalDistance(a,model->_vertices[index])+threeDimensionalDistance(b,model->_vertices[index+1])+threeDimensionalDistance(c,model->_vertices[index+2]);
	//bca
	float distance4 = threeDimensionalDistance(b,model->_vertices[index])+threeDimensionalDistance(c,model->_vertices[index+1])+threeDimensionalDistance(a,model->_vertices[index+2]);
	//cab
	float distance5 = threeDimensionalDistance(c,model->_vertices[index])+threeDimensionalDistance(a,model->_vertices[index+1])+threeDimensionalDistance(b,model->_vertices[index+2]);
	float distances[] = {distance1,distance4,distance5};
	float minimum = *std::min_element(distances,distances+3);	
	if(minimum==distance1){
		point1=1;point2=2;point3=3;return;
	}else if(minimum==distance4){
		point1=2;point2=3;point3=1;return;
	}else{
		point1=3;point2=1;point3=2;return;
	}
}

int findTriangleWithMinimumDistanceFromCenter(std::vector< Angel::vec4 > largerPoints, std::vector< Angel::vec4 > smallerPoints, int index){
	int minIndex = -1;
    float minDistance = INFINITY;

		for(size_t i=0; i<smallerPoints.size();i+= 3){
			vec4 point1,point2,point3;
			findOptimalOrientation(smallerPoints[index],smallerPoints[index+1],smallerPoints[index+2],largerPoints, i, point1, point2, point3);
			float distance = threeDimensionalDistance(point1,largerPoints[i])+threeDimensionalDistance(point2,largerPoints[i+1])+threeDimensionalDistance(point3,largerPoints[i+2]);
	
			if(distance < minDistance){
				minDistance = distance;
				minIndex = i;
			}
		}

		return minIndex;
/*		
  int minIndex = -1;
  float minDistance = INFINITY;

		for(int i=0; i<smallerPoints.size();i+= 3){
			float distance = threeDimensionalDistanceFromCenter(findCenter(smallerPoints[i],smallerPoints[i+1],smallerPoints[i+2]),findCenter(largerPoints[index],largerPoints[index+1],largerPoints[index+2]));

			if(distance < minDistance){
				minDistance = distance;
				minIndex = i;
			}
		}

		return minIndex;
		
		if (minIndex >= 0) return minIndex;
  else throw std::logic_error( "Unable to identify the minimum index." );
  
  */
}


int findTriangleWithMinimumDistanceFromCenter(Object* largerModel, Object* smallerModel, int index){
  int minIndex = -1;
  float minDistance = INFINITY;

		for(size_t i=0; i<smallerModel->numberOfPoints();i+= 3){
			float distance = 0.0;
			for(int l=0; l<3; l++){
				distance += threeDimensionalDistanceWithOrigin(largerModel->_vertices[index+l],smallerModel->_vertices[i+l]);
			}
		
			//vec4 point1,point2,point3;
			//findOptimalOrientation(smallerModel->_vertices[index],smallerModel->_vertices[index+1],smallerModel->_vertices[index+2],largerModel, i, point1, point2, point3);
			//float distance = threeDimensionalDistance(point1,largerModel->_vertices[i])+threeDimensionalDistance(point2,largerModel->_vertices[i+1])+threeDimensionalDistance(point3,largerModel->_vertices[i+2]);
	
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
			int index = findTriangleWithMinimumDistanceFromCenter(model1,model2,model2->numberOfPoints());
			for(int i=0; i<3; i++){
					model2->_vertices.push_back(vec4(model2->_vertices[index+i]));
					model2->_normals.push_back(vec3(model2->_normals[index+i]));
					model2->_colors.push_back(vec4(model2->_colors[index+i]));
			}
		}
	}else if(model1->numberOfPoints()<model2->numberOfPoints()){
		while(model1->numberOfPoints() < model2->numberOfPoints()){
			int index = findTriangleWithMinimumDistanceFromCenter(model2,model1,model1->numberOfPoints());
			for(int i=0; i<3; i++){
				model1->_vertices.push_back(vec4(model1->_vertices[index+i]));
				model1->_normals.push_back(vec3(model1->_normals[index+i]));
				model1->_colors.push_back(vec4(model1->_colors[index+i]));
			}
		}
	}else{
	  gprint( PRINT_INFO, "Models are already the same size.\n" );
	}
}

void matchInitialPoints(Object* model1, Object* model2){
	if(model1->numberOfPoints()>model2->numberOfPoints()){
		for(size_t i=0; i<model2->numberOfPoints();i+=3){
			int matchIndex = 0;
			float minDistance = INFINITY;
			for(size_t j=0; j<model1->numberOfPoints();j+=3){
				//float distance = 0.0;
				//for(int l=0; l<3; l++){
				//	distance += threeDimensionalDistance(model1->_vertices[j+l],model2->_vertices[i+l]);
				//}
				//float distance = threeDimensionalDistanceFromCenter(findCenter(model1->_vertices[j],model1->_vertices[j+1],model1->_vertices[j+2]),findCenter(model2->_vertices[i],model2->_vertices[i+1],model2->_vertices[i+2]));
				vec4 point1,point2,point3;
				findOptimalOrientation(model2->_vertices[i],model2->_vertices[i+1],model2->_vertices[i+2],model1, j, point1, point2, point3);
				float distance = threeDimensionalDistance(point1,model1->_vertices[j])+threeDimensionalDistance(point2,model1->_vertices[j+1])+threeDimensionalDistance(point3,model1->_vertices[j+2]);
				
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
		//int correctlyMatchedPoints = 0;
		for(size_t i=0; i<model1->numberOfPoints();i+=3){
			//int bestPoint1,bestPoint2,bestPoint3;
			int matchIndex = 0;
			float minDistance = INFINITY;

			for(size_t j=0; j<model2->numberOfPoints();j+=3){
				//float distance = 0.0;
				//for(int l=0; l<3; l++){
				//	distance += threeDimensionalDistance(model2->_vertices[j+l],model1->_vertices[i+l]);
				//}
				//float distance = threeDimensionalDistanceFromCenter(findCenter(model2->_vertices[j],model2->_vertices[j+1],model2->_vertices[j+2]),findCenter(model1->_vertices[i],model1->_vertices[i+1],model1->_vertices[i+2]));
				vec4 point1,point2,point3;
				findOptimalOrientation(model1->_vertices[i],model1->_vertices[i+1],model1->_vertices[i+2],model2, j, point1, point2, point3);
				float distance = threeDimensionalDistance(point1,model2->_vertices[j])+threeDimensionalDistance(point2,model2->_vertices[j+1])+threeDimensionalDistance(point3,model2->_vertices[j+2]);
	
				if(distance<minDistance){
					matchIndex = j;
					minDistance = distance;
				}
			}
			//if((minDistance <= 0.3)&&(i != matchIndex)){
			//	gprint( PRINT_DEBUG, "Index: %d  Match Index: %d  Distance: %f\n",i,matchIndex,minDistance);
			//}
			//if((i == matchIndex)&&(minDistance <= 0.3)){
			//	correctlyMatchedPoints+=3;
			//}
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

void matchPoints(std::vector< Angel::vec4 >& model1Vertices,std::vector< Angel::vec3 >& model1Normals,std::vector< Angel::vec4 >& model1Colors,std::vector< Angel::vec2 >& model1Textures, std::vector< Angel::vec4 >& model2Vertices,std::vector< Angel::vec3 >& model2Normals,std::vector< Angel::vec4 >& model2Colors,std::vector< Angel::vec2 >& model2Textures){
	if(model1Vertices.size()>model2Vertices.size()){
		for(size_t i=0; i<model2Vertices.size();i+=3){
			int matchIndex = 0;
			float minDistance = INFINITY;
			for(size_t j=0; j<model1Vertices.size();j+=3){
				float distance = 0.0;
				for(int l=0; l<3; l++){
					distance += threeDimensionalDistanceWithOrigin(model1Vertices[j+l],model2Vertices[i+l]);
				}
				distance += abs(threeDimensionalDistance(model1Normals[j],model2Normals[i]));
				//float distance = threeDimensionalDistanceFromCenter(findCenter(model1Vertices[j],model1Vertices[j+1],model1Vertices[j+2]),findCenter(model2Vertices[i],model2Vertices[i+1],model2Vertices[i+2]));
				//vec4 point1,point2,point3;
				//findOptimalOrientation(model2Vertices[i],model2Vertices[i+1],model2Vertices[i+2],model1Vertices, j, point1, point2, point3);
				//float distance = threeDimensionalDistance(point1,model1Vertices[j])+threeDimensionalDistance(point2,model1Vertices[j+1])+threeDimensionalDistance(point3,model1Vertices[j+2]);
	
				
				if(distance<minDistance){
					matchIndex = j;
					minDistance = distance;
				}
			}
			for(int m=0; m<3; m++){
				vec4 tempPoints = model1Vertices[i+m];
				vec3 tempNormals = model1Normals[i+m];
				vec4 tempColors = model1Colors[i+m];
				model1Vertices[i+m] = model1Vertices[matchIndex+m];
				model1Normals[i+m] = model1Normals[matchIndex+m];
				model1Colors[i+m] = model1Colors[matchIndex+m];
				model1Vertices[matchIndex+m] = tempPoints;
				model1Normals[matchIndex+m] = tempNormals;
				model1Colors[matchIndex+m] = tempColors;
			}
		}
	}else{
		//int correctlyMatchedPoints = 0;
		for(size_t i=0; i<model1Vertices.size();i+=3){
			//int bestPoint1,bestPoint2,bestPoint3;
			int matchIndex = 0;
			float minDistance = INFINITY;
			for(size_t j=0; j<model2Vertices.size();j+=3){
				float distance = 0.0;
				for(int l=0; l<3; l++){
					distance += threeDimensionalDistanceWithOrigin(model2Vertices[j+l],model1Vertices[i+l]);
				}
				distance += abs(threeDimensionalDistance(model2Normals[j],model1Normals[i]));
				
				//float distance = threeDimensionalDistanceFromCenter(findCenter(model2Vertices[j],model2Vertices[j+1],model2Vertices[j+2]),findCenter(model1Vertices[i],model1Vertices[i+1],model1Vertices[i+2]));
				//vec4 point1,point2,point3;
				//findOptimalOrientation(model1Vertices[i],model1Vertices[i+1],model1Vertices[i+2],model2Vertices, j, point1, point2, point3);
				//float distance = threeDimensionalDistance(point1,model2Vertices[j])+threeDimensionalDistance(point2,model2Vertices[j+1])+threeDimensionalDistance(point3,model2Vertices[j+2]);
	
				if(distance<minDistance){
					matchIndex = j;
					minDistance = distance;
				}
			}
			//if((minDistance <= 0.3)&&(i != matchIndex)){
			//	gprint( PRINT_DEBUG, "Index: %d  Match Index: %d  Distance: %f\n",i,matchIndex,minDistance);
			//}
			//if((i == matchIndex)&&(minDistance <= 0.3)){
			//	correctlyMatchedPoints+=3;
			//}
			for(int m=0; m<3; m++){
				vec4 tempPoints = model2Vertices[i+m];
				vec3 tempNormals = model2Normals[i+m];
				vec4 tempColors = model2Colors[i+m];
				model2Vertices[i+m] = model2Vertices[matchIndex+m];
				model2Normals[i+m] = model2Normals[matchIndex+m];
				model2Colors[i+m] = model2Colors[matchIndex+m];
				model2Vertices[matchIndex+m] = tempPoints;
				model2Normals[matchIndex+m] = tempNormals;
				model2Colors[matchIndex+m] = tempColors;
			}
		}
	}
}

void segmentModels(Object* model1, vec3 model1Low, vec3 model1High, Object* model2, vec3 model2Low, vec3 model2High){
	size_t partitionSize = 6;
	std::vector< Angel::vec4 > model1Vertices[6];
	std::vector< Angel::vec3 > model1Normals[6];
	std::vector< Angel::vec4 > model1Colors[6];
	std::vector< Angel::vec2 > model1Textures[6];
	std::vector< Angel::vec4 > model2Vertices[6];
	std::vector< Angel::vec3 > model2Normals[6];
	std::vector< Angel::vec4 > model2Colors[6];
	std::vector< Angel::vec2 > model2Textures[6];
	Angel::vec3 model1Distance = Angel::vec3(model1High.x - model1Low.x, model1High.y - model1Low.y, model1High.z - model1Low.z);
	Angel::vec3 model2Distance = Angel::vec3(model2High.x - model2Low.x, model2High.y - model2Low.y, model2High.z - model2Low.z);

	//Model1
	for(size_t i=0; i<model1->numberOfPoints(); i+=3){
		for(size_t j=0; j<partitionSize; j++){
			if(j == partitionSize-1){
				if((model1->_vertices[i].y >= (model1Low.y)+(j*model1Distance.y)/partitionSize)&&(model1->_vertices[i].y <= (model1Low.y + ((j+1)*model1Distance.y)/partitionSize))){
					for(size_t k=0; k<3; k++){
						model1Vertices[j].push_back(model1->_vertices[i+k]);
						model1Normals[j].push_back(model1->_normals[i+k]);
						model1Colors[j].push_back(model1->_colors[i+k]);
					}
				}
			}else{
				if((model1->_vertices[i].y >= (model1Low.y)+(j*model1Distance.y)/partitionSize)&&(model1->_vertices[i].y < (model1Low.y + ((j+1)*model1Distance.y)/partitionSize))){
					for(size_t k=0; k<3; k++){
						model1Vertices[j].push_back(model1->_vertices[i+k]);
						model1Normals[j].push_back(model1->_normals[i+k]);
						model1Colors[j].push_back(model1->_colors[i+k]);
					}
				}
			}
		}
	}
	//Model2
	for(size_t i=0; i<model2->numberOfPoints(); i+=3){
		for(size_t j=0; j<partitionSize; j++){
			if(j == partitionSize-1){
				if((model2->_vertices[i].y >= (model2Low.y)+(j*model2Distance.y)/partitionSize)&&(model2->_vertices[i].y <= (model2Low.y + ((j+1)*model2Distance.y)/partitionSize))){
					for(size_t k=0; k<3; k++){
						model2Vertices[j].push_back(model2->_vertices[i+k]);
						model2Normals[j].push_back(model2->_normals[i+k]);
						model2Colors[j].push_back(model2->_colors[i+k]);
					}
				}
			}else{
				if((model2->_vertices[i].y >= (model2Low.y)+(j*model2Distance.y)/partitionSize)&&(model2->_vertices[i].y < (model2Low.y + ((j+1)*model2Distance.y)/partitionSize))){
					for(size_t k=0; k<3; k++){
						model2Vertices[j].push_back(model2->_vertices[i+k]);
						model2Normals[j].push_back(model2->_normals[i+k]);
						model2Colors[j].push_back(model2->_colors[i+k]);
					}
				}
			}
		}
	}
	
	//gprint( PRINT_DEBUG, "Bottle Points Model1: %d\n", model1Vertices[2].size());
	//gprint( PRINT_DEBUG, "Bottle Points Model2: %d\n", model2Vertices[2].size());
	for(size_t i=0; i<partitionSize; i++){
		//makeModelTopSameSize(model1Vertices[i],model1Normals[i],model1Colors[i],model1Textures[i],model2Vertices[i],model2Normals[i],model2Colors[i],model2Textures[i]);
		//matchPoints(model1Vertices[i],model1Normals[i],model1Colors[i],model1Textures[i],model2Vertices[i],model2Normals[i],model2Colors[i],model2Textures[i]);
		//ScaleModel * scaleModel = new ScaleModel(model1Vertices[i], model2Vertices[i],1,3,1);
		//BipartiteGraph * bipartiteGraph = new BipartiteGraph(model1Vertices[i],model1Normals[i],model1Colors[i],model1Textures[i],model2Vertices[i],model2Normals[i],model2Colors[i],model2Textures[i]);
		//scaleModel->restorePartitionModel();
	}
	applyToObjects(model1, model2, model1Vertices,model1Normals,model1Colors,model1Textures,model2Vertices,model2Normals,model2Colors,model2Textures,partitionSize);
	
}

int findTriangleWithMinimumDistance(std::vector< Triangle* > larger, std::vector< Triangle* > smaller, int index){
	int minIndex = -1;
    float minDistance = INFINITY;
	for(size_t i=0; i<smaller.size();i++){
		vec4 point1,point2,point3;
		findOptimalOrientation(smaller[i]->a->vertex,smaller[i]->b->vertex,smaller[i]->c->vertex,larger, index, point1, point2, point3);
		float distance = threeDimensionalDistance(point1,larger[i]->a->vertex)+threeDimensionalDistance(point2,larger[i]->b->vertex)+threeDimensionalDistance(point3,larger[i]->c->vertex);
		//float distance = threeDimensionalDistance(smaller[i]->a->vertex,larger[i]->a->vertex)+threeDimensionalDistance(smaller[index]->b->vertex,larger[i]->b->vertex)+threeDimensionalDistance(smaller[index]->c->vertex,larger[i]->c->vertex);
		if(distance < minDistance){
			minDistance = distance;
			minIndex = i;
		}
	}
	//std::cout << minIndex << std::endl;
	return minIndex;
}

void makeSameSize(std::vector<Triangle*>& model1, std::vector<Triangle*>& model2){
	if((model1.size()>model2.size())&&(model2.size() > 0)){
			while(model2.size() < (model1.size())){
				int index = findTriangleWithMinimumDistance(model1,model2,model2.size());
				model2.push_back(model2[index]);
			}
		}else if((model1.size()<model2.size())&&(model1.size() > 0)){
			while(model1.size() < model2.size()){
				int index = findTriangleWithMinimumDistance(model2,model1,model1.size());
				model1.push_back(model1[index]);
			}
		}else{
		  gprint( PRINT_DEBUG, "Models already the same size\n" );
		}
}

void makeModelTopSameSize(std::vector< Angel::vec4 >& model1Vertices,std::vector< Angel::vec3 >& model1Normals,std::vector< Angel::vec4 >& model1Colors,std::vector< Angel::vec2 >& model1Textures, std::vector< Angel::vec4 >& model2Vertices,std::vector< Angel::vec3 >& model2Normals,std::vector< Angel::vec4 >& model2Colors,std::vector< Angel::vec2 >& model2Textures){

  gprint( PRINT_DEBUG, "Bottle Points Model1: %lu\n", model1Vertices.size());
  gprint( PRINT_DEBUG, "Bottle Points Model2: %lu\n", model2Vertices.size());
	
	if((model1Vertices.size()>model2Vertices.size())&&(model2Vertices.size() > 0)){
		while(model2Vertices.size() < (model1Vertices.size())){
			int index = findTriangleWithMinimumDistanceFromCenter(model1Vertices,model2Vertices,model2Vertices.size());
			for(int i=0; i<3; i++){
					model2Vertices.push_back(model2Vertices[index+i]);
					model2Normals.push_back(model2Normals[index+i]);
					model2Colors.push_back(model2Colors[index+i]);
			}
		}
	}else if((model1Vertices.size()<model2Vertices.size())&&(model1Vertices.size() > 0)){
		while(model1Vertices.size() < model2Vertices.size()){
			int index = findTriangleWithMinimumDistanceFromCenter(model2Vertices,model1Vertices,model1Vertices.size());
			for(int i=0; i<3; i++){
				model1Vertices.push_back(model1Vertices[index+i]);
				model1Normals.push_back(model1Normals[index+i]);
				model1Colors.push_back(model1Colors[index+i]);
			}
		}
	}else{
	  gprint( PRINT_DEBUG, "Models already the same size\n" );
	}
	gprint( PRINT_DEBUG, "Bottle Points Model1: %lu\n", model1Vertices.size());
	gprint( PRINT_DEBUG, "Bottle Points Model2: %lu\n", model2Vertices.size());
}

void applyToObjects(Object* model1, Object* model2, std::vector< Angel::vec4 > model1Vertices[],std::vector< Angel::vec3 > model1Normals[],std::vector< Angel::vec4 > model1Colors[],std::vector< Angel::vec2 > model1Textures[],std::vector< Angel::vec4 > model2Vertices[],std::vector< Angel::vec3 > model2Normals[],std::vector< Angel::vec4 > model2Colors[],std::vector< Angel::vec2 > model2Textures[],size_t partitionSize){

	model1->_vertices = model1Vertices[0];
	model1->_normals = model1Normals[0];
	model1->_colors = model1Colors[0];
	
	model2->_vertices = model2Vertices[0];
	model2->_normals = model2Normals[0];
	model2->_colors = model2Colors[0];

	for(size_t j=1; j<partitionSize; j++){
		for(size_t i=0; i<model1Vertices[j].size(); i++){
			model1->_vertices.push_back(model1Vertices[j][i]);
			model1->_normals.push_back(model1Normals[j][i]);
			model1->_colors.push_back(model1Colors[j][i]);
		}
		for(size_t i=0; i<model2Vertices[j].size(); i++){
			model2->_vertices.push_back(model2Vertices[j][i]);
			model2->_normals.push_back(model2Normals[j][i]);
			model2->_colors.push_back(model2Colors[j][i]);
		}	
	}
}

void splitProblemTriangles(Object* model1, Object* model2){
	float minTriangleSizeModel1 = INFINITY;
	float maxTriangleSizeModel1 = 0.0;
	for(size_t i=0; i<model1->numberOfPoints(); i+=3){
		float size = getTriangleSize(model1->_vertices[i],model1->_vertices[i+1],model1->_vertices[i+2]);
		if(size > maxTriangleSizeModel1){
			maxTriangleSizeModel1 = size;
		}
		if(size < minTriangleSizeModel1){
			minTriangleSizeModel1 = size;
		}
	}
	
	float minTriangleSizeModel2 = INFINITY;
	float maxTriangleSizeModel2 = 0.0;
	for(size_t i=0; i<model2->numberOfPoints(); i+=3){
		float size = getTriangleSize(model2->_vertices[i],model2->_vertices[i+1],model2->_vertices[i+2]);
		if(size > maxTriangleSizeModel2){
			maxTriangleSizeModel2 = size;
		}
		if(size < minTriangleSizeModel2){
			minTriangleSizeModel2 = size;
		}
	}
	
	int size = model1->numberOfPoints();
	for(int i=0; i<size; i+=3){
		if( getTriangleSize(model1->_vertices[i],model1->_vertices[i+1],model1->_vertices[i+2]) > (minTriangleSizeModel1+maxTriangleSizeModel1)/2){
			divideTriangle(model1,i);
		}
	}
	
	size = model2->numberOfPoints();
	for(int i=0; i<size; i+=3){
		if( getTriangleSize(model2->_vertices[i],model2->_vertices[i+1],model2->_vertices[i+2]) > (minTriangleSizeModel2+maxTriangleSizeModel2)/2){
			divideTriangle(model2,i);
		}
	}
	
}

void divideTriangle(Object* model, int index){
	vec4 aVec = model->_vertices[index];
	vec4 bVec = model->_vertices[index+1];
	vec4 cVec = model->_vertices[index+2];
	vec3 Norm = model->_normals[index];
	vec4 aCol = model->_colors[index];
	vec4 bCol = model->_colors[index+1];
	vec4 cCol = model->_colors[index+2];
	
	vec4 MidPoint1 = vec4((bVec + cVec)/2.0);
	vec4 MidPoint1Col = vec4((bCol + cCol)/2.0);
	vec4 MidPoint2 = vec4((aVec + cVec)/2.0);
	vec4 MidPoint2Col = vec4((aCol + cCol)/2.0);
	
	//std::cout << aVec << bVec << cVec << std::endl;
	//std::cout << MidPoint1 << MidPoint2 << std::endl;
	
	model->_vertices[index+2] = MidPoint1;
	model->_colors[index+2] = MidPoint1Col;
	model->_normals[index+2] = Norm;
	
	model->_vertices.push_back(aVec);
	model->_normals.push_back(Norm);
	model->_colors.push_back(aCol);
	model->_vertices.push_back(MidPoint1);
	model->_normals.push_back(Norm);
	model->_colors.push_back(MidPoint1Col);
	model->_vertices.push_back(MidPoint2);
	model->_normals.push_back(Norm);
	model->_colors.push_back(MidPoint2Col);
	
	model->_vertices.push_back(MidPoint2);
	model->_normals.push_back(Norm);
	model->_colors.push_back(MidPoint2Col);
	model->_vertices.push_back(MidPoint1);
	model->_normals.push_back(Norm);
	model->_colors.push_back(MidPoint1Col);
	model->_vertices.push_back(cVec);
	model->_normals.push_back(Norm);
	model->_colors.push_back(cCol);
	
}

float getTriangleSize(vec4 a, vec4 b, vec4 c){
	return 0.5*(threeDimensionalDistance(a,b))*(threeDimensionalDistance(c,vec4((a.x+b.x)/2.0,(a.y+b.y)/2.0,(a.z+b.z)/2.0,(a.w+b.w)/2.0)));
}

