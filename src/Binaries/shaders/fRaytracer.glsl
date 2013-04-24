#version 150 
// FragmentProgram
const int raytraceDepth = 3;
const float eps  = 0.0001;

uniform mat4 ModelView;

const int maxNumberOfLights = 5;
uniform int uNumberOfLights;
uniform vec3 LightAmbient, uLightPositions[maxNumberOfLights], uLightDiffuse[maxNumberOfLights], uLightSpecular[maxNumberOfLights];

const int maxNumSphere = 5;
uniform int uNumOfSpheres;
uniform vec3 uSphereCenterPoints[maxNumSphere];
uniform float uSphereRadius[maxNumSphere];
uniform vec3 uSphereDiffuse[maxNumSphere];
uniform vec3 uSphereAmbient[maxNumSphere];
uniform vec3 uSphereSpecular[maxNumSphere];
uniform float uSphereShininess[maxNumSphere];
uniform float uSphereReflect[maxNumSphere];
uniform float uSphereRefract[maxNumSphere];

uniform int uNumOfTriangle;
uniform int uNumOfTriangleVectors;

uniform int uNumOfBoundingSpheres;
uniform int uNumOfTrianglesBounded;

uniform samplerBuffer bufferData;

uniform int ftime;

float piover4 = atan(1.0);

const float dlight = 0.025;
float lightness = 0.5;

in vec3 org,dir;

struct Ray
{
	vec3 org;
	vec3 dir;
	float rIndex;
};
struct Sphere
{
	vec3 c;
	float r;
	vec3 diffuse;
};
struct Plane
{
	vec3 p;
	vec3 n;
	vec3 diffuse;
};
struct Triangle
{
	vec3 a;
	vec3 b;
	vec3 c;
	vec3 n;
	vec3 diffuse;
	vec3 ambient;
	vec3 specular;
	float shininess;
	float reflect;
	float refract;
};

struct Intersection
{
	float t;
	vec3 p;	 // hit point
	vec3 n;	 // normal
	int hit;
	vec3 diffuse;
	vec3 ambient;
	vec3 specular;
	float shininess;
	float reflect;
	float refract;
};

void triangle_intersect(in Triangle triangle, in Ray ray, inout Intersection isect)
{
    vec3 edge1 = triangle.b - triangle.a;
    vec3 edge2 = triangle.c - triangle.a;
    vec3 pvec = cross(ray.dir, edge2);
    float det = dot(edge1, pvec);
    
    if (det < 1.0e-6) return;
    
    float invDet = 1 / det;
    
    vec3 tvec = ray.org - triangle.a;
    
    float u = dot(tvec, pvec) * invDet;
    if (u < 0 || u > 1) return;
    
    vec3 qvec = cross(tvec, edge1);
    float v = dot(ray.dir, qvec) * invDet;
    if (v < 0 || u + v > 1) return;
    
    float t = dot(edge2, qvec) * invDet;
    
    if((t > 0.0) && (t < isect.t)) {
		// Point of intersection on the plane
		vec3 p = vec3(ray.org.x + t * ray.dir.x,
					  ray.org.y + t * ray.dir.y,
					  ray.org.z + t * ray.dir.z);

		isect.hit = 1;
		isect.t   = t;
		isect.n   = triangle.n;
		isect.p   = p;
		isect.diffuse = triangle.diffuse;
		isect.ambient = triangle.ambient;
		isect.specular = triangle.specular;
		isect.shininess = triangle.shininess;
		isect.reflect = triangle.reflect;
		isect.refract = triangle.refract;
	}
}

void sphere_intersect(in int c, in Ray ray, inout Intersection isect)
{
	vec3 rs = ray.org - uSphereCenterPoints[c];
	float B = dot(rs, ray.dir);
	float C = dot(rs, rs) - (uSphereRadius[c] * uSphereRadius[c]);
	float D = B * B - C;

	if (D > 0.0)
	{
		float sqrtD = sqrt(D);
		float t = -B - sqrtD;
		if ( (t > 0.0) && (t < isect.t) )
		{
			isect.t = t;
			isect.hit = 1;

			// calculate normal.
			vec3 p = vec3(ray.org.x + ray.dir.x * t,
						  ray.org.y + ray.dir.y * t,
						  ray.org.z + ray.dir.z * t);
			vec3 n = p - uSphereCenterPoints[c];
			n = normalize(n);
			isect.n = n;
			isect.p = p;
			isect.diffuse = uSphereDiffuse[c];
			isect.ambient = uSphereAmbient[c];
			isect.specular = uSphereSpecular[c];
			isect.shininess = uSphereShininess[c];
			isect.reflect = uSphereReflect[c];
			isect.refract = uSphereRefract[c];
		}
	}
}

