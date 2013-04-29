/**
 * @file nearestneighbor.cpp
 * @date 2013-04-26
 * @author Zach Maybury
 * @brief FIXME: Documentation needed from Zach.
 * @details Headers for functions related to morphing.
 */

#include "nearestNeighbor.hpp"
#include "modelFunctions.hpp"

void SquareVerticalMeshMap::sortTriangles(std::vector< Triangle* >& triangles){
	//std::cout << triangles.size() <<  std::endl;
	std::sort(triangles.begin(),triangles.end(),compareSquareTriangles);
}
void SquareHorizontalMeshMap::sortTriangles(std::vector< Triangle* >& triangles){
	//std::cout << triangles.size() <<  std::endl;
	std::sort(triangles.begin(),triangles.end(),compareSquareTriangles);
}

void rectangularMapping(Object* model1, Object* model2){
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
/*
	//model1->_vertices.clear();
	//model1->_normals.clear();
	//model1->_colors.clear();
	//model1->_vertices = verts1;
	//model1->_normals = norms1;
	//model1->_colors = colrs1;

	//model2->_vertices.clear();
	//model2->_normals.clear();
	//model2->_colors.clear();
	//model2->_vertices = verts2;
	//model2->_normals = norms2;
	//model2->_colors = colrs2;
*/
	Angel::vec3 lowBoundSrc = getMin(vertsQ1SRC);
	Angel::vec3 maxBoundSrc = getMax(vertsQ1SRC);
	Angel::vec3 lowBoundDst = getMin(vertsQ1DST);
	Angel::vec3 maxBoundDst = getMax(vertsQ1DST);
	//create mesh to match to both models
	int meshHeight = 50;
	int meshWidth = 50;
	SquareVerticalMeshMap *squareMap1 = new SquareVerticalMeshMap(vertsQ1SRC,normsQ1SRC,colrsQ1SRC,vertsQ1DST,normsQ1DST,colrsQ1DST,lowBoundSrc,maxBoundSrc,lowBoundDst,maxBoundDst,meshWidth,meshHeight);

	lowBoundSrc = getMin(vertsQ2SRC);
	maxBoundSrc = getMax(vertsQ2SRC);
	lowBoundDst = getMin(vertsQ2DST);
	maxBoundDst = getMax(vertsQ2DST);
	//create mesh to match to both models
	meshHeight = 50;
	meshWidth = 50;
	SquareVerticalMeshMap *squareMap2 = new SquareVerticalMeshMap(vertsQ2SRC,normsQ2SRC,colrsQ2SRC,vertsQ2DST,normsQ2DST,colrsQ2DST,lowBoundSrc,maxBoundSrc,lowBoundDst,maxBoundDst,meshWidth,meshHeight);

	lowBoundSrc = getMin(vertsQ3SRC);
	maxBoundSrc = getMax(vertsQ3SRC);
	lowBoundDst = getMin(vertsQ3DST);
	maxBoundDst = getMax(vertsQ3DST);
	//create mesh to match to both models
	meshHeight = 50;
	meshWidth = 50;
	SquareVerticalMeshMap *squareMap3 = new SquareVerticalMeshMap(vertsQ3SRC,normsQ3SRC,colrsQ3SRC,vertsQ3DST,normsQ3DST,colrsQ3DST,lowBoundSrc,maxBoundSrc,lowBoundDst,maxBoundDst,meshWidth,meshHeight);

	lowBoundSrc = getMin(vertsQ4SRC);
	maxBoundSrc = getMax(vertsQ4SRC);
	lowBoundDst = getMin(vertsQ4DST);
	maxBoundDst = getMax(vertsQ4DST);
	//create mesh to match to both models
	meshHeight = 50;
	meshWidth = 50;
	SquareVerticalMeshMap *squareMap4 = new SquareVerticalMeshMap(vertsQ4SRC,normsQ4SRC,colrsQ4SRC,vertsQ4DST,normsQ4DST,colrsQ4DST,lowBoundSrc,maxBoundSrc,lowBoundDst,maxBoundDst,meshWidth,meshHeight);

	lowBoundSrc = getMin(vertsTopSRC);
	maxBoundSrc = getMax(vertsTopSRC);
	lowBoundDst = getMin(vertsTopDST);
	maxBoundDst = getMax(vertsTopDST);
	//create mesh to match to both models
	meshHeight = 25;
	meshWidth = 25;
	//need modified square mest for x/z plane, instead of x/y one currently
	SquareHorizontalMeshMap *squareMapTop = new SquareHorizontalMeshMap(vertsTopSRC,normsTopSRC,colrsTopSRC,vertsTopDST,normsTopDST,colrsTopDST,lowBoundSrc,maxBoundSrc,lowBoundDst,maxBoundDst,meshWidth,meshHeight);

	lowBoundSrc = getMin(vertsBottomSRC);
	maxBoundSrc = getMax(vertsBottomSRC);
	lowBoundDst = getMin(vertsBottomDST);
	maxBoundDst = getMax(vertsBottomDST);
	//create mesh to match to both models
	meshHeight = 25;
	meshWidth = 25;
	//need modified square mest for x/z plane, instead of x/y one currently
	SquareHorizontalMeshMap *squareMapBottom = new SquareHorizontalMeshMap(vertsBottomSRC,normsBottomSRC,colrsBottomSRC,vertsBottomDST,normsBottomDST,colrsBottomDST,lowBoundSrc,maxBoundSrc,lowBoundDst,maxBoundDst,meshWidth,meshHeight);


	model1->_vertices.clear();
	model1->_colors.clear();
	model1->_normals.clear();
	model2->_vertices.clear();
	model2->_colors.clear();
	model2->_normals.clear();

	copyToVector(model1->_vertices,squareMap1->finalSrcVerts);
	copyToVector(model1->_colors,squareMap1->finalSrcColrs);
	copyToVector(model1->_normals,squareMap1->finalSrcNorms);
	copyToVector(model2->_vertices,squareMap1->finalDstVerts);
	copyToVector(model2->_colors,squareMap1->finalDstColrs);
	copyToVector(model2->_normals,squareMap1->finalDstNorms);

	copyToVector(model1->_vertices,squareMap2->finalSrcVerts);
	copyToVector(model1->_colors,squareMap2->finalSrcColrs);
	copyToVector(model1->_normals,squareMap2->finalSrcNorms);
	copyToVector(model2->_vertices,squareMap2->finalDstVerts);
	copyToVector(model2->_colors,squareMap2->finalDstColrs);
	copyToVector(model2->_normals,squareMap2->finalDstNorms);

	copyToVector(model1->_vertices,squareMap3->finalSrcVerts);
	copyToVector(model1->_colors,squareMap3->finalSrcColrs);
	copyToVector(model1->_normals,squareMap3->finalSrcNorms);
	copyToVector(model2->_vertices,squareMap3->finalDstVerts);
	copyToVector(model2->_colors,squareMap3->finalDstColrs);
	copyToVector(model2->_normals,squareMap3->finalDstNorms);

	copyToVector(model1->_vertices,squareMap4->finalSrcVerts);
	copyToVector(model1->_colors,squareMap4->finalSrcColrs);
	copyToVector(model1->_normals,squareMap4->finalSrcNorms);
	copyToVector(model2->_vertices,squareMap4->finalDstVerts);
	copyToVector(model2->_colors,squareMap4->finalDstColrs);
	copyToVector(model2->_normals,squareMap4->finalDstNorms);

	copyToVector(model1->_vertices,squareMapTop->finalSrcVerts);
	copyToVector(model1->_colors,squareMapTop->finalSrcColrs);
	copyToVector(model1->_normals,squareMapTop->finalSrcNorms);
	copyToVector(model2->_vertices,squareMapTop->finalDstVerts);
	copyToVector(model2->_colors,squareMapTop->finalDstColrs);
	copyToVector(model2->_normals,squareMapTop->finalDstNorms);

	copyToVector(model1->_vertices,squareMapBottom->finalSrcVerts);
	copyToVector(model1->_colors,squareMapBottom->finalSrcColrs);
	copyToVector(model1->_normals,squareMapBottom->finalSrcNorms);
	copyToVector(model2->_vertices,squareMapBottom->finalDstVerts);
	copyToVector(model2->_colors,squareMapBottom->finalDstColrs);
	copyToVector(model2->_normals,squareMapBottom->finalDstNorms);

	/*//lowBoundSrc = model1->getMin();
	//maxBoundSrc = model1->getMax();
	//lowBoundDst = model2->getMin();
	//maxBoundDst = model2->getMax();
	//std::cout << "Model1 Bounds: " << lowBoundSrc << " " << maxBoundSrc << std::endl;
	//std::cout << "Model2 Bounds: " << lowBoundDst << " " << maxBoundDst << std::endl;*/
}

