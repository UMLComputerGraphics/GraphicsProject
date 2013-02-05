/*

  model.cpp

  code to generate/load models

 */

#include "globals.h"
#include "Object.hpp"
#include "Timer.hpp"
#include "vec.hpp"
#include <cmath>
using namespace Angel;
typedef Angel::vec4  color4;
typedef Angel::vec4  point4;

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>


using std::vector       ;
using std::fstream      ;
using std::ifstream      ;
using std::istringstream  ;
using std::string        ;
using std::ios          ;
using std::cerr         ;
using std::endl         ;


void createPoint( Object *obj, point4 const &the_point, 
		  color4 const &the_color, vec3 const &the_normal ) {

  obj->points.push_back( the_point );
  obj->colors.push_back( the_color );
  obj->normals.push_back( the_normal );

}

/** Functions relating to the Sierpinski Pyramid **/

void triangle( Object *obj, const point4& a, const point4& b, 
	       const point4& c, const int color ) {

  static vec4  base_colors[] = {
    vec4( 1.0, 0.0, 0.0, 1.0 ), /* R */
    vec4( 0.0, 1.0, 0.0, 1.0 ), /* G */
    vec4( 0.0, 0.0, 1.0, 1.0 ), /* B */
    vec4( 1.0, 1.0, 0.0, 1.0 ), /* Y */
    vec4( 0.4, 0.1, 0.8, 1.0 ),  /* P */
    vec4( 1.0, 1.0, 1.0, 1.0 )  /* W */
  };

  // Initialize temporary vectors along the quad's edge to
  //   compute its face normal

  vec3 u = normalize(vec3(b.x-a.x, b.y-a.y, b.z-a.z)) ;
  vec3 v = normalize(vec3(c.x-b.x, c.y-b.y, c.z-b.z)) ;

  vec3 normal = normalize( cross(u, v) );

  createPoint( obj, a, base_colors[ color ], normal );
  createPoint( obj, b, base_colors[ color ], normal );
  createPoint( obj, c, base_colors[ color ], normal );

}

void tetra( Object *obj,
	    const point4& a, const point4& b, 
	    const point4& c, const point4& d ) {

  triangle( obj, a, b, c, 0 );
  triangle( obj, a, c, d, 1 );
  triangle( obj, a, d, b, 2 );  // old   triangle( a, d, b, 2 );
  triangle( obj, d, c, b, 3 );

}

///////////////////////////////////////////////////////////////////////////////

// I have no idea where this comes from, what it does, why need it. etc.

point4 unit(const point4 &p)
{
  point4 c ;
  double d = 0.0;

  for( int i=0; i<3; i++ ) d+=p[i]*p[i];

  d=sqrt(d);

  if( d > 0.0 ) for( int i=0; i<3; i++ ) c[i] = p[i]/d;

  c[3] = 1.0;

  return c;

}

point4 initialSpherePoints[4] = { point4(0.0, 0.0, 1.0, 1.0),
				  point4(0.0, 0.942809, -0.333333, 1.0),
				  point4(-0.816497, -0.471405, -0.333333, 1.0),
				  point4(0.816497, -0.471405, -0.333333, 1.0)};

void divide_triangle(Object *obj, const point4& a, const point4& b, const point4& c, int timesToRecurse, int color )
{

  point4 v1, v2, v3;

  if ( timesToRecurse > 0 )
    {
      v1 = unit( a + b ) ;
      v2 = unit( a + c ) ;
      v3 = unit( b + c ) ;

      divide_triangle( obj, a , v2, v1, timesToRecurse-1, color );
      divide_triangle( obj, c , v3, v2, timesToRecurse-1, color );
      divide_triangle( obj, b , v1, v3, timesToRecurse-1, color );
      divide_triangle( obj, v1, v2, v3, timesToRecurse-1, color );
    }
  else triangle( obj, a, b, c, color ) ;
}


void recursiveModelGen( Object *obj,
		        const point4& a, const point4& b, 
		        const point4& c, const point4& d,
		        int timesToRecurse,
			int color ) {

  divide_triangle( obj, a, b, c, timesToRecurse, color );
  divide_triangle( obj, d, c, b, timesToRecurse, color );
  divide_triangle( obj, a, d, b, timesToRecurse, color );
  divide_triangle( obj, a, d, c, timesToRecurse, color );
}

/**
   Creates a white sphere.
   @param obj the object-object to create/push verticies and colors to
 */

