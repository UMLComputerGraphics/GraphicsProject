varying  vec4 color;
varying  vec4 finalLight;


void main() 
{ 
    gl_FragColor = finalLight*color;
} 

