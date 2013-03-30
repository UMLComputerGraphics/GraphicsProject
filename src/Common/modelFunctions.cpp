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

void makeModelsSameSize( Object* model1, Object* model2 ) {
  if ( model1->numberOfPoints() > model2->numberOfPoints() ) {
    while ( model2->numberOfPoints() < (model1->numberOfPoints()) ) {
      int index = findTriangleWithMinimumDistance( model1, model2,
                                                   model2->numberOfPoints() );
      for ( int i = 0; i < 3; i++ ) {
        model2->_vertices.push_back( vec4( model2->_vertices[index + i] ) );
        model2->_normals.push_back( vec3( model2->_normals[index + i] ) );
        model2->_colors.push_back( Color4( model2->_colors[index + i] ) );
      }
    }
  } else if ( model1->numberOfPoints() < model2->numberOfPoints() ) {
    while ( model1->numberOfPoints() < model2->numberOfPoints() ) {
      int index = findTriangleWithMinimumDistance( model2, model1,
                                                   model1->numberOfPoints() );
      for ( int i = 0; i < 3; i++ ) {
        model1->_vertices.push_back( vec4( model1->_vertices[index + i] ) );
        model1->_normals.push_back( vec3( model1->_normals[index + i] ) );
        model1->_colors.push_back( Color4( model1->_colors[index + i] ) );
      }
    }
  } else {
    printf( "Models already the same size\n" );
  }
}
