/**
 * @file ObjLoader.cpp
 * @date 2013-03-14
 * @authors Greg Giannone, Nick VerVoort, John Huston
 * @brief Implementation for reading in geometry from .OBJ files.
 */

#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <stdexcept>
#include <cstdio>
#include "vec.hpp"
#include "ObjLoader.hpp"
#include "Util.hpp"

using std::vector;
using std::string;
using Angel::vec4;
using Angel::vec3;
using Angel::vec2;
using std::istringstream;

namespace ObjLoader {
  /**
   * Split a string by an arbitrary delimiter.
   * @param str The string to split.
   * @param delim The delimiter.
   * @return A std::vector<std::string> of all the tokens.
   */
  const vector< string > split( const string &str, const char delim ) {
    std::vector< string > elements;
    std::string tmp = str;
    
    // loop through and break string up by the delimiter
    while ( tmp.find_first_of( delim ) != string::npos ) {
      // position of delimiter in current string
      int pos = tmp.find_first_of( delim );
      elements.push_back( tmp.substr( 0, pos ) );
      tmp = tmp.substr( pos + 1 );
    }
    
    // str will have the final element remaining in it
    elements.push_back( tmp );
    return elements;
  }
  
  /**
   * Obtain a vertex from a std::string line.
   *
   * @param line The line to parse.
   * @return a vec4 containing the parsed vertex.
   */
  vec4 parseVertex( const string& line ) {
    
    vec4 v;
    
    // read in vertex coords
    istringstream s( line.substr( 2 ) );
    s >> v.x;
    s >> v.y;
    s >> v.z;
    v.w = 1.0;
    
    return v;
  }
  
  /**
   * Obtain a 2D TexUV coordinate from a std::string line.
   *
   * @param line The line to parse.
   * @return a vec2 containing the parsed texUV.
   */
  vec2 parseTextureUV( const string &line ) {
    vec2 uv;
    
    // read in uv coords
    istringstream s( line.substr( 3 ) );
    s >> uv.x;
    s >> uv.y;
    
    //Correct "out of range" UV coordinates
    while( uv.x > 1 ) {
      uv.x -= 1;
    }
    while( uv.x < 0 ) {
      uv.x += 1;
    }
    while( uv.y > 1 ) {
      uv.y -= 1;
    }
    while( uv.y < 0 ) {
      uv.y += 1;
    }

    return uv;
  }
  
  /**
   * Obtain a 3D normal vector from a std::string line.
   *
   * @param line the line to parse.
   * @return a vec3 containing the parsed normal.
   */
  vec3 parseNormal( const string& line ) {
    vec3 vn;
    
    istringstream s( line.substr( 3 ) );
    s >> vn.x;
    s >> vn.y;
    s >> vn.z;
    
    return vn;
  }
  
  /**
   * parses face elements into a vector of 3 vectors of 3 ints
   * from an .obj line
   * @param line The line to parse.
   * @return vector containing 3 vectors of 3 ints each.
   */
  vector< vector< int > > parseFaceElements( const string& line ) {
    vector< vector< int > > elements;
    
    // vectors for vertex, texture, and normal elements
    vector< int > v_elements;
    vector< int > uv_elements;
    vector< int > n_elements;
    
    // vector for elements returned from split
    vector< string > raw_elements;
    
    // get the 3 triples of elements from the line
    istringstream s( line.substr( 2 ) );
    string a, b, c;
    s >> a;
    s >> b;
    s >> c;
    
    // parse the 3 triples into 3 vectors
    parseElementTriple( a, v_elements, uv_elements, n_elements );
    parseElementTriple( b, v_elements, uv_elements, n_elements );
    parseElementTriple( c, v_elements, uv_elements, n_elements );
    
    // add the vectors of different elements to single vector
    elements.push_back( v_elements );
    elements.push_back( uv_elements );
    elements.push_back( n_elements );
    
    return elements;
  }
  
  /**
   * Helper function that splits an obj element listing
   * (x/y/z) into their proper components (vertices, UVs, _normals.)
   *
   * @param triple The string representing the triple.
   * @param v_elements Where to store vertices found.
   * @param uv_elements Where to store UVs found.
   * @param n_elements Where to store _normals found.
   */
// parses a line of elements and adds the elements to the vectors passed in
// if a line has less than 3 kinds of elements, the vectors of the types
// of elements that are not included in the file will not be updated
  void parseElementTriple( const string &triple, vector< int > &v_elements,
      vector< int > &uv_elements,
      vector< int > &n_elements ) {
    vector< string > raw_elements = split( triple, '/' );

    for(int i=raw_elements.size()-1;i>=0;i--)
      if (i!=1 || raw_elements[i].compare( "" ) != 0)
        (i>=2?n_elements:(i==1?uv_elements:v_elements))
          .push_back(atoi(raw_elements[i].c_str()));
  }