void copyToVector(std::vector< Angel::vec4 >& dest, std::vector< Angel::vec4 > src){
	for(size_t i=0; i<src.size(); i++){
		dest.push_back(src[i]);
	}
}
void copyToVector(std::vector< Angel::vec3 >& dest, std::vector< Angel::vec3 > src){
	for(size_t i=0; i<src.size(); i++){
		dest.push_back(src[i]);
	}
}

void nearestNeighbor(Object* model1, Object* model2){
	std::vector<Angel::vec4> verts1;
	std::vector<Angel::vec3> norms1;
	std::vector<Angel::vec4> colrs1;
	std::vector<Angel::vec4> verts2;
	std::vector<Angel::vec3> norms2;
	std::vector<Angel::vec4> colrs2;
	if(model1->numberOfPoints()>model2->numberOfPoints()){
		for(size_t i=0; i<model1->numberOfPoints(); i+=3){
			int matchIndex = 0;
			float minDistance = INFINITY;
			for(size_t j=0; j<model2->numberOfPoints();j+=3){
				vec4 point1,point2,point3;
				findOptimalOrientation(model2->_vertices[i],model2->_vertices[i+1],model2->_vertices[i+2],model1, i, point1, point2, point3);
				float distance = threeDimensionalDistance(point1,model1->_vertices[j])+threeDimensionalDistance(point2,model1->_vertices[j+1])+threeDimensionalDistance(point3,model1->_vertices[j+2]);

				if(distance<minDistance){
					matchIndex = j;
					minDistance = distance;
				}
			}
			for(int m=0; m<3; m++){
				verts1.push_back(model2->_vertices[matchIndex+m]);
				norms1.push_back(model2->_normals[matchIndex+m]);
				colrs1.push_back(model2->_colors[matchIndex+m]);
			}
		}
		for(size_t i=0; i<model2->numberOfPoints(); i+=3){
			int matchIndex = 0;
			float minDistance = INFINITY;
			for(size_t j=0; j<model1->numberOfPoints();j+=3){
				vec4 point1,point2,point3;
				findOptimalOrientation(model1->_vertices[i],model1->_vertices[i+1],model1->_vertices[i+2],model2, i, point1, point2, point3);
				float distance = threeDimensionalDistance(point1,model2->_vertices[j])+threeDimensionalDistance(point2,model2->_vertices[j+1])+threeDimensionalDistance(point3,model2->_vertices[j+2]);

				if(distance<minDistance){
					matchIndex = j;
					minDistance = distance;
				}
			}
			for(int m=0; m<3; m++){
				verts1.push_back(model2->_vertices[i+m]);
				norms1.push_back(model2->_normals[i+m]);
				colrs1.push_back(model2->_colors[i+m]);
				verts2.push_back(model1->_vertices[matchIndex+m]);
				norms2.push_back(model1->_normals[matchIndex+m]);
				colrs2.push_back(model1->_colors[matchIndex+m]);
			}
		}
		model2->_vertices.clear();
		model2->_normals.clear();
		model2->_colors.clear();
		model2->_vertices = verts1;
		model2->_normals = norms1;
		model2->_colors = colrs1;
		for(size_t i=0; i<verts2.size();i++){
			model1->_vertices.push_back(verts2[i]);
			model1->_normals.push_back(norms2[i]);
			model1->_colors.push_back(colrs2[i]);
		}

	}else{
		for(size_t i=0; i<model2->numberOfPoints(); i+=3){
			int matchIndex = 0;
			float minDistance = INFINITY;
			for(size_t j=0; j<model1->numberOfPoints();j+=3){
				vec4 point1,point2,point3;
				findOptimalOrientation(model1->_vertices[i],model1->_vertices[i+1],model1->_vertices[i+2],model2, i, point1, point2, point3);
				float distance = threeDimensionalDistance(point1,model2->_vertices[j])+threeDimensionalDistance(point2,model2->_vertices[j+1])+threeDimensionalDistance(point3,model2->_vertices[j+2]);

				if(distance<minDistance){
					matchIndex = j;
					minDistance = distance;
				}
			}
			for(int m=0; m<3; m++){
				verts1.push_back(model1->_vertices[matchIndex+m]);
				norms1.push_back(model1->_normals[matchIndex+m]);
				colrs1.push_back(model1->_colors[matchIndex+m]);
			}
		}
		for(size_t i=0; i<model1->numberOfPoints(); i+=3){
			int matchIndex = 0;
			float minDistance = INFINITY;
			for(size_t j=0; j<model2->numberOfPoints();j+=3){
				vec4 point1,point2,point3;
				findOptimalOrientation(model2->_vertices[i],model2->_vertices[i+1],model2->_vertices[i+2],model1, i, point1, point2, point3);
				float distance = threeDimensionalDistance(point1,model1->_vertices[j])+threeDimensionalDistance(point2,model1->_vertices[j+1])+threeDimensionalDistance(point3,model1->_vertices[j+2]);

				if(distance<minDistance){
					matchIndex = j;
					minDistance = distance;
				}
			}
			for(int m=0; m<3; m++){
				verts1.push_back(model1->_vertices[i+m]);
				norms1.push_back(model1->_normals[i+m]);
				colrs1.push_back(model1->_colors[i+m]);
				verts2.push_back(model2->_vertices[matchIndex+m]);
				norms2.push_back(model2->_normals[matchIndex+m]);
				colrs2.push_back(model2->_colors[matchIndex+m]);
			}
		}
		model1->_vertices.clear();
		model1->_normals.clear();
		model1->_colors.clear();
		model1->_vertices = verts1;
		model1->_normals = norms1;
		model1->_colors = colrs1;

		for(size_t i=0; i<verts2.size();i++){
			model2->_vertices.push_back(verts2[i]);
			model2->_normals.push_back(norms2[i]);
			model2->_colors.push_back(colrs2[i]);
		}
	}
}

