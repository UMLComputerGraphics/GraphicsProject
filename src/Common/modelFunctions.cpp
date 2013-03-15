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

float threeDimensionalDistance( vec4 points1, vec4 points2 ) {
  return sqrt(
      pow( (points1.x - points2.x), 2 ) + pow( (points1.y - points2.y), 2 )
      + pow( (points1.z - points2.z), 2 ) );
}

int findTriangleWithMinimumDistance( Object* largerModel, Object* smallerModel,
                                     int index ) {
  int minIndex;
  float minDistance = 99999.0;

  for ( int i = 0; i < smallerModel->getNumberPoints(); i += 3 ) {
    float distance;
    for ( int j = 0; j < 3; j++ ) {
      distance += threeDimensionalDistance( smallerModel->points[i + j],
                                            largerModel->points[index + j] );
    }
    if ( distance < minDistance ) {
      minDistance = distance;
      minIndex = i;
    }
  }

  return minIndex;
}

void makeModelsSameSize( Object* model1, Object* model2 ) {
  if ( model1->getNumberPoints() > model2->getNumberPoints() ) {
    while ( model2->getNumberPoints() < (model1->getNumberPoints()) ) {
      int index = findTriangleWithMinimumDistance( model1, model2,
                                                   model2->getNumberPoints() );
      for ( int i = 0; i < 3; i++ ) {
        model2->points.push_back( vec4( model2->points[index + i] ) );
        model2->normals.push_back( vec3( model2->normals[index + i] ) );
        model2->colors.push_back( color4( model2->colors[index + i] ) );
      }
    }
  } else if ( model1->getNumberPoints() < model2->getNumberPoints() ) {
    while ( model1->getNumberPoints() < model2->getNumberPoints() ) {
      int index = findTriangleWithMinimumDistance( model2, model1,
                                                   model1->getNumberPoints() );
      for ( int i = 0; i < 3; i++ ) {
        model1->points.push_back( vec4( model1->points[index + i] ) );
        model1->normals.push_back( vec3( model1->normals[index + i] ) );
        model1->colors.push_back( color4( model1->colors[index + i] ) );
      }
    }
  } else {
    printf( "Models already the same size\n" );
  }
}
