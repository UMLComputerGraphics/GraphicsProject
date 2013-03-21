// Particle System Vertex Shader
// Author: Chris Compton, christopher_compton@student.uml.edu

attribute vec4 vPosition;

// variables for transformations needed? or calculated on CPU?


void main()
{
	gl_Position = vPosition;  // add transformation?
}