void SquareVerticalMeshMap::copyToBuffers(){
	//copyBackToObjects(Src,Dest,squares);
}

void SquareHorizontalMeshMap::copyToFinal(){
	//Handle Mesh Maps
	for(size_t i=0; i<squares.size();i++){
		vec4 point1,point2,point3;
		findOptimalOrientation(squares[i]->srcMatch->a->vertex,squares[i]->srcMatch->b->vertex,squares[i]->srcMatch->c->vertex,squares[i]->dstMatch->a->vertex,squares[i]->dstMatch->b->vertex,squares[i]->dstMatch->c->vertex, point1, point2, point3);
		finalSrcVerts.push_back(point1);
		finalSrcNorms.push_back(squares[i]->srcMatch->a->normal);
		finalSrcColrs.push_back(squares[i]->srcMatch->a->color);
		finalSrcVerts.push_back(point2);
		finalSrcNorms.push_back(squares[i]->srcMatch->b->normal);
		finalSrcColrs.push_back(squares[i]->srcMatch->b->color);
		finalSrcVerts.push_back(point3);
		finalSrcNorms.push_back(squares[i]->srcMatch->c->normal);
		finalSrcColrs.push_back(squares[i]->srcMatch->c->color);

		finalDstVerts.push_back(squares[i]->dstMatch->a->vertex);
		finalDstNorms.push_back(squares[i]->dstMatch->a->normal);
		finalDstColrs.push_back(squares[i]->dstMatch->a->color);
		finalDstVerts.push_back(squares[i]->dstMatch->b->vertex);
		finalDstNorms.push_back(squares[i]->dstMatch->b->normal);
		finalDstColrs.push_back(squares[i]->dstMatch->b->color);
		finalDstVerts.push_back(squares[i]->dstMatch->c->vertex);
		finalDstNorms.push_back(squares[i]->dstMatch->c->normal);
		finalDstColrs.push_back(squares[i]->dstMatch->c->color);
		//std::cout<< "Square PushBack: " << i << std::endl;
	}

	int srcCount = 0;
	int srcMatch = 0;
	int destCount = 0;
	int destMatch = 0;
	for(float i=0; i<srcTriangles.size();i++){
		if(srcTriangles[i]->covered == false){
			srcCount++;
		}else{
			srcMatch++;
		}
	}
	int dstCount = 0;
	for(float i=0; i<destTriangles.size();i++){
		if(destTriangles[i]->covered == false){
			destCount++;
		}else{
			destMatch++;
		}
	}

	//Handle Unmapped Src Triangles
	for(float i=0; i<srcTriangles.size();i++){
		if(srcTriangles[i]->covered == false){
			srcTriangles[i]->covered = true;
			int index = findTriangleWithMinimumDistance(srcTriangles,destTriangles,i);
			vec4 point1,point2,point3;
			findOptimalOrientation(destTriangles[index]->a->vertex,destTriangles[index]->b->vertex,destTriangles[index]->c->vertex,srcTriangles[i]->a->vertex,srcTriangles[i]->b->vertex,srcTriangles[i]->c->vertex, point1, point2, point3);
			finalSrcVerts.push_back(srcTriangles[i]->a->vertex);
			finalSrcNorms.push_back(srcTriangles[i]->a->normal);
			finalSrcColrs.push_back(srcTriangles[i]->a->color);
			finalSrcVerts.push_back(srcTriangles[i]->b->vertex);
			finalSrcNorms.push_back(srcTriangles[i]->b->normal);
			finalSrcColrs.push_back(srcTriangles[i]->b->color);
			finalSrcVerts.push_back(srcTriangles[i]->c->vertex);
			finalSrcNorms.push_back(srcTriangles[i]->c->normal);
			finalSrcColrs.push_back(srcTriangles[i]->c->color);

			finalDstVerts.push_back(point1);
			finalDstNorms.push_back(destTriangles[index]->a->normal);
			finalDstColrs.push_back(destTriangles[index]->a->color);
			finalDstVerts.push_back(point2);
			finalDstNorms.push_back(destTriangles[index]->b->normal);
			finalDstColrs.push_back(destTriangles[index]->b->color);
			finalDstVerts.push_back(point3);
			finalDstNorms.push_back(destTriangles[index]->c->normal);
			finalDstColrs.push_back(destTriangles[index]->c->color);
		}
	}

	for(float i=0; i<destTriangles.size();i++){
		if(destTriangles[i]->covered == false){
			destTriangles[i]->covered = true;
			int index = findTriangleWithMinimumDistance(destTriangles,srcTriangles,i);
			vec4 point1,point2,point3;
			findOptimalOrientation(srcTriangles[index]->a->vertex,srcTriangles[index]->b->vertex,srcTriangles[index]->c->vertex,destTriangles[i]->a->vertex,destTriangles[i]->b->vertex,destTriangles[i]->c->vertex, point1, point2, point3);
			finalSrcVerts.push_back(point1);
			finalSrcNorms.push_back(srcTriangles[index]->a->normal);
			finalSrcColrs.push_back(srcTriangles[index]->a->color);
			finalSrcVerts.push_back(point2);
			finalSrcNorms.push_back(srcTriangles[index]->b->normal);
			finalSrcColrs.push_back(srcTriangles[index]->b->color);
			finalSrcVerts.push_back(point3);
			finalSrcNorms.push_back(srcTriangles[index]->c->normal);
			finalSrcColrs.push_back(srcTriangles[index]->c->color);

			finalDstVerts.push_back(destTriangles[i]->a->vertex);
			finalDstNorms.push_back(destTriangles[i]->a->normal);
			finalDstColrs.push_back(destTriangles[i]->a->color);
			finalDstVerts.push_back(destTriangles[i]->b->vertex);
			finalDstNorms.push_back(destTriangles[i]->b->normal);
			finalDstColrs.push_back(destTriangles[i]->b->color);
			finalDstVerts.push_back(destTriangles[i]->c->vertex);
			finalDstNorms.push_back(destTriangles[i]->c->normal);
			finalDstColrs.push_back(destTriangles[i]->c->color);
		}
	}
}

