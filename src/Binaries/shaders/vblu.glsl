// Standard Attributes
attribute vec4 vPosition;
attribute vec4 vColor;
attribute vec3 vNormal;
attribute vec2 vTex;

// Morphing Attributes
attribute vec4 vPositionMorph;
attribute vec4 vColorMorph;
attribute vec3 vNormalMorph;

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

// Morphing Information
uniform float morphPercentage;

void main() {

  fPosition = OTM * vPosition;
  gl_Position = P * CTM * OTM * vPosition;
  fColor = vec4( 0, 0, 1, 1 );
  fTex = vec2( 0, 0 );
  fNormal = vNormal;

}
