///////////////////////////////////////////////////////////////////////////////
// FSHADER

// received by the fshader
varying vec4 fColor;
varying vec3 fNormal;
varying vec2 fTex;
varying vec4 fPosition;
//varying vec3 cameraVector;


uniform bool fIsTextured;

uniform sampler2D sampler[]; // Array of sampler2D associated with this object.
uniform float     terrainMag;

float dirtUpper;
float grassUpper;
float grassLower;
float rockUpper;
float rockLower;
float sandUpper;
float sandLower;
float snowLower;

const float MAX_DIST = 21.0 ;
const float MAX_DIST_SQUARED = MAX_DIST * MAX_DIST;

vec4 textureGradient( sampler2D a, sampler2D b, float upper, float lower )
{
    float diff = upper - lower ;
    return ((texture2D( a, fTex) * (fPosition.y - lower)) +
            (texture2D( b, fTex) * (upper - fPosition.y))) / diff ;
}

void setTextureLimits( float mag ){

    snowLower  =  0.2     * mag;
    rockUpper  =  0.1675  * mag;
    rockLower  =  0.15    * mag;
    grassUpper =  0.08    * mag;
    grassLower =  0.0375  * mag;
    sandUpper  =  0.00625 * mag;
    sandLower  = -0.0125  * mag;
    dirtUpper  = -0.0375  * mag;
}

void main() 
{

  vec4 fragColor ;

  if (fIsTextured) {

     setTextureLimits( terrainMag );    

    if ( fPosition.y > snowLower )
        fragColor = texture2D( sampler[4], fTex ); 
    // Snow->Rock
    else if ((fPosition.y > rockUpper) && (fPosition.y < snowLower) )
	fragColor = textureGradient( sampler[4], sampler[3], snowLower, rockUpper);
    // Rock
    else if ((fPosition.y > rockLower) && (fPosition.y <= rockUpper)) 
        fragColor = texture2D( sampler[3], fTex );
    // Rock->Grass
    else if ((fPosition.y > grassUpper) && (fPosition.y < rockLower))
        fragColor = textureGradient( sampler[3], sampler[2], rockLower, grassUpper );
    // Grass
    else if (( fPosition.y > grassLower ) && (fPosition.y <= grassUpper ))
        fragColor = texture2D( sampler[2], fTex );  
    // Grass->Sand
    else if ((fPosition.y > sandUpper) && (fPosition.y < grassLower))
        fragColor = textureGradient( sampler[2], sampler[1], grassLower, sandUpper );
    // Sand
    else if ((fPosition.y > sandLower ) && (fPosition.y <= sandUpper ))
        fragColor = texture2D( sampler[1], fTex );  
    // Sand->Dirt
    else if ((fPosition.y > dirtUpper) && (fPosition.y < sandLower))
        fragColor = textureGradient( sampler[1], sampler[0], sandLower, dirtUpper );
    // Dirt
    else if ( fPosition.y <= dirtUpper )
        fragColor = texture2D( sampler[0], fTex );  
    }

    else {
      fragColor = fColor ;
    }

gl_FragColor = fragColor;

}
