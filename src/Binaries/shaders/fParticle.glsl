// Particle System Fragment Shader
// Author: Chris Compton, christopher_compton@student.uml.edu
// Source: http://ogldev.atspace.co.uk/www/tutorial27/tutorial27.html

uniform sampler2D gColorMap;

vec2 TexCoord; 
varying vec4 fColor;
varying float distance;
void main()
{

	//gl_FragColor = vec4(0.90, 0.45, 0.1, 1.0)/(distance*distance*64.0);
	//gl_FragColor = vec4(0.90, 0.45, distance, 1.0)/(distance*distance*64.0);

	gl_FragColor = vec4(0.90, 0.45, 0.1, 1.0)/(distance*8.0);

	// also pleasant!
	//gl_FragColor = vec4(1.0, 0.5, 0.0, 1.0)/(distance*distance*distance*1000.0);
	//gl_FragColor.b = distance*2.0;

}