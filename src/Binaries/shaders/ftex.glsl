varying vec4 color;
varying vec2 outtexture;
uniform sampler2D sampler;

void main() 
{ 
    gl_FragColor = texture2D( sampler, outtexture );
} 

