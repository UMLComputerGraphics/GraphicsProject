// Particle System Fragment Shader
// Author: Chris Compton, christopher_compton@student.uml.edu
// Source: http://ogldev.atspace.co.uk/www/tutorial27/tutorial27.html

uniform sampler2D gColorMap;

vec2 TexCoord; 
vec4 FragColor;

void main()
{
	FragColor = texture2D( gColorMap, TexCoord );

	if (FragColor.r == 0.0 && FragColor.g == 0.0 && FragColor.b == 0.0)
	{
	discard;
	}
}