  /**
   * Parses a color from a line in the .mtl file
   *
   * @param line The line containing the color data
   * @return A vec3 containing the color in rgb format
   */

  vec3 parseColor(const string &line)
  {
    vec3 color;
    
    istringstream s( line.substr( 3 ) );
    s >> color.x;
    s >> color.y;
    s >> color.z;
    
    return color;
  }

  /**
   * Parses the index of refraction from the .mtl file
   *
   * @param line The line containing the index of refraction
   * @return The index of refraction
   */
   
  float parseRefract(const string &line)
  {
    float ior;

    istringstream s(line.substr(3));
    s >> ior;

    return ior;
  }
  
  /**
   * loadObj loads all available objects from a .obj file into the provided scene.
   *
   * @param scene The scene to add objects into.
   * @param filename The filename to load objects from.
   * @param defaultObjName The name to use for any objects that are found.
   * @return A pointer to the most recently created object.
   */
  std::vector<Object *> loadObj(Scene scene, const char* filename,
                   const char *defaultObjName ) {
  /**
   * Parses the index of refraction from the .mtl file
   *
   * @param line The line containing the index of refraction
   * @return The index of refraction
   */
   
  float parseRefract(const string &line);

    // file input stream
    std::ifstream in( filename, std::ios::in );
    std::vector<Object*> results;
    
    if ( !in ) {
      throw std::runtime_error( "Could not open file." );
    }
    
    bool firstObject = true;
    
    vector< vec4 > raw_vertices;
    vector< vec4 > vertices;
    vector< vec3 > raw_normals;
    vector< vec3 > normals;
    vector< vec2 > raw_textureUVs;
    vector< vec2 > textureUVs;
    
    string line;
    string objName;
    
    // parse the .obj file for its data
    while ( getline( in, line ) ) {
      // lines beginning with 'v ' have vertices
      if ( line.substr( 0, 2 ) == "v " ) {
        raw_vertices.push_back( parseVertex( line ) );
      }

      // 'o ' or 'g ' signify a new object is starting
      else if ( line.substr( 0, 1 ) == "o" || line.substr( 0, 1 ) == "g" ) {
        
        if ( line.substr( 1 ).compare( "" ) == 0 ) {
          if ( defaultObjName ) objName = defaultObjName;
          else objName = std::string( filename ) + " Unnamed Object";
        } else {
          objName = line.substr( 2 );
        }
        
        // skip writing data to an object if we don't have data yet
        if ( firstObject ) {
          firstObject = false;
          continue;
        }
        
        // Add this object up-in-here.
	gprint( PRINT_DEBUG, "Creating Object: %s\n", objName.c_str() );
        Object *thisObj = scene.addObject( objName );
	results.push_back( thisObj );
        thisObj->_vertices = vertices;
        thisObj->_texUVs = textureUVs;
        thisObj->_normals = normals;
        
        vertices.clear();
        textureUVs.clear();
        normals.clear();
      }

      // lines beginning with 'f ' have elements
      else if ( line.substr( 0, 2 ) == "f " ) {
        // parse the elements from the .obj line
        vector< vector< int > > faceElements = parseFaceElements( line );
        
        // put the vertices described by the element vector into the main vertex vector
        for ( uint i = 0; i < faceElements.at( 0 ).size(); i++ ) {
          vertices.push_back(
              raw_vertices.at( faceElements.at( 0 ).at( i ) - 1 ) );
        }
        
        // do the same for texture elements
        for ( uint i = 0; i < faceElements.at( 1 ).size(); i++ ) {
          textureUVs.push_back(
              raw_textureUVs.at( faceElements.at( 1 ).at( i ) - 1 ) );
        }
        
        // and finally for normal elements
        for ( uint i = 0; i < faceElements.at( 2 ).size(); i++ ) {
          normals.push_back(
              raw_normals.at( faceElements.at( 2 ).at( i ) - 1 ) );
        }
      }

      // lines beginning with 'vt ' will have UV coords
      else if ( line.substr( 0, 3 ) == "vt " ) {
        raw_textureUVs.push_back( parseTextureUV( line ) );
      }

      // lines beginning with 'vn ' will have _normals
      else if ( line.substr( 0, 3 ) == "vn " ) {
        raw_normals.push_back( parseNormal( line ) );
      }

      else if ( line[0] == '#' ) { /* ignore comments */
      } else { /* blank/junk */
      }
    }
    
    // Add this object up-in-here.
    gprint( PRINT_DEBUG, "Creating Object: %s\n", objName.c_str() );
    if ( objName.length() == 0 ) objName = defaultObjName;
    Object *thisObj = scene.addObject( objName );
    results.push_back( thisObj );
    thisObj->_vertices = vertices;
    thisObj->_texUVs = textureUVs;
    thisObj->_normals = normals;
    
    return results;
  }
  
  /** RAND_FLOAT returns a random float from (0,1). */
#define RAND_FLOAT (rand() / (float) RAND_MAX)
  
