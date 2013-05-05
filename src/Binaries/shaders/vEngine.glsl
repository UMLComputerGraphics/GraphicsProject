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

//general light stuff
uniform bool letMeSeeThatPhong; //pha phong phong phong
const int maxNumberOfLights = 5;
uniform int uNumberOfLights;
uniform vec4 LightAmbient, 
	     uLightPositions[maxNumberOfLights], 
	     uLightDiffuse[maxNumberOfLights], 
	     uLightSpecular[maxNumberOfLights];

uniform float uLightIntensity[maxNumberOfLights];

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

  // If morphing is enabled, morphPercentage will be non-zero.
  // It is therefore easier to just always compute the following.
  fPosition = vPosition * (1.0 - morphPercentage) + vPositionMorph * morphPercentage;
  fNormal = vNormal * (1.0 - morphPercentage) + vNormalMorph * morphPercentage;

  // World coordinates of this vertex.
  fPosition = OTM * fPosition;
  // Screen coordinates of this vertex.
  gl_Position = P * CTM * fPosition;

  // As a reminder:
  // vPosition (Object Coordinates) -->
  // OTM (World/Scene Coordinates) -->
  // CTM (Camera-Centric Coordinates) -->
  // P (Screen Coordinates)

  // If we're using textures, send a dummy color.
  if (fIsTextured) {
    fTex = vTex;
    fColor = vec4( 0, 0, 0, 0 );
  }

  // If we're using colors, send a dummy texture.
  else {
    fTex = vec2( 0, 0 );
    // Again, it's easier to just always compute this.
    fColor = vColor * (1.0 - morphPercentage) + vColorMorph * morphPercentage;
  }

  // Pass along the normal.
  //fNormal = vNormal;

} // End main().
