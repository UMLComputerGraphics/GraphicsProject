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

vec2 TexCoord;        // Texture coordinates

void main () 
{
	// vec3 Pos = gl_in[0].gl_Position.xyz;
	int i;
	for (i = 0; i < gl_VerticesIn; i++) { 
		vec3 Pos = gl_PositionIn[i].xyz;

		vec3 toCamera = normalize( gCamPos - Pos );
		vec3 up = vec3( 0.0, 1.0, 0.0 );
		vec3 right = cross( toCamera, up );

		// 'Grow' quad around a point
		Pos -= ( right * 0.5 );
		gl_Position = gVP * vec4( Pos, 1.0 );
		TexCoord = vec2( 0.0, 0.0);
		EmitVertex();

		Pos.y += 1.0;
		gl_Position = gVP * vec4( Pos, 1.0 );
		TexCoord = vec2( 0.0, 1.0 );
		EmitVertex();

		Pos.y -= 1.0;
		Pos += right;
		gl_Position = gVP * vec4( Pos, 1.0 );
		TexCoord = vec2( 1.0, 0.0 );
		EmitVertex();

		Pos.y += 1.0;
		gl_Position = gVP * vec4( Pos, 1.0 );
		TexCoord = vec2( 1.0, 1.0 );
		EmitVertex();
		
		EndPrimitive();
	}
}
