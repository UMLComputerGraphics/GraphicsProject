varying vec4 fColor;
varying vec4 fPosition;
varying vec2 fTex;
varying vec3 fNormal;

uniform sampler2D sampler[];
uniform bool fIsTextured;

void main() { 

  if (fIsTextured) {
    gl_FragColor = texture2D( sampler[0], fTex );
  } else {
    gl_FragColor = fColor;
  }

}