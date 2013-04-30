/**
 * @file ObjLoader.hpp
 * @date 2013-03-14
 * @authors Greg Giannone, Nick VerVoort, John Huston
 * @brief Headers for functions for reading in geometry from .OBJ files.
 */

#ifndef __OBJ_LOADER_HPP
#define __OBJ_LOADER_HPP

#include <vector>
#include <string>
#include <exception>
#include <sstream>
#include "vec.hpp"
#include "Object.hpp"

using std::string;
using std::vector;
using std::exception;

namespace ObjLoader {
  
  /**
   * Split a string by an arbitrary delimeter.
   * @param str The string to split.
   * @param delim The delimeter.
   * @return A std::vector<std::string> of all the tokens.
   */
  const vector< string > split( const string &str, const char delim );
  
  /**
   * loadObj loads all available objects from a .obj file into the provided scene.
   *
   * @param scene The scene to add objects into.
   * @param filename The filename to load objects from.
   * @param defaultObjName The name to use for any objects that are found.
   * @return A pointer to the most recently created object.
   */
  std::vector<Object*> loadObj( const Scene &scene, const char* filename, const char *objName =
                       NULL );
  /**
   * Split a string by an arbitrary delimeter.
   * @param str The string to split.
   * @param delim The delimeter.
   * @return A std::vector<std::string> of all the tokens.
   */
  Angel::vec4 parseVertex( const string &line );
  
  /**
   * Obtain a 2D TexUV coordinate from a std::string line.
   *
   * @param line The line to parse.
   * @return a vec2 containing the parsed texUV.
   */
  Angel::vec2 parseTextureUV( const string &line );
  
  /**
   * Obtain a 3D normal vector from a std::string line.
   *
   * @param line the line to parse.
   * @return a vec3 containing the parsed normal.
   */
  Angel::vec3 parseNormal( const string &line );
  
  /**
   * parses face elements into a vector of 3 vectors of 3 ints
   * from an .obj line
   * @param line The line to parse.
   * @return vector containing 3 vectors of 3 ints each.
   */
  vector< vector< int > > parseFaceElements( const string &line );
  
  /**
   * Helper function that splits an obj element listing
   * (x/y/z) into their proper components (vertices, UVs, normals.)
   *
   * @param triple The string representing the triple.
   * @param v_elements Where to store vertices found.
   * @param uv_elements Where to store UVs found.
   * @param n_elements Where to store normals found.
   */
  void parseElementTriple( const string &triple, vector< int > &v_elements,
                           vector< int > &uv_elements,
                           vector< int > &n_elements );
  
  /**
   * Legacy function until I make everything suck less:
   * Loads a single model from an OBJ and stores it into
   * a single Object instance.
   * FIXME: Make object loading suck less. (John Huston)
   *
   * @param object The object to add the geometry into.
   * @param filename the OBJ file to load.
   */
  void loadModelFromFile( Object *object, const char *filename );

  /**
   * Parses a color from a line in the .mtl file
   *
   * @param line The line containing the color data
   * @return A vec3 containing the color in rgb format
   */

  Angel::vec3 parseColor(const string &line);

  /**
   * Parses the index of refraction from the .mtl file
   *
   * @param line The line containing the index of refraction
   * @return The index of refraction
   */
   
  float parseRefract(const string &line);

  /**
   * Loads a material file into an object
   *
   * @param object The object to add the material into.
   * @param filename the MTL file to load.
   */
  void loadMaterialFromFile(Object* object, const char *filename);

}

#endif
