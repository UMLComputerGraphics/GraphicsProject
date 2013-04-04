#version 150 
// FragmentProgram
const int raytraceDepth = 3;

uniform mat4 ModelView;

const int numberOfLights = 5;
uniform vec4 LightAmbient, LightPosition[numberOfLights], LightDiffuse[numberOfLights], LightSpecular[numberOfLights];

const int maxNumSphere = 5;
uniform int uNumOfSpheres;
uniform vec3 uSphereCenterPoints[maxNumSphere];
uniform float uSphereRadius[maxNumSphere];
uniform vec3 uSphereColors[maxNumSphere];

const int numOfTriangleVectors = 5;
uniform int uNumOfTriangle;

uniform int uNumOfBoundingSpheres;
uniform int uNumOfTrianglesBounded;

uniform samplerBuffer bufferData;

uniform float ftime;

const float dlight = 0.025;
float lightness = 0.5;

in vec3 org,dir;

struct Ray
{
	vec3 org;
	vec3 dir;
};
struct Sphere
{
	vec3 c;
	float r;
	vec3 color;
};
struct Plane
{
	vec3 p;
	vec3 n;
	vec3 color;
};
struct Triangle
{
	vec3 a;
	vec3 b;
	vec3 c;
	vec3 n;
	vec3 color;
};

struct Intersection
{
	float t;
	vec3 p;	 // hit point
	vec3 n;	 // normal
	int hit;
	vec3 color;
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
		isect.color = triangle.color;
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
		float t = -B - sqrt(D);
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
			isect.color = uSphereColors[c];
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
			isect.color = pl.color;
		else
			isect.color = pl.color * 0.5;
	}
}

Sphere buildSphere(in vec3 centerPoint, in float radius, in vec3 color) {
	Sphere s;
	s.c = (vec4(centerPoint, 1)).xyz;
	s.r = radius;
	s.color = color;

	return s;
}

void buildTriangle(inout Triangle t, vec3 a, vec3 b, vec3 c, vec3 color, vec3 normal) {
	t.a = (vec4(a, 1.0)).xyz;
	t.b = (vec4(b, 1.0)).xyz;
	t.c = (vec4(c, 1.0)).xyz;
	t.color = color;
	t.n = normal;
}

Plane plane;

void IntersectWithHitSpheres(in Ray r, inout Intersection i)
{
	for (int c = 0; c < uNumOfSpheres; c++)
	{
		sphere_intersect(c, r, i);
	}
	
	Triangle triangle;
	int index = uNumOfTriangle * numOfTriangleVectors, startingindex = index;
	vec3 centerPoint, rs;
	float radius, B, C, D, sqrtOfD, t, tPlus;
	for(int hitIndex = 0 ; hitIndex < uNumOfBoundingSpheres; hitIndex++) {
		centerPoint = (texelFetch(bufferData, index)).xyz;
		radius = texelFetch(bufferData, index+1).x;
		index += 2;
		rs = r.org - centerPoint;
		B = dot(rs, r.dir);
		C = dot(rs, rs) - (radius * radius);
		D = B * B - C;
	
		if (D > 0.0) {
			sqrtOfD = sqrt(D);
			t = -B - sqrtOfD;
			tPlus = B + sqrtOfD;
			
			// Check if the sphere is closer than the current intersection or the ray is inside the sphere
			if((t > 0.0) && (t < i.t) || (t < 0.0) && (tPlus > 0.0))
			{
				int pointIndex = hitIndex * numOfTriangleVectors * uNumOfTrianglesBounded;
				
				for (int c = 0; c < uNumOfTrianglesBounded && pointIndex < startingindex; c++)
				{
					buildTriangle(triangle, texelFetch(bufferData, pointIndex).xyz, texelFetch(bufferData, pointIndex+1).xyz, texelFetch(bufferData, pointIndex+2).xyz, 
														texelFetch(bufferData, pointIndex+3).xyz, texelFetch(bufferData, pointIndex+4).xyz);
					pointIndex += 5;
					triangle_intersect(triangle, r, i);
				}
			}
		}
	}
	
	//plane_intersect(plane, r, i);
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

	vec3 lightPoint = (vec4(0, 0, 5, 1)).xyz;
	Ray ray;
	ray.org = p;
	ray.dir = normalize(lightPoint - p);

	Intersection lisect;
	lisect.hit = 0;
	lisect.t = 1.0e+30;
	lisect.n = lisect.p = lisect.color = vec3(0, 0, 0);
	IntersectWithHitSpheres(ray, lisect);
	if (lisect.hit != 0)
		return vec3(0.0,0.0,0.0);
	else
	{
		float shade = max(0.0, dot(isect.n, ray.dir));
		shade = pow(shade,3.0) + shade * 0.5;
		return vec3(shade,shade,shade);
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
	vec4 color = vec4(0,0,0,1);
	
	Intersection i;
	i.hit = 0;
	i.t = 1.0e+30;
	i.n = i.p = i.color = vec3(0, 0, 0);
		
	//sphere_intersect(sphere, r, i);
	if (i.hit != 0)
		color = vec4(1,0,0,1);
		
	gl_FragColor = color;
}

float rand(in vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main()
{	
	plane.p = (vec4(0,-0.5, 0, 1.0)).xyz;
	plane.n = (vec4(0, 1.0, 0, 1.0)).xyz;
	plane.color = vec3(1,1, 1);
	
	Ray r;
	r.org = org;
	r.dir = normalize(dir);
	vec4 color = vec4(0,0,0,1);
	
	Intersection i;
	
	float eps  = 0.0001;
	float R = rand(vec2(-ftime,ftime));
	float d = floor(R) == round(R) ? dlight : -dlight;
	lightness += d;
	if (lightness < 0) lightness = 0;
	if (lightness > 1.0) lightness = 1.0;
	vec3 bcolor = lightness * vec3(1,1,1);
	for (int j = 0; j < raytraceDepth; j++)
	{
		i.hit = 0;
		i.t = 1.0e+30;
		i.n = i.p = i.color = vec3(0, 0, 0);
			
		IntersectWithHitSpheres(r, i);
		if (i.hit != 0)
		{
			color.rgb += bcolor * i.color * computeLightShadow(i);
			bcolor *= i.color;
		}
		else
		{
			break;
		}
				
		r.org = vec3(i.p.x + eps * i.n.x,
					 i.p.y + eps * i.n.y,
					 i.p.z + eps * i.n.z);
		r.dir = reflect(r.dir, vec3(i.n.x, i.n.y, i.n.z));
	}
	gl_FragColor = color;
}