void sphere( Object *obj ){


  recursiveModelGen(obj,
		    initialSpherePoints[0],
		    initialSpherePoints[1],
		    initialSpherePoints[2],
		    initialSpherePoints[3],
		    4,
		    5 ) ;

}

///////////////////////////////////////////////////////////////////////////////


void Sierpinski_Pyramid( Object *obj,
			 const point4& a,
			 const point4& b,
			 const point4& c,
			 const point4& d,
			 int count ) {
  if ( count > 0 ) {
    point4 v0 = ( a + b ) / 2.0;
    point4 v1 = ( a + c ) / 2.0;
    point4 v2 = ( a + d ) / 2.0;
    point4 v3 = ( b + c ) / 2.0;
    point4 v4 = ( c + d ) / 2.0;
    point4 v5 = ( b + d ) / 2.0;
    Sierpinski_Pyramid( obj, a, v0, v1, v2, count - 1 );
    Sierpinski_Pyramid( obj, v0,  b, v3, v5, count - 1 );
    Sierpinski_Pyramid( obj, v1, v3,  c, v4, count - 1 );
    Sierpinski_Pyramid( obj, v2, v5, v4,  d, count - 1 );
  } else {
    tetra( obj, a, b, c, d );   // draw tetrahedron at end of recursion
  }
}

//--------------------------------------------------------------------

void quad( Object *obj, 
	   const point4 &a, const point4 &b,
	   const point4 &c, const point4 &d,
	   const color4 &A, const color4 &B,
	   const color4 &C, const color4 &D ) {


  // Initialize temporary vectors along the quad's edge to                                                                                 
  //   compute its face normal                                                                                                             
  vec4 u = b - a;
  vec4 v = c - b;

  vec3 normal = normalize( cross(u, v) );

  createPoint( obj, a, A, normal );
  createPoint( obj, b, B, normal );
  createPoint( obj, c, C, normal );

  createPoint( obj, a, A, normal );
  createPoint( obj, c, C, normal );
  createPoint( obj, d, D, normal );

}

//--------------------------------------------------------------------

#define QUAD(A,B,C,D)							\
  quad( obj, vertices[A], vertices[B], vertices[C], vertices[D],	\
	colors[A], colors[B], colors[C], colors[D] );

void cube( Object *obj, const GLfloat &size, const color4 colors[8] ) {

  const GLfloat lower = -(size/2);
  const GLfloat upper = (size/2);

  const point4 vertices[8] = {
    point4( lower, lower, upper, 1.0 ),
    point4( lower, upper, upper, 1.0 ),
    point4( upper, upper, upper, 1.0 ),
    point4( upper, lower, upper, 1.0 ),
    point4( lower, lower, lower, 1.0 ),
    point4( lower, upper, lower, 1.0 ),
    point4( upper, upper, lower, 1.0 ),
    point4( upper, lower, lower, 1.0 )
  };
  
  QUAD( 1, 0, 3, 2 );
  QUAD( 2, 3, 7, 6 );
  QUAD( 3, 0, 4, 7 );
  QUAD( 6, 5, 1, 2 );
  QUAD( 4, 5, 6, 7 );
  QUAD( 5, 4, 0, 1 );
  
}

void colorcube( Object *obj, GLfloat size ) {

  static const color4 vertex_colors[8] = {
    color4( 0.0, 0.0, 0.0, 1.0 ),  // black
    color4( 1.0, 0.0, 0.0, 1.0 ),  // red
    color4( 1.0, 1.0, 0.0, 1.0 ),  // yellow
    color4( 0.0, 1.0, 0.0, 1.0 ),  // green
    color4( 0.0, 0.0, 1.0, 1.0 ),  // blue
    color4( 1.0, 0.0, 1.0, 1.0 ),  // magenta
    color4( 1.0, 1.0, 1.0, 1.0 ),  // white
    color4( 0.0, 1.0, 1.0, 1.0 )   // cyan
  };

  cube( obj, size, vertex_colors );

}

//--------------------------------------------------------------------


/**
 model loader:
 Shamelessly ripped straight from the Simiandroid's main source file!

  .obj model loader
  models must be triangulated, and have normals included to be
  correctly parsed
  vertices, normals, vertex elements, and normal elements will be returned to pointers to vectors
*/


/*
  Basic function to split strings over a delimiter
  used in model parsing
*/

