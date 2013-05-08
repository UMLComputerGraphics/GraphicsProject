#version 120

// Sent from the Vertex Shader
varying vec4 fColor;
varying vec4 fPosition;
varying vec4 vPosition;
varying vec2 fTex;
varying vec3 fNormal;

// position/movement
uniform mat4 P;
uniform mat4 R;
uniform mat4 T;
uniform mat4 CTM;
uniform mat4 OTM;

uniform float ftime;

uniform sampler2D sampler[];
uniform bool fIsTextured;

void main() {  

     vec2 me;

     me.x = 0.5+fPosition.x/2.0;

     me.y = 0.5+fPosition.y/-2.0; //+ 0.5;


     gl_FragColor = texture2D( sampler[0], me );

     //gl_FragCoord.xy

    
}
