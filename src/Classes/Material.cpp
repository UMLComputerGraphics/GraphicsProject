#include "Material.hpp"

/**
 * Default constructor for a material. Sets all values to their defaults:
 * ambient       (0.2, 0.2, 0.2)
 * diffuse       (1.0, 1.0, 1.0)
 * specular      (0.8, 0.8, 0.8)
 * shininess     0.0
 * transparency  1.0
 */
Material::Material() :
	ambient( vec3(0.2, 0.2, 0.2) ),
	specular( vec3(1.0, 1.0, 1.0) ),
	diffuse(vec3(0.8, 0.8, 0.8)),
	shininess(0.0),
	transparency(1.0),
	ior(1.0) {

	// Nothing
}

/**
 * Constructs a material with just a color vector.
 * 
 * @param ambient The ambient vector of the object.
 */
Material::Material(vec3 ambient) :
	ambient(ambient),
	specular(vec3(1.0, 1.0, 1.0)),
	diffuse(vec3(0.8, 0.8, 0.8)),
	shininess(0.0),
	transparency(1.0),
	ior(1.0) {
	
	// Nothing
} 

/** 
 * Constructs a material with basic color attributes. Sets transparency
 * to its default value (1.0).
 *
 * @param ambient The ambient vector of the object.
 * @param diffuse The diffuse vector of the object.
 * @param ambient The specular vector of the object.
 * @param ambient The shininess coefficient of the object.
 */
Material::Material(vec3 ambient, vec3 diffuse, vec3 specular, float shininess) :
	ambient(ambient),
	specular(specular),
	diffuse(diffuse),
	shininess(shininess),
	transparency(1.0),
	ior(1.0) {

	// Nothing
}

/** 
 * Constructs a material with basic color attributes. Sets transparency
 * to its default value (1.0).
 *
 * @param ambient The ambient vector of the object.
 * @param diffuse The diffuse vector of the object.
 * @param specular The specular vector of the object.
 * @param shininess The shininess coefficient of the object.
 * @param transparency The transparency coefficient of the object.
 */
Material::Material(vec3 ambient, vec3 diffuse, vec3 specular, 
				   float shininess, float transparency) :
	ambient(ambient),
	specular(specular),
	diffuse(diffuse),
	shininess(shininess),
	transparency(transparency),
	ior(1.0) {

	// Nothing
}

void Material::setAmbient(vec3 ambient) {
	this->ambient = ambient;
}

void Material::setDiffuse(vec3 diffuse) {
	this->diffuse = diffuse;
}

void Material::setSpecular(vec3 specular) {
	this->specular = specular;
}

void Material::setShininess(float shininess) {
	this->shininess = shininess;
}

void Material::setTransparency(float transparency) {
	this->transparency = transparency;
}

void Material::setRefract(float ior) {
	this->ior = ior;
}

const vec3 Material::getAmbient() const {
	return this->ambient;	
}

const vec3 Material::getDiffuse() const {
	return this->diffuse;	
}

const vec3 Material::getSpecular() const {
	return this->specular;	
}

float Material::getShininess() const {
	return this->shininess;	
}

float Material::getTransparency() const {
	return this->transparency;	
}

float Material::getRefract() const {
	return this->ior;
}

void Material::initMaterial(GLuint shaderProgram) {
	// TODO: Does the material initialization go here, or does it
	// go somewhere else. 
}