const vector<string> split(const string &str, const char delim) {
  std::vector<string> elements;
  std::string tmp = str;

  //vector<string> *elements = new vector<string>();
  // loop through and break string up by the deliminator
  while (tmp.find_first_of(delim) != string::npos) {
      // position of deliminator in current string
      int pos = tmp.find_first_of(delim);
      elements.push_back(tmp.substr(0, pos));
      tmp = tmp.substr(pos + 1);
    }

  // str will have the final element remaining in it
  elements.push_back(tmp);
  return elements;
}

void load_obj(const char* filename, vector<vec4> &vertices,
	      vector<vec3> &normals, vector<GLushort> &v_elements,
	      vector<GLushort> &n_elements)
{
  // file input stream
  ifstream in(filename, ios::in);

  if (!in)
    {
      cerr << "Cannot open " << filename << endl;
      exit(1);
    }

  string line;

  while (getline(in, line))
    {
      // lines beginning with 'v ' have vertices
      if (line.substr(0, 2) == "v ")
	{
	  // read in vertex coords
	  istringstream s(line.substr(2));
	  vec4 v;
	  s >> v.x;
	  s >> v.y;
	  s >> v.z;
	  v.w = 1.0;
	    
	  vertices.push_back(v);
	}
      // lines beginning with 'f ' have elements
      else if (line.substr(0, 2) == "f ")
	{
	  istringstream s(line.substr(2));
	  string a, b, c;
	  s >> a;
	  s >> b;
	  s >> c;

	  // split elements by '/'
	  v_elements.push_back(atoi(split(a, '/')[0].c_str()));
	  v_elements.push_back(atoi(split(b, '/')[0].c_str()));
	  v_elements.push_back(atoi(split(c, '/')[0].c_str()));

	  n_elements.push_back(atoi(split(a, '/')[2].c_str()));
	  n_elements.push_back(atoi(split(b, '/')[2].c_str()));
	  n_elements.push_back(atoi(split(c, '/')[2].c_str()));
	}
      // lines beginning with 'vn ' will have normals
      else if (line.substr(0, 3) == "vn ") {
	istringstream s(line.substr(3));
	vec3 vn;
	s >> vn.x;
	s >> vn.y;
	s >> vn.z;
	  
	normals.push_back(vn);
      }
      else if (line[0] == '#') { /* ignore comments */ }
      else { /* blank/junk */ }
    }

}

float rand_float( void ) ;

void loadModelFromFile( Object *obj, const char* filename )
{

  /*
    void load_obj(const char* filename, vector<vec4> &vertices,
              vector<vec3> &normals, vector<GLushort> &v_elements,
              vector<GLushort> &n_elements)
  */

  vector<vec4> raw_vertices;
  vector<vec4> vertices;
  vector<vec3> normals;
  vector<vec3> raw_normals;
  vector<GLushort> v_elements;
  vector<GLushort> n_elements;
  vector<vec4> colors;


  load_obj(filename, raw_vertices, raw_normals, v_elements, n_elements);

  if ( v_elements.size() != n_elements.size() ) { 
    fprintf( stderr,
	     "\nload_obj: loaded different number of normals and vertices\n"); 
  }

  for(unsigned int i = 0; i < v_elements.size() ; i ++ ){
    colors.push_back(color4(rand_float(),
			    rand_float(),
			    rand_float(),
			    1.0));
  }

  //push vertices obtained from the model loader in the order specified in the .obj elements section
  for (unsigned int i = 0; i < v_elements.size(); i++)
    {
      obj->points.push_back(raw_vertices[v_elements[i] - 1]);
      obj->colors.push_back(colors[v_elements[i]-1]);

    }
  //numVertices = vertices.size();


  //push normals obtained from the model loader in the order specified in the .obj elements section
  for (unsigned int i = 0; i < n_elements.size(); i++)
    {
      obj->normals.push_back(raw_normals[n_elements[i] - 1]);
    }
  

  //colors.resize(vertices.size());
  /*for (unsigned int i = 0; i < obj->points.size(); i++)
    {
      obj->colors.push_back(
    }
  */
  /*
  std::vector<Angel::vec4> points;
  std::vector<Angel::vec3> normals;
  std::vector<unsigned int> indices;
  std::vector<Angel::vec4> colors;
  std::vector<Angel::vec2> texcoords;
  */
  
}


// Helpers for landGen:

float rand_float( void ) {
  return rand() / (float)RAND_MAX;
}

