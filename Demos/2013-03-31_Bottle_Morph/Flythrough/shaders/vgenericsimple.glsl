///////////////////////////////////////////////////////////////////////////////
// GENERIC VSHADER
// Adadapted from this magnificent web site
// http://ogldev.atspace.co.uk/www/tutorial20/tutorial20.html

// attributes
attribute vec4 vPosition ;
attribute vec4 vNormal   ;

// sent to the fshader
varying vec4 color;
varying vec3 cameraVector;
varying vec3 fragmentNormal;
varying vec3 lightVector[8];

// position/movement
uniform mat4 P   ;
uniform mat4 R   ;
uniform mat4 T   ;
uniform mat4 CTM ; // CTM is either PRT or TRP, depending.

// Lighting 
uniform int numLights ;
uniform vec3 LightPositionArray[8] ;


void main() {

     int i;

     for ( i = 0 ; i < numLights && i < 8 ; i ++ ) {
	  lightVector[i] = LightPositionArray[i] - vPosition.xyz ;
     }

     //fragmentNormal = vNormal ;

     fragmentNormal = vec3(0.0,1.0,0.0) ;

     cameraVector = (R * vec4(0.0,0.0,1.0,1.0)).xyz;
     gl_Position = CTM * vPosition ;

}
