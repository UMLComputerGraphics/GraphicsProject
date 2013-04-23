/**
 * @file bipartiteGraph.hpp
 * @author Zachary Maybury
 * @date 2013-04-18
 * @brief create graph for matching purposes
 * @details 
 **/
 
#ifndef __BIPARTITEGRAPH_HPP
#define __BIPARTITEGRAPH_HPP

#include <string>
#include <vector>
#include <cmath>
#include "platform.h"
#include "vec.hpp"
#include "mat.hpp"
#include "Scene.hpp"
#include "Object.hpp"

struct Node {
	int modelIndex;
	int nodeIndex;
	Node * matchNode;
	bool matched;
	bool hasBetterMatch;
	float minDistance;
	int minDistanceIndex;
	Angel::vec4 a;
	Angel::vec4 b;
	Angel::vec4 c;
	
	Node(){
		modelIndex = -1;
		nodeIndex = -1;
		matchNode = NULL;
		matched = false;
		hasBetterMatch = false;
		minDistance = INFINITY;
		minDistanceIndex = -1;
	}
	Node(int index, int nodeindex){
		modelIndex = index;
		nodeIndex = nodeindex;
		matchNode = NULL;
		matched = false;
		hasBetterMatch = false;
		minDistance = INFINITY;
		minDistanceIndex = -1;
	}
};

struct Edge {
	float distance;
	Node * source;
	Node * destination;
	
	Edge(float dist, Node *src, Node *dest){
		distance = dist;
		source = src;
		destination = dest;
	}
};

struct BipartiteGraph {
	std::vector< Node* > srcNodes;
	std::vector< Node* > destNodes;
	std::vector< Edge* > edges;
	Object* src;
	Object* dest;
	std::vector< Angel::vec4 > srcVertices;
	std::vector< Angel::vec3 > srcNormals;
	std::vector< Angel::vec4 > srcColors;
	std::vector< Angel::vec2 > srcTextures;
	std::vector< Angel::vec4 > destVertices;
	std::vector< Angel::vec3 > destNormals;
	std::vector< Angel::vec4 > destColors;
	std::vector< Angel::vec2 > destTextures;

	BipartiteGraph(Object* Src, Object* Dest){
		if(Src->_vertices.size() > Dest->_vertices.size()){
			src = Src;
			dest = Dest;
		}else{
			src = Dest;
			dest = Src;
		}
		constructGraph();
	}
	BipartiteGraph(std::vector< Angel::vec4 >& model1Vertices,std::vector< Angel::vec3 >& model1Normals,std::vector< Angel::vec4 >& model1Colors,std::vector< Angel::vec2 >& model1Textures, std::vector< Angel::vec4 >& model2Vertices,std::vector< Angel::vec3 >& model2Normals,std::vector< Angel::vec4 >& model2Colors,std::vector< Angel::vec2 >& model2Textures){		
		srcVertices = model1Vertices;
		srcNormals = model1Normals;
		srcColors = model1Colors;
		srcTextures = model1Textures;
		
		destVertices = model2Vertices;
		destNormals = model2Normals;
		destColors = model2Colors;
		destTextures = model2Textures;
		constructPartitionGraph();
	}
	
	void constructGraph();
	void constructPartitionGraph();
	void createMatches();
	void createPartitionMatches();
};

bool compareNodes(Edge * a, Edge * b);

#endif
