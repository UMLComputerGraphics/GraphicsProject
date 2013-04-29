#include "Engine.hpp"

typedef struct triangle_slice {
  
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
  
  float sentinel;

} triangle_t;
