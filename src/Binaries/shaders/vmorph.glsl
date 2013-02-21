// attributes
attribute vec4 vPosition;
attribute vec4 vColor;
attribute vec3 vNormal;

attribute vec4 vPositionMorph;
attribute vec4 vColorMorph;
attribute vec3 vNormalMorph;


// sent to the fshader
varying vec4 color;
varying vec4 fPosition;


// position/movement
uniform mat4 P;
uniform mat4 R;
uniform mat4 T;
uniform mat4 CTM; // CTM is either PRT or TRP, depending.
uniform mat4 OTM;


// IsTextured boolean.
uniform bool fIsTextured;


//Morph stuff
uniform float morphPercentage;


// fshader lighting vars
//varying vec3 cameraVector;
//varying vec3 fragmentNormal;
//varying vec3 lightVector[5];



void main() {

  vec4 position = vec4(0.0,0.0,0.0,1.0) ;

//  position   = (vec4(vPosition.xyz*(1.0-morphPercentage), 1.0) + (vec4(vPositionMorph.xyz*(morphPercentage), 1.0))) ;

  position.x   =  vPosition.x*(1.0-morphPercentage) + vPositionMorph.x*morphPercentage ;
  position.y   =  vPosition.y*(1.0-morphPercentage) + vPositionMorph.y*morphPercentage ;
  position.z   =  vPosition.z*(1.0-morphPercentage) + vPositionMorph.z*morphPercentage ;
  position.w   =  1.0 ;

  gl_Position = P * CTM * OTM * position;



  color.x = vColor.x*(1.0-morphPercentage) + vColorMorph.x*morphPercentage ;
  color.y = vColor.y*(1.0-morphPercentage) + vColorMorph.y*morphPercentage ;
  color.z = vColor.z*(1.0-morphPercentage) + vColorMorph.z*morphPercentage ;
  color.w = 1.0 ;


}
