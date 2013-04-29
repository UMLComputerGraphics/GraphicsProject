// Particle System Vertex Shader
// Author: Chris Compton, christopher_compton@student.uml.edu
// 	   Nick Ver Voort, nicholas_vervoort@student.uml.edu

// Standard Attributes
attribute vec4 vPosition;
attribute vec4 vColor;
attribute vec3 vNormal;
attribute vec2 vTex;

// position/movement
uniform mat4 P;   // Camera Perspective: Vertices are transformed to Screen coordinates.
uniform mat4 R;   // Camera Rotations only.
uniform mat4 T;   // Camera Translations only.
uniform mat4 CTM; // Camera transformations: Vertices are transformed to Camera coordinates with world scaling.
uniform mat4 OTM; // Object Transformations: Objects are adjusted to world coordinates.

// IsTextured boolean.
uniform bool fIsTextured;

// Sent out to the gShader
varying vec3 gCamPos;
varying vec4 gColor ;

varying float distance ;

void main()
{

	vec4 my_Position = OTM * vPosition;
  	gl_Position = P * CTM * my_Position;

	gl_FrontColor = vColor ;

/*
	distance = sqrt((my_Position.x * my_Position.x)
               	      + (my_Position.z * my_Position.z)
 	              + (my_Position.y * my_Position.y));
*//*
	distance = sqrt( (vPosition.x * vPosition.x) 
		       + (vPosition.z * vPosition.z) 
		       + (vPosition.y * vPosition.y));
*/
	//distance = sqrt((vPosition.x * vPosition.x) + (vPosition.z * vPosition.z) + (vPosition.y * vPosition.y));
        //distance = 0.01;
	//distance = abs(vPosition.y) + abs(vPosition.x) + abs(vPosition.z) ;

}