void SquareVerticalMeshMap::copyToFinal(){
	//Handle Mesh Maps
	for(size_t i=0; i<squares.size();i++){
		if(squares[i]->srcMatch->srcMatched == false){
			vec4 point1,point2,point3;
			findOptimalOrientation(squares[i]->srcMatch->a->vertex,squares[i]->srcMatch->b->vertex,squares[i]->srcMatch->c->vertex,squares[i]->dstMatch->a->vertex,squares[i]->dstMatch->b->vertex,squares[i]->dstMatch->c->vertex, point1, point2, point3);
			squares[i]->srcMatch->srcMatched = true;
			finalSrcVerts.push_back(point1);
			finalSrcNorms.push_back(squares[i]->srcMatch->a->normal);
			finalSrcColrs.push_back(squares[i]->srcMatch->a->color);
			finalSrcVerts.push_back(point2);
			finalSrcNorms.push_back(squares[i]->srcMatch->b->normal);
			finalSrcColrs.push_back(squares[i]->srcMatch->b->color);
			finalSrcVerts.push_back(point3);
			finalSrcNorms.push_back(squares[i]->srcMatch->c->normal);
			finalSrcColrs.push_back(squares[i]->srcMatch->c->color);
		}else{
			//need to birth this triangle
			float distanceEdge1 = threeDimensionalDistance(squares[i]->dstMatch->a->vertex,squares[i]->srcMatch->a->vertex)+threeDimensionalDistance(squares[i]->dstMatch->b->vertex,squares[i]->srcMatch->b->vertex)+threeDimensionalDistance(squares[i]->dstMatch->c->vertex,vec4((squares[i]->srcMatch->a->vertex.x+squares[i]->srcMatch->b->vertex.x)/2.0,(squares[i]->srcMatch->a->vertex.y+squares[i]->srcMatch->b->vertex.y)/2.0,(squares[i]->srcMatch->a->vertex.w+squares[i]->srcMatch->b->vertex.w)/2.0));
			float distanceEdge2 = threeDimensionalDistance(squares[i]->dstMatch->b->vertex,squares[i]->srcMatch->b->vertex)+threeDimensionalDistance(squares[i]->dstMatch->c->vertex,squares[i]->srcMatch->c->vertex)+threeDimensionalDistance(squares[i]->dstMatch->c->vertex,vec4((squares[i]->srcMatch->b->vertex.x+squares[i]->srcMatch->c->vertex.x)/2.0,(squares[i]->srcMatch->b->vertex.y+squares[i]->srcMatch->c->vertex.y)/2.0,(squares[i]->srcMatch->b->vertex.w+squares[i]->srcMatch->c->vertex.w)/2.0));
			float distanceEdge3 = threeDimensionalDistance(squares[i]->dstMatch->a->vertex,squares[i]->srcMatch->a->vertex)+threeDimensionalDistance(squares[i]->dstMatch->c->vertex,squares[i]->srcMatch->c->vertex)+threeDimensionalDistance(squares[i]->dstMatch->c->vertex,vec4((squares[i]->srcMatch->a->vertex.x+squares[i]->srcMatch->c->vertex.x)/2.0,(squares[i]->srcMatch->a->vertex.y+squares[i]->srcMatch->c->vertex.y)/2.0,(squares[i]->srcMatch->a->vertex.w+squares[i]->srcMatch->c->vertex.w)/2.0));
			Point1* a = new Point1();
			Point1* b = new Point1();
			Point1* c = new Point1();
			if(distanceEdge1 < distanceEdge2){
				if(distanceEdge1 < distanceEdge3){
					//edge1 match
					a = new Point1(squares[i]->srcMatch->a->vertex,squares[i]->srcMatch->a->normal,squares[i]->srcMatch->a->color);
					b = new Point1(squares[i]->srcMatch->b->vertex,squares[i]->srcMatch->b->normal,squares[i]->srcMatch->b->color);
					//c = new Point1(averageVec(squares[i]->srcMatch->b->vertex,squares[i]->srcMatch->a->vertex),squares[i]->srcMatch->b->normal,(squares[i]->srcMatch->b->color+squares[i]->srcMatch->a->color)/2.0);
					c = new Point1(squares[i]->srcMatch->b->vertex,squares[i]->srcMatch->b->normal,squares[i]->srcMatch->b->color);
				}else{
					//edge3 match
					a = new Point1(squares[i]->srcMatch->a->vertex,squares[i]->srcMatch->a->normal,squares[i]->srcMatch->a->color);
					//b = new Point1(averageVec(squares[i]->srcMatch->a->vertex,squares[i]->srcMatch->c->vertex),squares[i]->srcMatch->a->normal,(squares[i]->srcMatch->a->color+squares[i]->srcMatch->c->color)/2.0);
					b = new Point1(squares[i]->srcMatch->a->vertex,squares[i]->srcMatch->a->normal,squares[i]->srcMatch->a->color);
					c = new Point1(squares[i]->srcMatch->c->vertex,squares[i]->srcMatch->c->normal,squares[i]->srcMatch->c->color);
				}
			}else{
				if(distanceEdge3 < distanceEdge2){
					//edge3 match
					a = new Point1(squares[i]->srcMatch->a->vertex,squares[i]->srcMatch->a->normal,squares[i]->srcMatch->a->color);
					//b = new Point1(averageVec(squares[i]->srcMatch->a->vertex,squares[i]->srcMatch->c->vertex),squares[i]->srcMatch->a->normal,(squares[i]->srcMatch->a->color+squares[i]->srcMatch->c->color)/2.0);
					b = new Point1(squares[i]->srcMatch->a->vertex,squares[i]->srcMatch->a->normal,squares[i]->srcMatch->a->color);
					c = new Point1(squares[i]->srcMatch->c->vertex,squares[i]->srcMatch->c->normal,squares[i]->srcMatch->c->color);
				}else{
					//edge2 match
					//a = new Point1(averageVec(squares[i]->srcMatch->b->vertex,squares[i]->srcMatch->c->vertex),squares[i]->srcMatch->c->normal,(squares[i]->srcMatch->c->color+squares[i]->srcMatch->b->color)/2.0);
					a = new Point1(squares[i]->srcMatch->c->vertex,squares[i]->srcMatch->c->normal,squares[i]->srcMatch->c->color);
					b = new Point1(squares[i]->srcMatch->b->vertex,squares[i]->srcMatch->b->normal,squares[i]->srcMatch->b->color);
					c = new Point1(squares[i]->srcMatch->c->vertex,squares[i]->srcMatch->c->normal,squares[i]->srcMatch->c->color);
				}
			}
			finalSrcVerts.push_back(a->vertex);
			finalSrcNorms.push_back(a->normal);
			finalSrcColrs.push_back(a->color);
			finalSrcVerts.push_back(b->vertex);
			finalSrcNorms.push_back(b->normal);
			finalSrcColrs.push_back(b->color);
			finalSrcVerts.push_back(c->vertex);
			finalSrcNorms.push_back(c->normal);
			finalSrcColrs.push_back(c->color);

		}
		if(squares[i]->dstMatch->dstMatched == false){
			squares[i]->dstMatch->dstMatched = true;
			finalDstVerts.push_back(squares[i]->dstMatch->a->vertex);
			finalDstNorms.push_back(squares[i]->dstMatch->a->normal);
			finalDstColrs.push_back(squares[i]->dstMatch->a->color);
			finalDstVerts.push_back(squares[i]->dstMatch->b->vertex);
			finalDstNorms.push_back(squares[i]->dstMatch->b->normal);
			finalDstColrs.push_back(squares[i]->dstMatch->b->color);
			finalDstVerts.push_back(squares[i]->dstMatch->c->vertex);
			finalDstNorms.push_back(squares[i]->dstMatch->c->normal);
			finalDstColrs.push_back(squares[i]->dstMatch->c->color);
			//std::cout<< "Square PushBack: " << i << std::endl;
		}else{
			//need to birth this triangle
				float distanceEdge1 = threeDimensionalDistance(squares[i]->dstMatch->a->vertex,squares[i]->srcMatch->a->vertex)+threeDimensionalDistance(squares[i]->dstMatch->b->vertex,squares[i]->srcMatch->b->vertex)+threeDimensionalDistance(squares[i]->dstMatch->c->vertex,vec4((squares[i]->srcMatch->a->vertex.x+squares[i]->srcMatch->b->vertex.x)/2.0,(squares[i]->srcMatch->a->vertex.y+squares[i]->srcMatch->b->vertex.y)/2.0,(squares[i]->srcMatch->a->vertex.w+squares[i]->srcMatch->b->vertex.w)/2.0));
				float distanceEdge2 = threeDimensionalDistance(squares[i]->dstMatch->b->vertex,squares[i]->srcMatch->b->vertex)+threeDimensionalDistance(squares[i]->dstMatch->c->vertex,squares[i]->srcMatch->c->vertex)+threeDimensionalDistance(squares[i]->dstMatch->c->vertex,vec4((squares[i]->srcMatch->b->vertex.x+squares[i]->srcMatch->c->vertex.x)/2.0,(squares[i]->srcMatch->b->vertex.y+squares[i]->srcMatch->c->vertex.y)/2.0,(squares[i]->srcMatch->b->vertex.w+squares[i]->srcMatch->c->vertex.w)/2.0));
				float distanceEdge3 = threeDimensionalDistance(squares[i]->dstMatch->a->vertex,squares[i]->srcMatch->a->vertex)+threeDimensionalDistance(squares[i]->dstMatch->c->vertex,squares[i]->srcMatch->c->vertex)+threeDimensionalDistance(squares[i]->dstMatch->c->vertex,vec4((squares[i]->srcMatch->a->vertex.x+squares[i]->srcMatch->c->vertex.x)/2.0,(squares[i]->srcMatch->a->vertex.y+squares[i]->srcMatch->c->vertex.y)/2.0,(squares[i]->srcMatch->a->vertex.w+squares[i]->srcMatch->c->vertex.w)/2.0));
				Point1* a = new Point1();
				Point1* b = new Point1();
				Point1* c = new Point1();
				if(distanceEdge1 < distanceEdge2){
					if(distanceEdge1 < distanceEdge3){
						//edge1 match
						a = new Point1(squares[i]->dstMatch->a->vertex,squares[i]->dstMatch->a->normal,squares[i]->dstMatch->a->color);
						b = new Point1(squares[i]->dstMatch->b->vertex,squares[i]->dstMatch->b->normal,squares[i]->dstMatch->b->color);
						//c = new Point1(averageVec(squares[i]->dstMatch->b->vertex,squares[i]->dstMatch->a->vertex),squares[i]->dstMatch->b->normal,(squares[i]->dstMatch->b->color+squares[i]->dstMatch->a->color)/2.0);
						c = new Point1(squares[i]->dstMatch->b->vertex,squares[i]->dstMatch->b->normal,squares[i]->dstMatch->b->color);
					}else{
						//edge3 match
						a = new Point1(squares[i]->dstMatch->a->vertex,squares[i]->dstMatch->a->normal,squares[i]->dstMatch->a->color);
						//b = new Point1(averageVec(squares[i]->dstMatch->c->vertex,squares[i]->dstMatch->a->vertex),squares[i]->dstMatch->a->normal,(squares[i]->dstMatch->a->color+squares[i]->dstMatch->c->color)/2.0);
						b = new Point1(squares[i]->dstMatch->a->vertex,squares[i]->dstMatch->a->normal,squares[i]->dstMatch->a->color);
						c = new Point1(squares[i]->dstMatch->c->vertex,squares[i]->dstMatch->c->normal,squares[i]->dstMatch->c->color);
					}
				}else{
					if(distanceEdge3 < distanceEdge2){
						//edge3 match
						a = new Point1(squares[i]->dstMatch->a->vertex,squares[i]->dstMatch->a->normal,squares[i]->dstMatch->a->color);
						//b = new Point1(averageVec(squares[i]->dstMatch->c->vertex,squares[i]->dstMatch->a->vertex),squares[i]->dstMatch->a->normal,(squares[i]->dstMatch->a->color+squares[i]->dstMatch->c->color)/2.0);
						b = new Point1(squares[i]->dstMatch->a->vertex,squares[i]->dstMatch->a->normal,squares[i]->dstMatch->a->color);
						c = new Point1(squares[i]->dstMatch->c->vertex,squares[i]->dstMatch->c->normal,squares[i]->dstMatch->c->color);
					}else{
						//edge2 match
						//a = new Point1(averageVec(squares[i]->dstMatch->c->vertex,squares[i]->dstMatch->b->vertex),squares[i]->dstMatch->c->normal,(squares[i]->dstMatch->c->color+squares[i]->dstMatch->b->color)/2.0);
						a = new Point1(squares[i]->dstMatch->c->vertex,squares[i]->dstMatch->c->normal,squares[i]->dstMatch->c->color);
						b = new Point1(squares[i]->dstMatch->b->vertex,squares[i]->dstMatch->b->normal,squares[i]->dstMatch->b->color);
						c = new Point1(squares[i]->dstMatch->c->vertex,squares[i]->dstMatch->c->normal,squares[i]->dstMatch->c->color);
					}
				}
				finalDstVerts.push_back(a->vertex);
				finalDstNorms.push_back(a->normal);
				finalDstColrs.push_back(a->color);
				finalDstVerts.push_back(b->vertex);
				finalDstNorms.push_back(b->normal);
				finalDstColrs.push_back(b->color);
				finalDstVerts.push_back(c->vertex);
				finalDstNorms.push_back(c->normal);
				finalDstColrs.push_back(c->color);
		}
	}


	//Handle Unmapped Src Triangles
	for(float i=0; i<srcTriangles.size();i++){
		if(srcTriangles[i]->covered == false){
			srcTriangles[i]->covered = true;
			int index = findTriangleWithMinimumDistance(srcTriangles,destTriangles,i);
			vec4 point1,point2,point3;
			findOptimalOrientation(destTriangles[index]->a->vertex,destTriangles[index]->b->vertex,destTriangles[index]->c->vertex,srcTriangles[i]->a->vertex,srcTriangles[i]->b->vertex,srcTriangles[i]->c->vertex, point1, point2, point3);
			finalSrcVerts.push_back(srcTriangles[i]->a->vertex);
			finalSrcNorms.push_back(srcTriangles[i]->a->normal);
			finalSrcColrs.push_back(srcTriangles[i]->a->color);
			finalSrcVerts.push_back(srcTriangles[i]->b->vertex);
			finalSrcNorms.push_back(srcTriangles[i]->b->normal);
			finalSrcColrs.push_back(srcTriangles[i]->b->color);
			finalSrcVerts.push_back(srcTriangles[i]->c->vertex);
			finalSrcNorms.push_back(srcTriangles[i]->c->normal);
			finalSrcColrs.push_back(srcTriangles[i]->c->color);

			finalDstVerts.push_back(point1);
			finalDstNorms.push_back(destTriangles[index]->a->normal);
			finalDstColrs.push_back(destTriangles[index]->a->color);
			finalDstVerts.push_back(point2);
			finalDstNorms.push_back(destTriangles[index]->b->normal);
			finalDstColrs.push_back(destTriangles[index]->b->color);
			finalDstVerts.push_back(point3);
			finalDstNorms.push_back(destTriangles[index]->c->normal);
			finalDstColrs.push_back(destTriangles[index]->c->color);
		}
	}

	for(float i=0; i<destTriangles.size();i++){
		if(destTriangles[i]->covered == false){
			destTriangles[i]->covered = true;
			int index = findTriangleWithMinimumDistance(destTriangles,srcTriangles,i);
			vec4 point1,point2,point3;
			findOptimalOrientation(srcTriangles[index]->a->vertex,srcTriangles[index]->b->vertex,srcTriangles[index]->c->vertex,destTriangles[i]->a->vertex,destTriangles[i]->b->vertex,destTriangles[i]->c->vertex, point1, point2, point3);
			finalSrcVerts.push_back(point1);
			finalSrcNorms.push_back(srcTriangles[index]->a->normal);
			finalSrcColrs.push_back(srcTriangles[index]->a->color);
			finalSrcVerts.push_back(point2);
			finalSrcNorms.push_back(srcTriangles[index]->b->normal);
			finalSrcColrs.push_back(srcTriangles[index]->b->color);
			finalSrcVerts.push_back(point3);
			finalSrcNorms.push_back(srcTriangles[index]->c->normal);
			finalSrcColrs.push_back(srcTriangles[index]->c->color);

			finalDstVerts.push_back(destTriangles[i]->a->vertex);
			finalDstNorms.push_back(destTriangles[i]->a->normal);
			finalDstColrs.push_back(destTriangles[i]->a->color);
			finalDstVerts.push_back(destTriangles[i]->b->vertex);
			finalDstNorms.push_back(destTriangles[i]->b->normal);
			finalDstColrs.push_back(destTriangles[i]->b->color);
			finalDstVerts.push_back(destTriangles[i]->c->vertex);
			finalDstNorms.push_back(destTriangles[i]->c->normal);
			finalDstColrs.push_back(destTriangles[i]->c->color);
		}
	}
}

