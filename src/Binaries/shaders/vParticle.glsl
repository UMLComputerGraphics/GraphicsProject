// Particle System Vertex Shader
// Author: Chris Compton, christopher_compton@student.uml.edu
// 	   Nick Ver Voort, nicholas_vervoort@student.uml.edu

attribute vec4 vPosition;
attribute vec4 vColor;
uniform vec3 cameraPos;
varying vec3 gCamPos;
varying vec4 gColor;

// position/movement
uniform mat4 P;
uniform mat4 R;
uniform mat4 T;
uniform mat4 CTM; // CTM is either PRT or TRP, depending.
uniform mat4 OTM;

// variables for transformations needed? or calculated on CPU?


void main()
{
  	gl_Position = P * CTM * OTM * vPosition;
	//gl_Position = vPosition;  // add transformation?
	gCamPos = cameraPos;
	gColor = vColor;
}