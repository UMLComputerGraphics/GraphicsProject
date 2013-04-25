/**
 * @file bipartiteGraph.cpp
 * @author Zachary Maybury
 * @date 2013-03-28
 * @brief bipartite Graph for morph matching
 * @details
 **/
 
 #include "bipartiteGraph.hpp"
 #include "modelFunctions.hpp"
 
 void BipartiteGraph::constructGraph(){
 	printf("Constructing Graph...\n");
 	for(size_t i=0; i<src->numberOfPoints(); i+=3){
 		Node * temp = new Node(i,i/3);
 		temp->a = src->_vertices[i];
 		temp->b = src->_vertices[i+1];
 		temp->c = src->_vertices[i+2];
 		srcNodes.push_back(temp);
 	}
 	for(size_t i=0; i<dest->numberOfPoints(); i+=3){
 		Node * temp = new Node(i,i/3);
 		temp->a = dest->_vertices[i];
 		temp->b = dest->_vertices[i+1];
 		temp->c = dest->_vertices[i+2];
 		destNodes.push_back(temp);
 	}
 	printf("Calculate Distances...\n");
 	for(size_t i=0; i<srcNodes.size(); i++){
 		for(size_t j=0; j<destNodes.size(); j++){
 			vec4 point1,point2,point3;
 			findOptimalOrientation(destNodes[j]->a,destNodes[j]->b,destNodes[j]->c,srcNodes[i]->a,srcNodes[i]->b,srcNodes[i]->c, point1, point2, point3);
			float distance = threeDimensionalDistance(point1,srcNodes[i]->a)+threeDimensionalDistance(point2,srcNodes[i]->b)+threeDimensionalDistance(point3,srcNodes[i]->c);
			if(distance < srcNodes[i]->minDistance){
				srcNodes[i]->minDistance = distance;
				srcNodes[i]->minDistanceIndex = j;
				//printf("Min Distance %d: %f\n", i, srcNodes[i]->minDistance);
			}else if (distance == srcNodes[i]->minDistance){
				printf("More than 1 match for node %lu\n", i);
			}
			edges.push_back(new Edge(distance,srcNodes[i],destNodes[j]));
 		}
 	}
 	
 	printf("Sort by min distance...\n");
 	std::sort (edges.begin(), edges.end(), compareNodes);
 	createMatches();	
 }
 
 bool compareNodes(Edge * a, Edge * b){
 	return (a->distance<b->distance);
 }
 
 void BipartiteGraph::createMatches(){
 	printf("Creating Matches...\n");
 	
 	printf("Total Edges: %lu\n",edges.size());
 	std::vector< Edge* > bestEdges;
 	std::vector< Edge* > firstBirths;
 	

 	for(size_t i=0; i<edges.size(); i++){
 		if((!edges[i]->source->matched)&&(!edges[i]->destination->matched)&&(!edges[i]->destination->hasBetterMatch)){
 			edges[i]->source->matched = true;
 			edges[i]->source->hasBetterMatch = true;
 			edges[i]->destination->matched = true;
 			edges[i]->destination->hasBetterMatch = true;
 			edges[i]->source->matchNode = edges[i]->destination;
 			edges[i]->destination->matchNode = edges[i]->source;
 			bestEdges.push_back(new Edge(edges[i]->distance,edges[i]->source,edges[i]->destination));
 			//printf("Min Index: %d  Node Index: %d  Distance: %f\n", bestEdges[i]->source->minDistanceIndex, bestEdges[i]->destination->nodeIndex, bestEdges[i]->source->minDistance);
		}else if((edges[i]->source->matched)&&(!edges[i]->destination->matched)){
			edges[i]->destination->hasBetterMatch = true;
		}
 	}
 	printf("Best Edges: %lu\n\n", bestEdges.size());
 	//printf("Edge Match 1: %d - %d\n", bestEdges[0]->source->modelIndex,bestEdges[0]->source->matchNode->modelIndex);
 	for(size_t i=0; i<bestEdges.size(); i++){
 		if(bestEdges[i]->source->minDistanceIndex != bestEdges[i]->destination->nodeIndex){
 			firstBirths.push_back(new Edge(bestEdges[i]->distance,bestEdges[i]->source,bestEdges[i]->destination));
 		}
 	}
 	
 	int count = 0;
 	for(size_t i=0; i<bestEdges.size(); i++){ 
 		if(bestEdges[i]->source->minDistanceIndex == bestEdges[i]->destination->nodeIndex){
 			for(int m=0; m<3; m++){
 				//Handle source points
				vec4 tempPoints = src->_vertices[3*i+m];
				vec3 tempNormals = src->_normals[3*i+m];
				vec4 tempColors = src->_colors[3*i+m];
				src->_vertices[3*i+m] = src->_vertices[bestEdges[i]->source->modelIndex+m];
				src->_normals[3*i+m] = src->_normals[bestEdges[i]->source->modelIndex+m];
				src->_colors[3*i+m] = src->_colors[bestEdges[i]->source->modelIndex+m];
				src->_vertices[bestEdges[i]->source->modelIndex+m] = tempPoints;
				src->_normals[bestEdges[i]->source->modelIndex+m] = tempNormals;
				src->_colors[bestEdges[i]->source->modelIndex+m] = tempColors;
				//Handle destination points
				tempPoints = dest->_vertices[3*i+m];
				tempNormals = dest->_normals[3*i+m];
				tempColors = dest->_colors[3*i+m];
				dest->_vertices[3*i+m] = dest->_vertices[bestEdges[i]->destination->modelIndex+m];
				dest->_normals[3*i+m] = dest->_normals[bestEdges[i]->destination->modelIndex+m];
				dest->_colors[3*i+m] = dest->_colors[bestEdges[i]->destination->modelIndex+m];
				dest->_vertices[bestEdges[i]->destination->modelIndex+m] = tempPoints;
				dest->_normals[bestEdges[i]->destination->modelIndex+m] = tempNormals;
				dest->_colors[bestEdges[i]->destination->modelIndex+m] = tempColors;
				count++;
			}
		}/*else{
			bestEdges.erase(bestEdges.begin()+i);
			for(int m=0; m<3; m++){
				srcVertices.erase(srcVertices.begin()+3*i);
				srcNormals.erase(srcNormals.begin()+3*i);
 				srcColors.erase(srcColors.begin()+3*i);
 				destVertices.erase(destVertices.begin()+3*i);
 				destNormals.erase(destNormals.begin()+3*i);
 				destColors.erase(destColors.begin()+3*i);
			}
			i--;
		}*/
 	}

 	/*for(size_t i=bestEdges.size()-firstBirths.size(); i<bestEdges.size(); i++){
 		for(int m=0; m<3; m++){
 			src->_vertices.pop_back();
 			src->_normals.pop_back();
 			src->_colors.pop_back();
 			dest->_vertices.pop_back();
 			dest->_normals.pop_back();
 			dest->_colors.pop_back();
 			count++;
 		}
 	}
 	printf("Count: %d\n", count);*/
 }
 
 void BipartiteGraph::constructPartitionGraph(){
 	 	printf("Constructing Graph...\n");
 	for(size_t i=0; i<srcVertices.size(); i+=3){
 		Node * temp = new Node(i,i/3);
 		temp->a = srcVertices[i];
 		temp->b = srcVertices[i+1];
 		temp->c = srcVertices[i+2];
 		srcNodes.push_back(temp);
 	}
 	for(size_t i=0; i<destVertices.size(); i+=3){
 		Node * temp = new Node(i,i/3);
 		temp->a = destVertices[i];
 		temp->b = destVertices[i+1];
 		temp->c = destVertices[i+2];
 		destNodes.push_back(temp);
 	}
 	printf("Calculate Distances...\n");
 	for(size_t i=0; i<srcNodes.size(); i++){
 		for(size_t j=0; j<destNodes.size(); j++){
 			vec4 point1,point2,point3;
 			findOptimalOrientation(destNodes[j]->a,destNodes[j]->b,destNodes[j]->c,srcNodes[i]->a,srcNodes[i]->b,srcNodes[i]->c, point1, point2, point3);
			float distance = threeDimensionalDistance(point1,srcNodes[i]->a)+threeDimensionalDistance(point2,srcNodes[i]->b)+threeDimensionalDistance(point3,srcNodes[i]->c);
			if(distance < srcNodes[i]->minDistance){
				srcNodes[i]->minDistance = distance;
				srcNodes[i]->minDistanceIndex = j;
				//printf("Min Distance %d: %f\n", i, srcNodes[i]->minDistance);
			}else if (distance == srcNodes[i]->minDistance){
				printf("More than 1 match for node %lu\n", i);
			}
			edges.push_back(new Edge(distance,srcNodes[i],destNodes[j]));
 		}
 	}
 	
 	printf("Sort by min distance...\n");
 	std::sort (edges.begin(), edges.end(), compareNodes);
 	createPartitionMatches();	
 }
 
 void BipartiteGraph::createPartitionMatches(){
 	 printf("Creating Matches...\n");
 	
 	printf("Total Edges: %lu\n",edges.size());
 	std::vector< Edge* > bestEdges;
 	std::vector< Edge* > firstBirths;
 	
 	for(size_t i=0; i<edges.size(); i++){
 		if((!edges[i]->source->matched)&&(!edges[i]->destination->matched)&&(!edges[i]->destination->hasBetterMatch)){
 			edges[i]->source->matched = true;
 			edges[i]->source->hasBetterMatch = true;
 			edges[i]->destination->matched = true;
 			edges[i]->destination->hasBetterMatch = true;
 			edges[i]->source->matchNode = edges[i]->destination;
 			edges[i]->destination->matchNode = edges[i]->source;
 			bestEdges.push_back(new Edge(edges[i]->distance,edges[i]->source,edges[i]->destination));
 			//printf("Min Index: %d  Node Index: %d  Distance: %f\n", bestEdges[i]->source->minDistanceIndex, bestEdges[i]->destination->nodeIndex, bestEdges[i]->source->minDistance);
		}else if((edges[i]->source->matched)&&(!edges[i]->destination->matched)){
			edges[i]->destination->hasBetterMatch = true;
		}
 	}
 	printf("Best Edges: %lu\n\n", bestEdges.size());
 	//printf("Edge Match 1: %d - %d\n", bestEdges[0]->source->modelIndex,bestEdges[0]->source->matchNode->modelIndex);
 	for(size_t i=0; i<bestEdges.size(); i++){
 		if(bestEdges[i]->source->minDistanceIndex != bestEdges[i]->destination->nodeIndex){
 			firstBirths.push_back(new Edge(bestEdges[i]->distance,bestEdges[i]->source,bestEdges[i]->destination));
 		}
 	}
 	
 	int count = 0;
 	for(size_t i=0; i<bestEdges.size(); i++){ 
	 	if(bestEdges[i]->source->minDistanceIndex == bestEdges[i]->destination->nodeIndex){
 			for(int m=0; m<3; m++){
 				//Handle source points
				vec4 tempPoints = srcVertices[3*i+m];
				vec3 tempNormals = srcNormals[3*i+m];
				vec4 tempColors = srcColors[3*i+m];
				srcVertices[3*i+m] = srcVertices[bestEdges[i]->source->modelIndex+m];
				srcNormals[3*i+m] = srcNormals[bestEdges[i]->source->modelIndex+m];
				srcColors[3*i+m] = srcColors[bestEdges[i]->source->modelIndex+m];
				srcVertices[bestEdges[i]->source->modelIndex+m] = tempPoints;
				srcNormals[bestEdges[i]->source->modelIndex+m] = tempNormals;
				srcColors[bestEdges[i]->source->modelIndex+m] = tempColors;
				//Handle destination points
				tempPoints = destVertices[3*i+m];
				tempNormals = destNormals[3*i+m];
				tempColors = destColors[3*i+m];
				destVertices[3*i+m] = destVertices[bestEdges[i]->destination->modelIndex+m];
				destNormals[3*i+m] = destNormals[bestEdges[i]->destination->modelIndex+m];
				destColors[3*i+m] = destColors[bestEdges[i]->destination->modelIndex+m];
				destVertices[bestEdges[i]->destination->modelIndex+m] = tempPoints;
				destNormals[bestEdges[i]->destination->modelIndex+m] = tempNormals;
				destColors[bestEdges[i]->destination->modelIndex+m] = tempColors;
				count++;
			}
		}else{
			printf("Min Index: %d  Node Index: %d\n", bestEdges[i]->source->minDistanceIndex, bestEdges[i]->destination->nodeIndex);
			bestEdges.erase(bestEdges.begin()+i);
			for(int m=0; m<3; m++){
				srcVertices.erase(srcVertices.begin()+3*i);
				srcNormals.erase(srcNormals.begin()+3*i);
 				srcColors.erase(srcColors.begin()+3*i);
 				destVertices.erase(destVertices.begin()+3*i);
 				destNormals.erase(destNormals.begin()+3*i);
 				destColors.erase(destColors.begin()+3*i);
			}
			i--;
		}
 	}

 	for(size_t i=bestEdges.size()-firstBirths.size(); i<bestEdges.size(); i++){
 		for(int m=0; m<3; m++){
 			srcVertices.pop_back();
 			srcNormals.pop_back();
 			srcColors.pop_back();
 			destVertices.pop_back();
 			destNormals.pop_back();
 			destColors.pop_back();
 			count++;
 		}
 	}
 	printf("Count: %d\n", count);
 }