void SquareVerticalMeshMap::copyBackToObjects(Object* Src,Object* Dest,std::vector< Square* > squares){

	Src->_vertices.clear();
	Src->_colors.clear();
	Src->_normals.clear();
	Dest->_vertices.clear();
	Dest->_colors.clear();
	Dest->_normals.clear();

	//Handle Mesh Maps
	for(size_t i=0; i<squares.size();i++){
		vec4 point1,point2,point3;
		findOptimalOrientation(squares[i]->srcMatch->a->vertex,squares[i]->srcMatch->b->vertex,squares[i]->srcMatch->c->vertex,squares[i]->dstMatch->a->vertex,squares[i]->dstMatch->b->vertex,squares[i]->dstMatch->c->vertex, point1, point2, point3);
		Src->_vertices.push_back(point1);
		Src->_normals.push_back(squares[i]->srcMatch->a->normal);
		Src->_colors.push_back(squares[i]->srcMatch->a->color);
		Src->_vertices.push_back(point2);
		Src->_normals.push_back(squares[i]->srcMatch->b->normal);
		Src->_colors.push_back(squares[i]->srcMatch->b->color);
		Src->_vertices.push_back(point3);
		Src->_normals.push_back(squares[i]->srcMatch->c->normal);
		Src->_colors.push_back(squares[i]->srcMatch->c->color);

		Dest->_vertices.push_back(squares[i]->dstMatch->a->vertex);
		Dest->_normals.push_back(squares[i]->dstMatch->a->normal);
		Dest->_colors.push_back(squares[i]->dstMatch->a->color);
		Dest->_vertices.push_back(squares[i]->dstMatch->b->vertex);
		Dest->_normals.push_back(squares[i]->dstMatch->b->normal);
		Dest->_colors.push_back(squares[i]->dstMatch->b->color);
		Dest->_vertices.push_back(squares[i]->dstMatch->c->vertex);
		Dest->_normals.push_back(squares[i]->dstMatch->c->normal);
		Dest->_colors.push_back(squares[i]->dstMatch->c->color);
		//std::cout<< "Square PushBack: " << i << std::endl;
	}

	int srcCount = 0;
	int srcMatch = 0;
	int destCount = 0;
	int destMatch = 0;
	for(float i=0; i<srcTriangles.size();i++){
		if(srcTriangles[i]->covered == false){
			srcCount++;
		}else{
			srcMatch++;
		}
	}
	int dstCount = 0;
	for(float i=0; i<destTriangles.size();i++){
		if(destTriangles[i]->covered == false){
			destCount++;
		}else{
			destMatch++;
		}
	}
	std::cout << "Matched Source Triangles     : " << srcMatch << std::endl;
	std::cout << "Matched Destination Triangles: " << destMatch << std::endl;
	std::cout << "Unmatched Source Triangles     : " << srcCount << std::endl;
	std::cout << "Unmatched Destination Triangles: " << destCount << std::endl;

	//Handle Unmapped Src Triangles
	for(float i=0; i<srcTriangles.size();i++){
		if(srcTriangles[i]->covered == false){
			srcTriangles[i]->covered = true;
			int index = findTriangleWithMinimumDistance(srcTriangles,destTriangles,i);
			vec4 point1,point2,point3;
			findOptimalOrientation(destTriangles[index]->a->vertex,destTriangles[index]->b->vertex,destTriangles[index]->c->vertex,srcTriangles[i]->a->vertex,srcTriangles[i]->b->vertex,srcTriangles[i]->c->vertex, point1, point2, point3);
			Src->_vertices.push_back(srcTriangles[i]->a->vertex);
			Src->_normals.push_back(srcTriangles[i]->a->normal);
			Src->_colors.push_back(srcTriangles[i]->a->color);
			Src->_vertices.push_back(srcTriangles[i]->b->vertex);
			Src->_normals.push_back(srcTriangles[i]->b->normal);
			Src->_colors.push_back(srcTriangles[i]->b->color);
			Src->_vertices.push_back(srcTriangles[i]->c->vertex);
			Src->_normals.push_back(srcTriangles[i]->c->normal);
			Src->_colors.push_back(srcTriangles[i]->c->color);

			Dest->_vertices.push_back(point1);
			Dest->_normals.push_back(destTriangles[index]->a->normal);
			Dest->_colors.push_back(destTriangles[index]->a->color);
			Dest->_vertices.push_back(point2);
			Dest->_normals.push_back(destTriangles[index]->b->normal);
			Dest->_colors.push_back(destTriangles[index]->b->color);
			Dest->_vertices.push_back(point3);
			Dest->_normals.push_back(destTriangles[index]->c->normal);
			Dest->_colors.push_back(destTriangles[index]->c->color);
		}
	}

	for(float i=0; i<destTriangles.size();i++){
		if(destTriangles[i]->covered == false){
			destTriangles[i]->covered = true;
			int index = findTriangleWithMinimumDistance(destTriangles,srcTriangles,i);
			vec4 point1,point2,point3;
			findOptimalOrientation(srcTriangles[index]->a->vertex,srcTriangles[index]->b->vertex,srcTriangles[index]->c->vertex,destTriangles[i]->a->vertex,destTriangles[i]->b->vertex,destTriangles[i]->c->vertex, point1, point2, point3);
			Src->_vertices.push_back(point1);
			Src->_normals.push_back(srcTriangles[index]->a->normal);
			Src->_colors.push_back(srcTriangles[index]->a->color);
			Src->_vertices.push_back(point2);
			Src->_normals.push_back(srcTriangles[index]->b->normal);
			Src->_colors.push_back(srcTriangles[index]->b->color);
			Src->_vertices.push_back(point3);
			Src->_normals.push_back(srcTriangles[index]->c->normal);
			Src->_colors.push_back(srcTriangles[index]->c->color);

			Dest->_vertices.push_back(destTriangles[i]->a->vertex);
			Dest->_normals.push_back(destTriangles[i]->a->normal);
			Dest->_colors.push_back(destTriangles[i]->a->color);
			Dest->_vertices.push_back(destTriangles[i]->b->vertex);
			Dest->_normals.push_back(destTriangles[i]->b->normal);
			Dest->_colors.push_back(destTriangles[i]->b->color);
			Dest->_vertices.push_back(destTriangles[i]->c->vertex);
			Dest->_normals.push_back(destTriangles[i]->c->normal);
			Dest->_colors.push_back(destTriangles[i]->c->color);
		}
	}
}

