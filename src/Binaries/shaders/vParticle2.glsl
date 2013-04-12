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

varying float distance ;
void main()
{

	vec4 my_Position = OTM * vPosition;
  	gl_Position = P * CTM * my_Position;


	//gCamPos = cameraPos;
	//gColor = vColor;
	//gColor = vec4(1.0, 1.0, 1.0, 1.0);

/*
	distance = sqrt((my_Position.x * my_Position.x)
               	      + (my_Position.z * my_Position.z)
 	              + (my_Position.y * my_Position.y));
*/

	distance = sqrt( (vPosition.x * vPosition.x) 
		       + (vPosition.z * vPosition.z) 
		       + (vPosition.y * vPosition.y));

	//distance = sqrt((vPosition.x * vPosition.x) + (vPosition.z * vPosition.z) + (vPosition.y * vPosition.y));
        // distance = 0.01;

	//distance = abs(vPosition.y) + abs(vPosition.x) + abs(vPosition.z) ;

}