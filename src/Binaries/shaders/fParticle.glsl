// Particle System Fragment Shader
// Author: Chris Compton, christopher_compton@student.uml.edu
// Source: http://ogldev.atspace.co.uk/www/tutorial27/tutorial27.html

uniform sampler2D gColorMap;

vec2 TexCoord; 
varying vec4 fColor;

void main()
{
	gl_FragColor = fColor;
}