int SquareVerticalMeshMap::findTriangleWithMinimumDistance(std::vector< Triangle* > larger, std::vector< Triangle* > smaller, int index){
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

int SquareHorizontalMeshMap::findTriangleWithMinimumDistance(std::vector< Triangle* > larger, std::vector< Triangle* > smaller, int index){
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

Angel::vec2 Square::getSquareCenter(float xMin,float xMax, float yMin, float yMax){
	return Angel::vec2((xMax+xMin)/2.0,(yMax+yMin)/2.0);
}

bool compareSquareTriangles(Triangle* a, Triangle* b){
	//if(comparePoints(a->a,b->a)){
	if(a->a->vertex.y > b->a->vertex.y){
		return true;
	}
	else{
		return false;
	}
}

Angel::vec3 getMin(std::vector<Angel::vec4> points){
	Angel::vec3 min = Angel::vec3( INFINITY, INFINITY, INFINITY);
	  for ( size_t i = 0; i < points.size(); ++i ) {
		if ( points[i].x < min.x ) min.x = points[i].x;
		if ( points[i].y < min.y ) min.y = points[i].y;
		if ( points[i].z < min.z ) min.z = points[i].z;
	  }

	  return min;
}

Angel::vec3 getMax(std::vector<Angel::vec4> points){
	Angel::vec3 max = Angel::vec3( -INFINITY, -INFINITY, -INFINITY);
	for ( size_t i = 0; i < points.size(); ++i ) {
	if ( points[i].x > max.x ) max.x = points[i].x;
	if ( points[i].y > max.y ) max.y = points[i].y;
	if ( points[i].z > max.z ) max.z = points[i].z;
	}

	return max;
}

Angel::vec4 averageVec(Angel::vec4 a, Angel::vec4 b){
	return Angel::vec4((a.x-b.x)/2.0,(a.y-b.y)/2.0,(a.z-b.z)/2.0,1.0);
}
