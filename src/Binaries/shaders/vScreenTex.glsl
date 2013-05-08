// Standard Attributes
attribute vec4 vPosition;
attribute vec4 vColor;
attribute vec3 vNormal;
attribute vec2 vTex;

// sent to the fshader
varying vec4 fPosition;
varying vec2 fTex;


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

  // World coordinates of this vertex.
  fPosition = OTM * vPosition;
  // Screen coordinates of this vertex.
  gl_Position = P * CTM * fPosition;

  fPosition = gl_Position;

  // As a reminder:
  // vPosition (Object Coordinates) -->
  // OTM (World/Scene Coordinates) -->
  // CTM (Camera-Centric Coordinates) -->
  // P (Screen Coordinates)

  // If we're using textures, send a dummy color.
  if (fIsTextured) {
    fTex = vTex;
  }


} // End main().
