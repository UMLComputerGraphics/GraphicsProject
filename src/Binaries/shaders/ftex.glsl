varying vec4 color;
varying vec2 outtexture;

uniform sampler2D gSampler0;

void main() 
{ 
    gl_FragColor = texture2D( gSampler0, outtexture );
} 

