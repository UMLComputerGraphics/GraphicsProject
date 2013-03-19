#version 150 
// FragmentProgram
const int raytraceDepth = 8;

uniform mat4 ModelView;

const int maxNumSphere = 5;
uniform int uNumOfSpheres;
uniform vec3 uSphereCenterPoints[maxNumSphere];
uniform float uSphereRadius[maxNumSphere];
uniform vec3 uSphereColors[maxNumSphere];

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

void triangle_intersect(Triangle triangle, Ray ray, inout Intersection isect)
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

void sphere_intersect(Sphere s, Ray ray, inout Intersection isect)
{
	vec3 rs = ray.org - s.c;
	float B = dot(rs, ray.dir);
	float C = dot(rs, rs) - (s.r * s.r);
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
			vec3 n = p - s.c;
			n = normalize(n);
			isect.n = n;
			isect.p = p;
			isect.color = s.color;
		}
	}
}

void plane_intersect(Plane pl, Ray ray, inout Intersection isect)
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

Sphere buildSphere(vec3 centerPoint, float radius, vec3 color) {
	Sphere s;
	s.c = (ModelView * vec4(centerPoint, 1)).xyz;
	s.r = radius;
	s.color = color;

	return s;
}

Triangle buildTriangle(vec3 a, vec3 b, vec3 c, vec3 color) {
	Triangle t;

	t.a = (ModelView * vec4(a, 1.0)).xyz;
	t.b = (ModelView * vec4(b, 1.0)).xyz;
	t.c = (ModelView * vec4(c, 1.0)).xyz;
	t.color = color;
	vec3 N = cross((a - b), (a - c)); 
	t.n = normalize(N);
	
	return t;
}

Sphere sphere[8];
Triangle triangle[6];
Plane plane;
void Intersect(Ray r, inout Intersection i)
{
	for (int c = 0; c < uNumOfSpheres; c++)
	{
		sphere_intersect(buildSphere(uSphereCenterPoints[c], uSphereRadius[c], uSphereColors[c]), r, i);
	}
	
	for (int c = 0; c < 2; c++)
	{
		triangle_intersect(triangle[c], r, i);
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

	vec3 lightPoint = (ModelView * vec4(0, 0, 5, 1)).xyz;
	Ray ray;
	ray.org = p;
	ray.dir = normalize(lightPoint - p);

	Intersection lisect;
	lisect.hit = 0;
	lisect.t = 1.0e+30;
	lisect.n = lisect.p = lisect.color = vec3(0, 0, 0);
	Intersect(ray, lisect);
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
	sphere.c   = (ModelView * vec4(0.0, 0.0, -2.0, 1.0)).xyz;
	sphere.r   = 0.5;
	
	triangle[0] = buildTriangle(vec3(-2.0, -2.0, -1), vec3(2.0, -2.0, -1), vec3(2.0, 2.0, -1), vec3(1, 0, 1));
	triangle[1] = buildTriangle(vec3(-2.0, 2.0, -1), vec3(-2.0, -2.0, -1), vec3(2.0, 2.0, -1), vec3(0, 1, 1));
	
	Ray r;
	r.org = org;
	r.dir = normalize(dir);
	vec4 color = vec4(0,0,0,1);
	
	Intersection i;
	i.hit = 0;
	i.t = 1.0e+30;
	i.n = i.p = i.color = vec3(0, 0, 0);
	
	
	triangle_intersect(triangle[0], r, i);
	if (i.hit != 0)
		color = vec4(triangle[0].color, 1);
		
	i.hit = 0;
	i.t = 1.0e+30;
	
	triangle_intersect(triangle[1], r, i);
	if (i.hit != 0)
		color = vec4(triangle[1].color, 1);
		
	i.hit = 0;
	i.t = 1.0e+30;
		
	//sphere_intersect(sphere, r, i);
	if (i.hit != 0)
		color = vec4(1,0,0,1);
		
	gl_FragColor = color;
}

void main()
{
	sphere[0] = buildSphere(vec3(-1, -1, -2), .5, vec3(1,0.3,0.3));
	sphere[1] = buildSphere(vec3(1, -1, -2), .5, vec3(0.3,1,0.3));
	
	sphere[2] = buildSphere(vec3(-1, 1, -2), .5, vec3(0.3,0.3,1));
	sphere[3] = buildSphere(vec3(1, 1, -2), .5, vec3(1,0.3,0.3));
	
	sphere[4] = buildSphere(vec3(-1, -1, -1), .5, vec3(0.3,1,0.3));
	sphere[5] = buildSphere(vec3(1, -1, -1), .5, vec3(0.3,0.3,1));
	
	sphere[6] = buildSphere(vec3(-1, 1, -1), .5, vec3(0.3,1,0.3));
	sphere[7] = buildSphere(vec3(1, 1, -1), .5, vec3(0.3,0.3,1));

	triangle[0] = buildTriangle(vec3(-2.0, -2.0, -5), vec3(2.0, -2.0, -5), vec3(2.0, 2.0, -5), vec3(1, 0, 1));
	triangle[1] = buildTriangle(vec3(-2.0, 2.0, -5), vec3(-2.0, -2.0, -5), vec3(2.0, 2.0, -5), vec3(0, 1, 1));
	
	triangle[2] = buildTriangle(vec3(0.0, 2.0, -5), vec3(-2.0, 0.0, -5), vec3(0.0, -2.0, -5), vec3(1, 1, 0));
	triangle[3] = buildTriangle(vec3(0.0, 2.0, -5), vec3(0.0, -2.0, -5), vec3(2.0, 0.0, -5), vec3(1, 1, 1));
	
	plane.p = (ModelView * vec4(0,-0.5, 0, 1.0)).xyz;
	plane.n = (vec4(0, 1.0, 0, 1.0)).xyz;
	plane.color = vec3(1,1, 1);
	
	Ray r;
	r.org = org;
	r.dir = normalize(dir);
	vec4 color = vec4(0,0,0,1);
	
	Intersection i;
	i.hit = 0;
	i.t = 1.0e+30;
	i.n = i.p = i.color = vec3(0, 0, 0);
	
	float eps  = 0.0001;
	vec3 bcolor = vec3(1,1,1);
	for (int j = 0; j < raytraceDepth; j++)
	{
		Intersection i;
		i.hit = 0;
		i.t = 1.0e+30;
		i.n = i.p = i.color = vec3(0, 0, 0);
			
		Intersect(r, i);
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