double jitter( double H ) {
  return (-H) + rand() * (H - (-H)) / RAND_MAX;
}

vec3 calcNormal( point4 &a, point4 &b, point4 &c ) {
  return cross( c-b, b-a ) ;
}

vec3 calcNormal( point4 &a, point4 &b, point4 &c, point4 &d ) {
  return cross( c-b, b-a ) ;
}


void makeAgua( Object *land_obj, Object *agua_obj ) {

  float wh = 0.1;

  // Ensure that the size of agua_obj == land_obj
  int S = sqrt( land_obj->points.size() );
  
  // Push the vertices
  agua_obj->points.push_back( vec4( -S/2, wh , S/2, 1) );
  agua_obj->points.push_back( vec4( -S/2, wh ,-S/2, 1) );
  agua_obj->points.push_back( vec4(  S/2, wh , S/2, 1) );
  agua_obj->points.push_back( vec4(  S/2, wh , S/2, 1) );
  agua_obj->points.push_back( vec4( -S/2, wh ,-S/2, 1) );
  agua_obj->points.push_back( vec4(  S/2, wh ,-S/2, 1) );

  agua_obj->colors.push_back( vec4( 0.0, 0.0, 0.8, 0.6 ) );
  agua_obj->colors.push_back( vec4( 0.0, 0.0, 0.8, 0.6 ) );
  agua_obj->colors.push_back( vec4( 0.0, 0.0, 0.8, 0.6 ) );
  agua_obj->colors.push_back( vec4( 0.0, 0.0, 0.8, 0.6 ) );
  agua_obj->colors.push_back( vec4( 0.0, 0.0, 0.8, 0.6 ) );
  agua_obj->colors.push_back( vec4( 0.0, 0.0, 0.8, 0.6 ) );

  return;
}