  /**
   * Legacy function until I make everything suck less:
   * Loads a single model from an OBJ and stores it into
   * a single Object instance.
   * FIXME: Make object loading suck less. (John Huston)
   *
   * @param object The object to add the geometry into.
   * @param filename the OBJ file to load.
   */
  void loadModelFromFile( Object *object, const char *filename ) {
    
    std::string relativePath = Util::getRelativePath(filename);

    // file input stream
    std::ifstream in( relativePath.c_str(), std::ios::in );
    
    if ( !in ) {
      throw std::runtime_error( "Could not open file." );
    }
    
    vector< vec4 > raw_vertices;
    vector< vec3 > raw_normals;
    vector< vec2 > raw_textureUVs;
    
    string line;
    
    short unsigned numObjects = 0;
    
    // parse the .obj file for its data
    while ( getline( in, line ) ) {
      // lines beginning with 'v ' have vertices
      if ( line.substr( 0, 2 ) == "v " ) {
        raw_vertices.push_back( parseVertex( line ) );
      }

      // 'o ' or 'g ' signify a new object is starting
      else if ( line.substr( 0, 1 ) == "o" || line.substr( 0, 1 ) == "g" ) {
        if ( ++numObjects > 1 ) break;
      }

      // lines beginning with 'f ' have elements
      else if ( line.substr( 0, 2 ) == "f " ) {
        // parse the elements from the .obj line
        vector< vector< int > > faceElements = parseFaceElements( line );
        
        // put the vertices described by the element vector into the main vertex vector
        for ( uint i = 0; i < faceElements.at( 0 ).size(); i++ ) {
          object->_vertices.push_back(
              raw_vertices.at( faceElements.at( 0 ).at( i ) - 1 ) );
        }
        
        // do the same for texture elements
        for ( uint i = 0; i < faceElements.at( 1 ).size(); i++ ) {
          object->_texUVs.push_back(
              raw_textureUVs.at( faceElements.at( 1 ).at( i ) - 1 ) );
        }
        
        // and finally for normal elements
        for ( uint i = 0; i < faceElements.at( 2 ).size(); i++ ) {
          object->_normals.push_back(
              raw_normals.at( faceElements.at( 2 ).at( i ) - 1 ) );
        }
      }

      // lines beginning with 'vt ' will have UV coords
      else if ( line.substr( 0, 3 ) == "vt " ) {
        raw_textureUVs.push_back( parseTextureUV( line ) );
      }

      // lines beginning with 'vn ' will have _normals
      else if ( line.substr( 0, 3 ) == "vn " ) {
        raw_normals.push_back( parseNormal( line ) );
      }

      else if ( line[0] == '#' ) { /* ignore comments */
      }
    }
    
    if ( object->_texUVs.size() < object->_vertices.size() )
      for ( size_t i = 0; i < object->_vertices.size(); ++i ) {
        object->_colors.push_back(vec4( RAND_FLOAT, RAND_FLOAT, RAND_FLOAT, 1.0 ) );
      }

    for ( size_t i = 0; i < object->_texUVs.size(); ++i ) {
      Angel::vec2 &txy = object->_texUVs[i];

      if ((txy.x < 0.0) || (txy.x > 1.0) || (txy.y < 0.0) || (txy.y > 1.0))
	gprint( PRINT_WARNING, "WARNING: Texture UV is out of bounds. (%f,%f)\n", txy.x, txy.y );

      if (txy.x < 0.0) txy.x = 0.0;
      if (txy.x > 1.0) txy.x = 1.0;
      if (txy.y < 0.0) txy.y = 0.0;
      if (txy.y > 1.0) txy.y = 1.0;
    }

  } // End Object Loader

  /**
   * Loads a material file into an object
   *
   * @param object The object to add the material into.
   * @param filename the MTL file to load.
   */
  void loadMaterialFromFile(Object* object, const char *filename) {

    std::string relativePath = Util::getRelativePath(filename);

    // file input stream
    std::ifstream in( relativePath.c_str(), std::ios::in );

    string line;

    Material *material = new Material();
    
    if ( !in ) {
      throw std::runtime_error( "Could not open file." );
    }

    // parse the .mtl file for its data
    while ( getline( in, line ) ) {
	 // line starting with 'Kd ' is diffuse color
      if ( line.substr( 0, 3 ) == "Kd " ) {
	   material->setDiffuse(parseColor(line));
      }
	 // ambient color
	 else if (line.substr(0, 3) == "Ka ") {
	   material->setAmbient(parseColor(line));
	 }
	 // specular color
	 else if (line.substr(0, 3) == "Ks ") {
	   material->setSpecular(parseColor(line));
	 }
	 // index of refraction
	 else if (line.substr(0, 3) == "Ni ") {
	   material->setRefract(parseRefract(line));
	 }
    }

    object->addMaterial(material);    

  }

} // End Namespace
