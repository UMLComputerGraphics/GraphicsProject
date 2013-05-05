#ifndef RAYTRACE_BUFFER_H
#define RAYTRACE_BUFFER_H

#include "vec.hpp"
using Angel::vec3;

typedef struct triangle_slice {

  // This structure needs to remain tightly packed:
  // E.g, if you insert floats inbetween vec3s,
  // You need to use three floats to keep the vec3s on proper boundaries.
  // In addition, try not to insert anything! add it after the sentinel!
  
  vec3 a;
  vec3 b;
  vec3 c;
  
  vec3 diffuse;
  vec3 ambient;
  vec3 specular;

  float shininess;
  float reflect;
  float refract;

  vec3 normal;

  float centerx;
  float centery;
  float centerz;

  float distance;
  float distanceSquared;
  float padding;

} triangle_t;

#endif