double landGen( Object *obj, int N, float H ) {

  Timer Tick;
  const int S = pow(2,N) + 1;
  std::vector< point4 > &vec = obj->points;
  std::vector< point4 > &col = obj->colors;
  //std::vector< vec3 > &nor = obj->normals;
  std::vector< unsigned int > &drawIndex = obj->indices;
  std::vector< Angel::vec2 > &txy = obj->texcoords;

  if (DEBUG) printf( "\nEntering landGen()...\n");
  // the range (-h -> h) for the average offset
  // This determines the jaggedness of the peaks and valleys.
  // A smaller initial value will create smaller peaks and shallow valleys for
  // a look more similar to rolling hill tops.
  double CH = 0.5;
  double magnitude = (H*(2 - pow(2,-(N))));
  if (DEBUG) fprintf( stderr, "landGen theoretical magnitude: %f\n", magnitude );

  /* Initialize all points in the vector to have their X,Z (and w) coordinates. */
  if (vec.size()) vec.clear();
  vec.reserve( S * S );
  for ( int i = 0; i < S; ++i )
    for ( int j = 0; j < S; ++j ) 
      vec.push_back( vec4( j - (S/2), 0, i - (S/2), 1 ) );

  /* Initialize our color vectors. */
  if (col.size()) col.clear();
  col.reserve( S*S );
  for ( int i = 0; i < S; ++i )
    for ( int j = 0; j < S; ++j )
      col.push_back( vec4( 0.5, 0.5, 0.5, 1.0 ) );

  if (txy.size()) txy.clear();
  txy.reserve( S*S );
  for ( int z = 0; z < S; ++z )
    for ( int x = 0; x < S; ++x )
      // Set coordinates so that the texture applied to 
      // smaller model coords.
      // Prevent texture from being stretched to fit model.
      txy.push_back(vec2( fmod(100*(float)x/(float)S, 100), 
			  fmod(100*(float)z/(float)S, 100)));
  

  /* Simulate a 2D array in this 1D array. Use these Macros to help. */
#define OffsetAt(X,Z) ((X)*S+(Z))
#define VertexAt(X,Z) (vec.at(OffsetAt(X,Z)))
#define HeightAt(X,Z) (VertexAt(X,Z).y)
#define ColorAt(X,Z) (col.at(OffsetAt(X,Z)))
#define TXYAt(X,Z) (txy.at(OffsetAt(X,Z)))
#define NormalAt(X,Z) (nor.at(OffsetAt(X,Z)))

  // Initialize the corners of the grid
  HeightAt( 0, 0 )     = 0;
  HeightAt( S-1, 0 )   = 0;
  HeightAt( 0, S-1 )   = 0;
  HeightAt( S-1, S-1 ) = 0;

  // Populate the (x, y, z) values of vec4 according to the
  // Diamond-Square algorithm
  // sideLength is the distance of a single square side or
  // distance of diagonal in diamond.
  if (DEBUG) printf("\nEntering for( sideLength...) ...\n");
  for (int sideLength = S-1; sideLength >= 2; sideLength /= 2, H /= 2.0, CH /= 2.0) { 
    int halfSide = sideLength / 2;
    // generate new square values
    for ( int x = 0 ; x < S-1 ; x += sideLength ) {
      for ( int z = 0 ; z < S-1 ; z += sideLength) { 
	double avg =						\
	  (HeightAt( x, z ) + HeightAt( x + sideLength, z ) +
	   HeightAt( x, z + sideLength ) + HeightAt( x + sideLength, z + sideLength ))/4.0;

	vec4 color_avg =				\
	  (ColorAt(x,z) + ColorAt(x+sideLength,z) +
	   ColorAt(x,z+sideLength) + ColorAt(x+sideLength,z+sideLength))/4.0;
	vec4 color_jitter = vec4( jitter(CH), jitter(CH), jitter(CH), 0 );

	HeightAt( x + halfSide, z + halfSide ) = avg + jitter(H);
	ColorAt( x + halfSide, z + halfSide ) = color_avg + color_jitter;
      } // for z
    } // for x

    // Generate the diamond values
    // Since diamonds are staggered, we only move x by half side
    // NOTE: If the data shouldn't wrap the x < SIZE and y < SIZE
    for ( int x = 0 ; x < S-1 ; x += halfSide ) {
      for (int z = ( x+halfSide ) % sideLength ; z < S-1 ; z += sideLength ) {

        // x,z is center of diamond
        // We must use mod and add SIZE for subtraction
        // so that we can wrap around the array to find the corners

	double avg = 
	  (HeightAt( (x-halfSide + S) % S, z ) +
	   HeightAt( x + halfSide % S, z ) +
	   HeightAt( x, z + halfSide % S ) +
	   HeightAt( x, (z - halfSide + S) % S )) / 4.0;

	vec4 color_avg = 
	  (ColorAt( (x-halfSide + S) % S, z ) +
	   ColorAt( x + halfSide % S, z ) +
	   ColorAt( x, z + halfSide % S ) +
	   ColorAt( x, (z - halfSide + S) % S )) / 4.0;
	vec4 color_jitter = vec4( jitter(CH), jitter(CH), jitter(CH), 0 );

	HeightAt( x, z ) = avg + jitter(H);
	ColorAt( x, z ) = color_avg + color_jitter;

	// Wrapping:
	if (x == 0) {
	  HeightAt( S-1, z ) = HeightAt( x, z );
	  ColorAt( S-1, z ) = ColorAt( x, z );
	}
	if (z == 0) {
	  HeightAt( x, S-1 ) = HeightAt( x, z );
	  ColorAt( x, S-1 ) = ColorAt( x, z );
	}
      } // for z
    } // for x
  } // for sideLength


  //vec.reserve(2*S*S - 2*S);
  // Convert 2D array into 1D array to pass to shaders
  // Size of 1D array is 2*SIZE^2 - 2SIZE for
  // the sake of proper parsing by glTriangleStrips
  if (drawIndex.size()) drawIndex.clear();
  drawIndex.reserve( 2*S*S - 2*S );
  for ( int i = 0, j = 0 ; i + 1 < S ; i++ ) {
    for ( j = 0 ; j < S ; j++ ) {
      drawIndex.push_back(OffsetAt(i,j));
      drawIndex.push_back(OffsetAt(i+1,j));
    }

    /* If we're out of rows to serialize, give up. */
    if (++i == S) break;

    for ( ; j > 0 ; j-- ){
      drawIndex.push_back(OffsetAt(i,j-1));
      drawIndex.push_back(OffsetAt(i+1,j-1));
    }
  }

  /*
  for ( int i = 0; i < S*S; i++ ) {
    col.push_back(vec4(rand_float(),rand_float(),rand_float(),1.0));
  }
  */

  Tick.Tock();
  if (DEBUG)
    fprintf( stderr, "Landgen took %lu usec, %f msec, %f sec to generate %d vertices.\n", 
	     Tick.Delta(), Tick.Delta()/1000.0, Tick.Delta()/1000000.0, S*S );

  return magnitude;
}
