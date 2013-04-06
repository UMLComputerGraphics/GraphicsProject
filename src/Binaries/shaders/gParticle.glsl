#version 120
#extension GL_EXT_gpu_shader4: enable
#extension GL_EXT_geometry_shader4: enable
// Particle System Geometry Shader
// Author: Chris Compton, christopher_compton@student.uml.edu
// Source: http://ogldev.atspace.co.uk/www/tutorial27/tutorial27.html

// layout (points) in;
// layout (triangle_strip) out;
// layout (max_vertices) = 4 out;

// varying vec4 gl_Position; 

uniform mat4 gVP;     // Veiw/Projection matrix
uniform vec3 gCamPos; // Camera Position

varying vec4 gColor;
varying vec4 fColor;

vec2 TexCoord;        // Texture coordinates

void main () 
{
		//Pass through color
		fColor = gColor;

		vec4 Pos = gl_PositionIn[0];

		// 'Grow' quad around a point
		gl_Position = Pos ;
		EmitVertex();

		Pos.z += 10.0 ;
		Pos.x += 10.0 ;
		gl_Position = Pos ;
		EmitVertex();

		Pos.z += 10.0 ;
		Pos.z += 10.0 ;
		Pos.y += 10.0;
		gl_Position = Pos ;
		EmitVertex();

		Pos.z += 10.0 ;
		Pos.x -= 10.0;
		gl_Position = Pos ;
		EmitVertex();
			
		EndPrimitive();
}
