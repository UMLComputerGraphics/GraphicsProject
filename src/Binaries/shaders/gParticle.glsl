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

vec2 TexCoord;        // Texture coordinates

void main () 
{
	float particleSize = 0.25;

	//Pass through color
	fColor = gColor;

	vec4 Pos = gl_PositionIn[0];


	// 'Grow' quad around a point
	gl_Position = Pos ;

	// uh actually if you want the quad /AROUND/ the point, we need to first:
	Pos.x -= particleSize;
	EmitVertex();

	Pos.y += particleSize;
	gl_Position = Pos ;
	EmitVertex();

	Pos.y -= particleSize;
	Pos.x += particleSize;
	gl_Position = Pos ;
	EmitVertex();

	Pos.y += particleSize;
	gl_Position = Pos ;
	EmitVertex();

			
	EndPrimitive();
}
