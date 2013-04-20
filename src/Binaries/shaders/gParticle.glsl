#version 120
//#extension GL_EXT_gpu_shader4: enable
#extension GL_EXT_geometry_shader4: enable
// Particle System Geometry Shader
// Author: Chris Compton, christopher_compton@student.uml.edu
// 	   Nick Ver Voort, nicholas_vervoort@student.uml.edu
// Source: http://ogldev.atspace.co.uk/www/tutorial27/tutorial27.html

uniform mat4 gVP;     // View/Projection matrix
uniform vec3 gCamPos; // Camera Position

varying vec4 gColor;
//varying vec4 fColor;

varying float distance;
//varying float distanceF;

vec2 TexCoord;        // Texture coordinates

void main () 
{

	// PASS THRU //
	//distanceF = distance ;	

	//float particleSize = 0.2;
	float particleSize = 0.01;

	//Pass through color
	//fColor = gColor;

	gl_FrontColor = gl_FrontColorIn[0];

	for ( int i = 0 ; i < gl_VerticesIn ; i++ )
	{

//		fColor = gl_FrontColorIn[0];
//		gl_FrontColor = vec4(1.0,1.0,1.0,1.0) ;
//		gl_FrontColor = gColor ;

		vec4 Pos = gl_PositionIn[i];

		Pos.x -= (particleSize/2.0);
		Pos.y -= (particleSize/2.0);
		gl_Position = Pos ;
		EmitVertex();

//		gl_FrontColor = vec4(1.0,0.0,1.0,1.0) ;

		Pos.x += particleSize;
		gl_Position = Pos ;
		EmitVertex();

//		gl_FrontColor = vec4(0.0,1.0,1.0,1.0) ;

		Pos.x -= particleSize;
		Pos.y += particleSize;
		gl_Position = Pos ;
		EmitVertex();

//		gl_FrontColor = vec4(1.0,0.0,0.0,1.0) ;

		Pos.x += particleSize;
		gl_Position = Pos ;
		EmitVertex();

		EndPrimitive();

	}

}