void plane_intersect(in Plane pl, in Ray ray, inout Intersection isect)
{
	// d = -(p . n)
	// t = -(ray.org . n + d) / (ray.dir . n)
	float d = -dot(pl.p, pl.n);
	float v = dot(ray.dir, pl.n);

	if (abs(v) < 1.0e-6)
		return; // the plane is parallel to the ray.

	float t = -(dot(ray.org, pl.n) + d) / v;

	if ( (t > 0.0) && (t < isect.t) )
	{
		isect.hit = 1;
		isect.t   = t;
		isect.n   = pl.n;

		vec3 p = vec3(ray.org.x + t * ray.dir.x,
					  ray.org.y + t * ray.dir.y,
					  ray.org.z + t * ray.dir.z);
		isect.p = p;
		float offset = 0.2;
		vec3 dp = p + offset;
		if ((mod(dp.x, 1.0) > 0.5 && mod(dp.z, 1.0) > 0.5)
		||  (mod(dp.x, 1.0) < 0.5 && mod(dp.z, 1.0) < 0.5))
			isect.diffuse = pl.diffuse;
		else
			isect.diffuse = pl.diffuse * 0.5;
	}
}

Sphere buildSphere(in vec3 centerPoint, in float radius, in vec3 diffuse) {
	Sphere s;
	s.c = (vec4(centerPoint, 1)).xyz;
	s.r = radius;
	s.diffuse = diffuse;

	return s;
}

void buildTriangle(inout Triangle t, vec3 a, vec3 b, vec3 c, vec3 diffuse, vec3 ambient, vec3 specular, vec3 material, vec3 normal) {
	t.a = a;
	t.b = b;
	t.c = c;
	t.diffuse = diffuse;
	t.ambient = ambient;
	t.specular = specular;
	t.shininess = material.x;
	t.reflect = material.y;
	t.refract = material.z;
	t.n = normal;
}

