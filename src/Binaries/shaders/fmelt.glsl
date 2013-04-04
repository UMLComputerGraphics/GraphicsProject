// Test melt fragment shader

varying vec4 vColor;

void main()
{
	// Set the output color of our current pixel
	gl_FragColor = vColor;
}
