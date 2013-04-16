varying vec4 color;
varying vec2 outtexture;
uniform sampler2D sampler[];
uniform bool fIsTextured;

void main() { 
  if (fIsTextured) {
    gl_FragColor = texture2D( sampler[0], outtexture );
  } else {
    gl_FragColor = color;
  }
}