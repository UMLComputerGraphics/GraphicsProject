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

// Morphing Information
uniform float morphPercentage;

void main() {

  vec4 position_tmp;

  // If vPositionMorph has valid data, calculate the morph.
  if (vPositionMorph.w != -1.0) {
    position_tmp = vPosition * (1.0 - morphPercentage) + vPositionMorph * morphPercentage;
  } else {
    position_tmp = vPosition;
  }

  // World coordinates of this vertex.
  fPosition = OTM * position_tmp;
  // Screen coordinates of this vertex.
  gl_Position = P * CTM * fPosition;

/*
  vPosition (Object coordinates) --> OTM (World coordinates) --> CTM (Camera coordinates) --> P (Screen coordinates.)
*/

  /*
    If we're using textures, send a dummy color.
    If we're using colors, send a dummy texture.
    If we're using colors and morphing is enabled, calculate that color.
  */

  if (fIsTextured) {
    outtexture = vTex;
    color = vec4( 0, 0, 0, 0 );
  } else {
    outtexture = vec2( 0, 0 );
    if (vColorMorph.w != -1.0) {
      color = vColor * (1.0 - morphPercentage) + vColorMorph * morphPercentage;
    } else {
      color = vColor;
    }
  }

}
