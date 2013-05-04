#ifndef _MATERIAL_H
#define _MATERIAL_H

#include "vec.hpp"

using Angel::vec3;

/**
 * This class contains the material information of an object. The elements
 * in this class are parsed from the .mtl file. The values stored are the
 * Ambient, specular, diffuse vectors, and the shininess and transparency
 * coefficients. Planned later is to hold the bump mappings.
 *
 * @author Michael Gorlin
 * @since 2013-04-16
 */
class Material {
public:
	Material();
	Material(vec3 ambient);
	Material(vec3 ambient, vec3 diffuse, vec3 specular, float shininess);
	Material(vec3 ambient, vec3 diffuse, vec3 specular, float shininess, float transparency);

	void setAmbient(vec3 ambient);
	void setSpecular(vec3 specular);
	void setDiffuse(vec3 diffuse);
	void setShininess(float shininess);
	void setTransparency(float transparency);
	void setRefract(float ior);
	
	const vec3 getAmbient() const;
	const vec3 getDiffuse() const;
	const vec3 getSpecular() const;

	float getShininess() const;
	float getTransparency() const;
	float getRefract() const;

	/**
	 * This function initializes the uniforms for the material in the shader. 
	 * Uniform the uniform names are:
	 *    vec3  mat_ambient
	 *    vec3  mat_diffuse
	 *    vec3  mat_specular
	 *    float mat_shininess
	 *    float mat_transparency
	 *
	 * @param shaderProgram The shader program that is going to be used to
	 * render the object.
	 */
	void initMaterial(GLuint shaderProgram);

private:
	vec3 ambient;
	vec3 specular;
	vec3 diffuse;
	
	float shininess;
	float transparency;
	float ior;

};

#endif