void IntersectWithHitSpheres(in Ray r, inout Intersection i)
{
	for (int c = 0; c < uNumOfSpheres; c++)
	{
		sphere_intersect(c, r, i);
	}
	
	Triangle triangle;
	int index = uNumOfTriangle * uNumOfTriangleVectors, startingindex = index;
	vec3 centerPoint, rs, triangleNormal, temp;
	float radius, radiusSquared, B, C, D, sqrtOfD, t, tPlus;
	
	//normal modulation guts
	vec3 v1=vec3(1,0,0), v2, v3, v4;
	mat3 m1,m2,tmp;
	float mcos,msin;
	mat4 fromUnit;
  float rotx=piover4*cos(ftime/125.0)/8.0;
  float rotz=piover4*sin(ftime/125.0)/8.0;
  float crotx=cos(rotx),srotx=sin(rotx),crotz=cos(rotz),srotz=sin(rotz);
  float rotmag,cosmagx,sinmagx,cosmagz,sinmagz;
	
	for(int hitIndex = 0 ; hitIndex < uNumOfBoundingSpheres; hitIndex++) {
		centerPoint = (texelFetch(bufferData, index)).xyz;
		radius = texelFetch(bufferData, index+1).x;
		radiusSquared = texelFetch(bufferData, index+1).y;
		
		rs = r.org - centerPoint;
		B = dot(rs, r.dir);
		C = dot(rs, rs) - radiusSquared;
		D = B * B - C;
	
		if (D > 0.0) 
		{
			sqrtOfD = sqrt(D);
			tPlus = -B + sqrtOfD;
			
			if(tPlus > 0.0)
			{
				int pointIndex = hitIndex * uNumOfTriangleVectors * uNumOfTrianglesBounded;
				
				for (int c = 0; c < uNumOfTrianglesBounded && pointIndex < startingindex; c++) 
				{
					centerPoint = (texelFetch(bufferData, pointIndex+8)).xyz;
					radius = texelFetch(bufferData, pointIndex+9).x;
					radiusSquared = texelFetch(bufferData, pointIndex+9).y;
					rs = r.org - centerPoint;
					B = dot(rs, r.dir);
					C = dot(rs, rs) - radiusSquared;
					D = B * B - C;
				
					if (D > 0.0) {
						sqrtOfD = sqrt(D);
						tPlus = -B + sqrtOfD;
						
						if(tPlus > 0.0)
						{
							triangleNormal = texelFetch(bufferData, pointIndex+7).xyz;

              //////////////////////////////////////
              //modulation of triangle normals
              //////////////////////////////////////
              
              //find transformation of (1,0,0) that results in the triangleNormal
              /*v2=triangleNormal;
              v3=normalize(cross(v1,v2));
              v4=cross(v4,v1);
              m1=mat3(v1,v4,v3);
              mcos=dot(v2,v1);
              msin=dot(v2,v4);
              m2=mat3(mcos, msin, 0,
                      -msin, mcos, 0,
                      0, 0, 1);
              tmp=inverse(m1)*m2*m1;
              fromUnit=mat4(vec4(tmp[0], 0.0), vec4(tmp[1], 0.0), vec4(tmp[2], 0.0), vec4(0.0, 0.0, 0.0, 1.0));              
              
              mat4(vec4(1.0,0.0,0.0,0.0),vec4(0.0,cos(rotx),sin(rotx),0.0),vec4(0.0,-sin(rotx),cos(rotx),0.0),vec4(0.0,0.0,0.0,1.0)) *
                                mat4(vec4(cos(rotz),sin(rotz),0.0,0.0),vec4(-sin(rotz),cos(rotz),0.0,0.0),vec4(0.0,0.0,1.0,0.0),vec4(0.0,0.0,0.0,1.0)) * 
                                fromUnit * vec4(1.0,0.0,0.0,1.0)
              */
              
              /*cosmagx=cos(atan(triangleNormal[1]/triangleNormal[0]));
              sinmagx=sin(atan(triangleNormal[1]/triangleNormal[0]));
              cosmagz=cos(atan(triangleNormal[1]/triangleNormal[2]));
              sinmagz=sin(atan(triangleNormal[1]/triangleNormal[2]));*/
              //vec3 ownage = (ModelView * vec4(org,1.0)).xyz;;
              //cosmagx=sinmagx=cosmagz=sinmagz=sqrt((ownage[0]-centerPoint[0])*(ownage[0]-centerPoint[0])+(ownage[1]-centerPoint[1])*(ownage[1]-centerPoint[1]));
              cosmagx=sinmagx=cosmagz=sinmagz=1.0;
              
              //do some noodling
              triangleNormal = normalize(mat3(vec3(1.0,0.0,0.0),vec3(0.0,cosmagx*crotx,sinmagx*srotx),vec3(0.0,-sinmagx*srotx,cosmagx*crotx)) *
                               mat3(vec3(cosmagx*crotx,0.0,sinmagz*srotz),vec3(0.0,1.0,0.0),vec3(-sinmagx*srotx,0.0,cosmagz*crotz)) *
                               mat3(vec3(cosmagz*crotz,sinmagz*srotz,0.0),vec3(-sinmagz*srotz,cosmagz*crotz,0.0),vec3(0.0,0.0,1.0)) 
                                * triangleNormal);
                                
              
              ///////////////////////////////////////
              //END of modulation of triangle normals
              /////////////////////////////////////// 
              
						
							temp = r.dir + triangleNormal;
							if(temp.x < 1.5 && temp.y < 1.5 && temp.z < 1.5)
							{							
								buildTriangle(triangle, texelFetch(bufferData, pointIndex).xyz, texelFetch(bufferData, pointIndex+1).xyz, texelFetch(bufferData, pointIndex+2).xyz, 
																	texelFetch(bufferData, pointIndex+3).xyz, texelFetch(bufferData, pointIndex+4).xyz, texelFetch(bufferData, pointIndex+5).xyz,
																	texelFetch(bufferData, pointIndex+6).xyz,
																	triangleNormal
																	);
								
								triangle_intersect(triangle, r, i);
							}
						}
					}
					
					pointIndex += uNumOfTriangleVectors;
				}
			}
		}
		
		index += 2;
	}
}

