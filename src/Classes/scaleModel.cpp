/**
 * @file scaleModel.cpp
 * @author Zachary Maybury
 * @date 2013-04-21
 * @brief functionality to scale models to aid morph matching
 * @details 
 **/
 
 #include "scaleModel.hpp"
 
 void ScaleModel::scaleModels(){
 	for(size_t i=0; i<src->_vertices.size(); i++){
 		src->_vertices[i] = vec4(scaleFactorX*(src->_vertices[i].x - originalLowBoundSrc.x)/(originalMaxBoundSrc.x - originalLowBoundSrc.x),
 		scaleFactorY*(src->_vertices[i].y - originalLowBoundSrc.y)/(originalMaxBoundSrc.y - originalLowBoundSrc.y),
 		scaleFactorZ*(src->_vertices[i].z - originalLowBoundSrc.z)/(originalMaxBoundSrc.z - originalLowBoundSrc.z), src->_vertices[i].w);
 	}
 	for(size_t i=0; i<dest->_vertices.size(); i++){
 		dest->_vertices[i] = vec4(scaleFactorX*(dest->_vertices[i].x - originalLowBoundDst.x)/(originalMaxBoundDst.x - originalLowBoundDst.x),
 		scaleFactorY*(dest->_vertices[i].y - originalLowBoundDst.y)/(originalMaxBoundDst.y - originalLowBoundDst.y),
 		scaleFactorZ*(dest->_vertices[i].z - originalLowBoundDst.z)/(originalMaxBoundDst.z - originalLowBoundDst.z),dest->_vertices[i].w);
 	}
 }
 
 void ScaleModel::scalePartitionModel(){
 	for(size_t i=0; i<srcVertices.size(); i++){
 		srcVertices[i] = vec4(scaleFactorX*(srcVertices[i].x - originalLowBoundSrc.x)/(originalMaxBoundSrc.x - originalLowBoundSrc.x),
 		scaleFactorY*(srcVertices[i].y - originalLowBoundSrc.y)/(originalMaxBoundSrc.y - originalLowBoundSrc.y),
 		scaleFactorZ*(srcVertices[i].z - originalLowBoundSrc.z)/(originalMaxBoundSrc.z - originalLowBoundSrc.z), srcVertices[i].w);
 	}
 	for(size_t i=0; i<destVertices.size(); i++){
 		destVertices[i] = vec4(scaleFactorX*(destVertices[i].x - originalLowBoundDst.x)/(originalMaxBoundDst.x - originalLowBoundDst.x),
 		scaleFactorY*(destVertices[i].y - originalLowBoundDst.y)/(originalMaxBoundDst.y - originalLowBoundDst.y),
 		scaleFactorZ*(destVertices[i].z - originalLowBoundDst.z)/(originalMaxBoundDst.z - originalLowBoundDst.z),destVertices[i].w);
 	}
 }
 
 void ScaleModel::restoreModels(){
 	for(size_t i=0; i<src->_vertices.size(); i++){
 		src->_vertices[i] = vec4(
 			(originalMaxBoundSrc.x - originalLowBoundSrc.x)*(src->_vertices[i].x)/(scaleFactorX)+originalLowBoundSrc.x,
 			(originalMaxBoundSrc.y - originalLowBoundSrc.y)*(src->_vertices[i].y)/(scaleFactorY)+originalLowBoundSrc.y,
 			(originalMaxBoundSrc.z - originalLowBoundSrc.z)*(src->_vertices[i].z)/(scaleFactorZ)+originalLowBoundSrc.z,
 			src->_vertices[i].w);			 			
 	}
 	for(size_t i=0; i<dest->_vertices.size(); i++){
 		dest->_vertices[i] = vec4(
 			(originalMaxBoundDst.x - originalLowBoundDst.x)*(dest->_vertices[i].x)/(scaleFactorX)+originalLowBoundDst.x,
 			(originalMaxBoundDst.y - originalLowBoundDst.y)*(dest->_vertices[i].y)/(scaleFactorY)+originalLowBoundDst.y,
 			(originalMaxBoundDst.z - originalLowBoundDst.z)*(dest->_vertices[i].z)/(scaleFactorZ)+originalLowBoundDst.z,
 			dest->_vertices[i].w);			 			
 	}
 }
 
 void ScaleModel::restorePartitionModel(){
 	for(size_t i=0; i<srcVertices.size(); i++){
 		srcVertices[i] = vec4(
 			(originalMaxBoundSrc.x - originalLowBoundSrc.x)*(srcVertices[i].x)/(scaleFactorX)+originalLowBoundSrc.x,
 			(originalMaxBoundSrc.y - originalLowBoundSrc.y)*(srcVertices[i].y)/(scaleFactorY)+originalLowBoundSrc.y,
 			(originalMaxBoundSrc.z - originalLowBoundSrc.z)*(srcVertices[i].z)/(scaleFactorZ)+originalLowBoundSrc.z,
 			srcVertices[i].w);			 			
 	}
 	for(size_t i=0; i<destVertices.size(); i++){
 		destVertices[i] = vec4(
 			(originalMaxBoundDst.x - originalLowBoundDst.x)*(destVertices[i].x)/(scaleFactorX)+originalLowBoundDst.x,
 			(originalMaxBoundDst.y - originalLowBoundDst.y)*(destVertices[i].y)/(scaleFactorY)+originalLowBoundDst.y,
 			(originalMaxBoundDst.z - originalLowBoundDst.z)*(destVertices[i].z)/(scaleFactorZ)+originalLowBoundDst.z,
 			destVertices[i].w);			 			
 	}
 }
 
 /**
 * Retrieve a vec3 containing the maximum x,y,z values found in this Vector.
 * @return vec3( maxX, maxY, maxZ )
 */
Angel::vec3 ScaleModel::getMax(std::vector< Angel::vec4 > vertices) {
  
  Angel::vec3 max = Angel::vec3( -INFINITY, -INFINITY, -INFINITY);
  for ( size_t i = 0; i < vertices.size(); ++i ) {
    if ( vertices[i].x > max.x ) max.x = vertices[i].x;
    if ( vertices[i].y > max.y ) max.y = vertices[i].y;
    if ( vertices[i].z > max.z ) max.z = vertices[i].z;
  }

  return max;

}

/**
 * Retrieve a vec3 containing the minimum x,y,z values found in this Vector.
 * @return vec3( minX, minY, minZ )
 */
Angel::vec3 ScaleModel::getMin(std::vector< Angel::vec4 > vertices) {
  
  Angel::vec3 min = Angel::vec3( INFINITY, INFINITY, INFINITY);
  for ( size_t i = 0; i < vertices.size(); ++i ) {
    if ( vertices[i].x < min.x ) min.x = vertices[i].x;
    if ( vertices[i].y < min.y ) min.y = vertices[i].y;
    if ( vertices[i].z < min.z ) min.z = vertices[i].z;
  }

  return min;

}