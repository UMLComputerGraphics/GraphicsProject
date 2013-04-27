// A simple shader that draws blue vertices. Used for testing multi-shader code.
// include "include/vertexBase.inc"

// attributes
attribute vec4 vPosition;
attribute vec4 vColor;
attribute vec2 vTex;

// sent to the fshader
varying vec4 fColor;
varying vec2 fTex;

// position/movement
uniform mat4 P;
uniform mat4 R;
uniform mat4 T;
uniform mat4 CTM; // CTM is either PRT or TRP, depending.
uniform mat4 OTM;

// IsTextured boolean.
uniform bool fIsTextured;

void main() {

  gl_Position = P * CTM * OTM * vPosition;
  fColor = vec4( 0, 0, 1, 1 );
  fTex = vec2( 0, 0 );

}