vec3 computeLightShadow(in Intersection isect)
{
	int i, j;
	int ntheta = 16;
	int nphi   = 16;
	float eps  = 0.0001;

	// Slightly move ray org towards ray dir to avoid numerical probrem.
	vec3 p = vec3(isect.p.x + eps * isect.n.x,
				  isect.p.y + eps * isect.n.y,
				  isect.p.z + eps * isect.n.z);

	vec3 lightPoint = uLightPositions[0];
	Ray ray;
	ray.org = p;
	ray.dir = normalize(lightPoint - p);
	
	Intersection lisect;
	lisect.hit = 0;
	lisect.t = 1.0e+30;
	lisect.n = lisect.p = lisect.diffuse = vec3(0, 0, 0);
	IntersectWithHitSpheres(ray, lisect);
	if (lisect.hit != 0)
		return vec3(0.0, 0.0, 0.0);
	else
	{
		//Phong shading
		vec3 L = normalize(lightPoint - p);
	    vec3 E = normalize(-p);
	    vec3 H = normalize(L + E);
	    
	    vec3 lightDiffuse = uLightDiffuse[0];
	    
	    float Kd = max( dot(L, isect.n), 0.0 );
	    vec3 diffuse = Kd * lightDiffuse * isect.diffuse;
		
	    float Ks = pow( max(dot(isect.n, H), 0.0), isect.shininess);
	    vec3  specular = Ks * uLightSpecular[0] * isect.specular;
	    if( dot(L, isect.n) < 0.0) {
	        specular = vec3(0.0, 0.0, 0.0);
	    }
	    
		return diffuse + specular;
	}
}

// test intersection functions
void test_main()
{
	Sphere sphere;
	sphere.c   = (vec4(0.0, 0.0, -2.0, 1.0)).xyz;
	sphere.r   = 0.5;
	
	Ray r;
	r.org = org;
	r.dir = normalize(dir);
	vec4 diffuse = vec4(0,0,0,1);
	
	Intersection i;
	i.hit = 0;
	i.t = 1.0e+30;
	i.n = i.p = i.diffuse = vec3(0, 0, 0);
		
	//sphere_intersect(sphere, r, i);
	if (i.hit != 0)
		diffuse = vec4(1,0,0,1);
		
	gl_FragColor = diffuse;
}

// View hit spheres
void test2_main() {

	Ray r;
	r.org = org;
	r.dir = normalize(dir);
	vec4 diffuse = vec4(0,0,0,1);
	
	Intersection i;
	i.hit = 0;
	i.t = 1.0e+30;
	i.n = i.p = i.diffuse = vec3(0, 0, 0);
	
	float eps  = 0.0001;

	int index = uNumOfTriangle * uNumOfTriangleVectors, startingindex = index;
	vec3 centerPoint, rs;
	float radius, radiusSquared, B, C, D, sqrtOfD, t, tPlus;
	for(int hitIndex = 0 ; hitIndex < uNumOfBoundingSpheres; hitIndex++) 
	{
		centerPoint = (texelFetch(bufferData, index)).xyz;
		radius = texelFetch(bufferData, index+1).x;
		radiusSquared = texelFetch(bufferData, index+1).y;
		
		rs = r.org - centerPoint;
		B = dot(rs, r.dir);
		C = dot(rs, rs) - radiusSquared;
		D = B * B - C;
	
		if (D > 0.0) {
			sqrtOfD = sqrt(D);
			t = -B - sqrtOfD;

			float t = -B - sqrt(D);
			if ( (t > 0.0) && (t < i.t) )
			{
				diffuse = vec4(1,1,1,1);
			}
		}
		
		index += 2;
	}
	
	gl_FragColor = diffuse;
}

void main()
{	
	Ray r;
	r.org = org;
	r.dir = normalize(dir);
	vec4 color = vec4(0, 0, 0, 1);
	
	Intersection isect;
	vec3 bcolor = vec3(1,1,1);

	for (int j = 0; j < raytraceDepth; j++)
	{
		isect.hit = 0;
		isect.t = 1.0e+30;
		isect.n = isect.p = isect.diffuse = vec3(0, 0, 0);
			
		IntersectWithHitSpheres(r, isect);
		if (isect.hit != 0)
		{
			color.rgb += isect.ambient + bcolor * computeLightShadow(isect);
			bcolor *= isect.reflect * isect.diffuse;
		}
		else
		{
			break;
		}
		
		if(isect.reflect < eps) break;

		r.org = vec3(isect.p.x + eps * isect.n.x,
					 isect.p.y + eps * isect.n.y,
					 isect.p.z + eps * isect.n.z);
		r.dir = reflect(r.dir, vec3(isect.n.x, isect.n.y, isect.n.z));
	}

	gl_FragColor = color;
}