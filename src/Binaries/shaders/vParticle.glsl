// Particle System Vertex Shader
// Author: Chris Compton, christopher_compton@student.uml.edu
// 	   Nick Ver Voort, nicholas_vervoort@student.uml.edu

attribute vec4 vPosition;
uniform vec3 cameraPos;
varying vec3 gCamPos;

// variables for transformations needed? or calculated on CPU?


void main()
{
	gl_Position = vPosition;  // add transformation?
	gCamPos = cameraPos;
}