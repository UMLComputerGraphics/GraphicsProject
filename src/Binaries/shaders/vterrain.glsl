// attributes
attribute vec4 vPosition;
attribute vec4 vColor;
attribute vec2 vTex;

// sent to the fshader
varying vec4 color;
varying vec4 fPosition;
varying vec2 outtexture;

// position/movement
uniform mat4 P;
uniform mat4 R;
uniform mat4 T;
uniform mat4 CTM; // CTM is either PRT or TRP, depending.
uniform mat4 OTM;

// IsTextured boolean.
uniform bool fIsTextured;

// Lighting, dammit.
uniform int numLights ;
uniform vec3 LightPositionArray[5] ;

// fshader lighting vars
varying vec3 cameraVector;
varying vec3 fragmentNormal;
varying vec3 lightVector[5];

void main() {

  //fsunHeight = sunHeight ;
  gl_Position = P * CTM * OTM * vPosition;

  cameraVector = (R * T * vec4(0.0,0.0,-1.0,1.0)).xyz ;
  fragmentNormal = vec3(0.0,1.0,0.0) ;


  color = vColor;
  if (fIsTextured) outtexture = vTex;
  //else outtexture = vec2( -1.0, -1.0 );
  fPosition = OTM * vPosition;
  //fPosition = R*T*OTM*vPosition;

/*
  int i;
  for ( i = 0 ; i < numLights && i < 5 ; i++ ) {
    lightVector[i] = LightPositionArray[i] - fPosition.xyz ; //(R*T*OTM*vPosition).xyz ;
  }
*/


}
