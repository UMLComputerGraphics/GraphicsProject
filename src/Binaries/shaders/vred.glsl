// Standard Attributes
attribute vec4 vPosition;
attribute vec4 vColor;
attribute vec3 vNormal;
attribute vec2 vTex;

// sent to the fshader
varying vec4 fColor;
varying vec4 fPosition;
varying vec2 fTex;
varying vec3 fNormal;

// position/movement
uniform mat4 P;   // Camera Perspective: Vertices are transformed to Screen coordinates.
uniform mat4 R;   // Camera Rotations only.
uniform mat4 T;   // Camera Translations only.
uniform mat4 CTM; // Camera transformations: Vertices are transformed to Camera coordinates with world scaling.
uniform mat4 OTM; // Object Transformations: Objects are adjusted to world coordinates.

// IsTextured boolean.
uniform bool fIsTextured;

void main() {

  fPosition = OTM * vPosition;
  gl_Position = P * CTM * OTM * vPosition;
  fColor = vec4( 1, 0, 0, 1 );
  fTex = vec2( 0, 0 );
  fNormal = vNormal;

}
