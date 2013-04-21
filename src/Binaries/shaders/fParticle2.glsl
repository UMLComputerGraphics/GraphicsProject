// Particle System Fragment Shader
// Source: http://ogldev.atspace.co.uk/www/tutorial27/tutorial27.html

//uniform sampler2D gColorMap;

//vec2 TexCoord; 
//varying vec4 fColor;
//varying float distanceF;

void main()
{
	//float distance;

	//distance = distanceF;

	//gl_FragColor = vec4(0.90, 0.45, 0.1, 1.0)/(distance*distance*64.0);
	//gl_FragColor = vec4(0.90, 0.45, distance, 1.0)/(distance*distance*64.0);

	//gl_FragColor = vec4(0.90, 0.45, distance*distance*16.0, 1.0)/(distance*8.0);

	//gl_FragColor = vec4(0.90, 0.45, 0.05, 1.0)/(distance*8.0);

	//gl_FragColor.b = 0.2;

	// also pleasant!
	//gl_FragColor = vec4(1.0, 0.5, 0.0, 1.0)/(distance*distance*distance*1000.0);
	//gl_FragColor.b = distance*2.0;

	//gl_FragColor = fColor;
	gl_FragColor = gl_Color;
	//gl_FragColor = vec4(0.90, 0.50, 0.10, 1.0);

}
