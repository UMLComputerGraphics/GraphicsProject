// attributes
attribute vec4 vPosition;
attribute vec4 vColor;
attribute vec2 vTex;

// sent to the fshader
varying vec4 color;
varying vec4 fPosition;
varying vec2 outtexture;

// position/movement
uniform mat4 P;   // Camera Perspective: Vertices are transformed to Screen coordinates.
uniform mat4 R;   // Camera Rotations only.
uniform mat4 T;   // Camera Translations only.
uniform mat4 CTM; // Camera transformations: Vertices are transformed to Camera coordinates with world scaling.
uniform mat4 OTM; // Object Transformations: Objects are adjusted to world coordinates.

// IsTextured boolean.
uniform bool fIsTextured;

void main() {

  // World coordinates of this vertex.
  fPosition = OTM * vPosition;
  // Screen coordinates of this vertex.
  gl_Position = P * CTM * fPosition;

/*
  vPosition (Object coordinates) --> OTM (World coordinates) --> CTM (Camera coordinates) --> P (Screen coordinates.)
*/

  if (fIsTextured) {
    outtexture = vTex;
    color = vec4( 0, 0, 0, 0 );
  } else {
    outtexture = vec2( 0, 0 );
    color = vColor;
  }

}
