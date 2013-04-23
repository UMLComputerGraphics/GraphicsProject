/**
 * @file scaleModel.hpp
 * @author Zachary Maybury
 * @date 2013-04-21
 * @brief functionality to scale models to aid morph matching
 * @details 
 **/
 
 #ifndef __SCALEMODEL_HPP
 #define __SCALEMODEL_HPP
 
 #include <string>
 #include <vector>
 #include <cmath>
 #include "platform.h"
 #include "vec.hpp"
 #include "mat.hpp"
 #include "Scene.hpp"
 #include "Object.hpp"
 
 struct ScaleModel {
 	Object* src;
 	Object* dest;
 	int scaleFactorX;
 	int scaleFactorY;
 	int scaleFactorZ;
	Angel::vec3 originalLowBoundSrc;
  	Angel::vec3 originalMaxBoundSrc;
  	Angel::vec3 originalLowBoundDst;
  	Angel::vec3 originalMaxBoundDst;
	std::vector< Angel::vec4 > srcVertices;
	std::vector< Angel::vec4 > destVertices;
  		
  	ScaleModel(Object* Src, Object* Dest, int scaleX, int scaleY, int scaleZ){
  		src = Src;
  		dest = Dest;
  		scaleFactorX = scaleX;
  		scaleFactorY = scaleY;
  		scaleFactorZ = scaleZ;
  		originalLowBoundSrc = src->getMin();
  		originalMaxBoundSrc = src->getMax();
  		originalLowBoundDst = dest->getMin();
  		originalMaxBoundDst = dest->getMax();
  		scaleModels();
  	}
  	
  	ScaleModel(std::vector< Angel::vec4 >& src,std::vector< Angel::vec4 >& dest,int scaleX,int scaleY,int scaleZ){		
		srcVertices = src;
		destVertices = dest;
		scaleFactorX = scaleX;
  		scaleFactorY = scaleY;
  		scaleFactorZ = scaleZ;
		originalLowBoundSrc = getMin(src);
  		originalMaxBoundSrc = getMax(src);
  		originalLowBoundDst = getMin(dest);
  		originalMaxBoundDst = getMax(dest);
		scalePartitionModel();
	}
  	
  	
  	void scaleModels();
  	void scalePartitionModel();
  	void restoreModels();
  	void restorePartitionModel();
  	Angel::vec3 getMin(std::vector< Angel::vec4 > vertices);
  	Angel::vec3 getMax(std::vector< Angel::vec4 > vertices);
 };
 
 
 #endif