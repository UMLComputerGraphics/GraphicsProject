#version 120
#extension GL_EXT_gpu_shader4: enable
#extension GL_EXT_geometry_shader4: enable
// Particle System Geometry Shader
// Author: Chris Compton, christopher_compton@student.uml.edu
// 	   Nick Ver Voort, nicholas_vervoort@student.uml.edu
// Source: http://ogldev.atspace.co.uk/www/tutorial27/tutorial27.html

uniform mat4 gVP;     // Veiw/Projection matrix
uniform vec3 gCamPos; // Camera Position

varying vec4 gColor;
varying vec4 fColor;

varying float distance;
varying float distanceF;

vec2 TexCoord;        // Texture coordinates

void main () 
{

	// PASS THRU //
	distanceF = distance ;	


	//float particleSize = 0.25;
	float particleSize = 0.004;

	//Pass through color
	fColor = gColor;

	for ( int i = 0 ; i < gl_VerticesIn ; i++ )
	{

		vec4 Pos = gl_PositionIn[i];

		Pos.x -= (particleSize/2.0);
		Pos.y -= (particleSize/2.0);
		gl_Position = Pos ;
		EmitVertex();

		Pos.x += particleSize;
		gl_Position = Pos ;
		EmitVertex();

		Pos.x -= particleSize;
		Pos.y += particleSize;
		gl_Position = Pos ;
		EmitVertex();

		Pos.x += particleSize;
		gl_Position = Pos ;
		EmitVertex();

		EndPrimitive();

	}

}
