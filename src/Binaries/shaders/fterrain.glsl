///////////////////////////////////////////////////////////////////////////////
// FSHADER

// received by the fshader
varying vec4 color;
varying vec3 cameraVector;
varying vec3 fragmentNormal;
varying vec3 lightVector[5];
varying vec4 fsunHeight;

//lights
uniform int numLights ;
uniform vec3 lightColor[5];
uniform vec3 LightPositionArray[5] ;
uniform vec3 LightDirectionArray[5];

varying vec2 outtexture;
varying vec4 fPosition;

uniform bool fIsTextured;

uniform sampler2D sampler; // Array of sampler2D associated with this object.

uniform sampler2D gSampler0; // Dirt
uniform sampler2D gSampler1; // Sand
uniform sampler2D gSampler2; // Grss
uniform sampler2D gSampler3; // Rock
uniform sampler2D gSampler4; // Snow
uniform float     terrainMag;

uniform vec4 sunHeight;

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
    return ((texture2D( a, outtexture) * (fPosition.y - lower)) +
            (texture2D( b, outtexture) * (upper - fPosition.y))) / diff ;
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
        fragColor = texture2D( gSampler4, outtexture ); 
    // Snow->Rock
    else if ((fPosition.y > rockUpper) && (fPosition.y < snowLower) )
	fragColor = textureGradient( gSampler4, gSampler3, snowLower, rockUpper);
    // Rock
    else if ((fPosition.y > rockLower) && (fPosition.y <= rockUpper)) 
        fragColor = texture2D( gSampler3, outtexture );
    // Rock->Grass
    else if ((fPosition.y > grassUpper) && (fPosition.y < rockLower))
        fragColor = textureGradient( gSampler3, gSampler2, rockLower, grassUpper );
    // Grass
    else if (( fPosition.y > grassLower ) && (fPosition.y <= grassUpper ))
        fragColor = texture2D( gSampler2, outtexture );  
    // Grass->Sand
    else if ((fPosition.y > sandUpper) && (fPosition.y < grassLower))
        fragColor = textureGradient( gSampler2, gSampler1, grassLower, sandUpper );
    // Sand
    else if ((fPosition.y > sandLower ) && (fPosition.y <= sandUpper ))
        fragColor = texture2D( gSampler1, outtexture );  
    // Sand->Dirt
    else if ((fPosition.y > dirtUpper) && (fPosition.y < sandLower))
        fragColor = textureGradient( gSampler1, gSampler0, sandLower, dirtUpper );
    // Dirt
    else if ( fPosition.y <= dirtUpper )
        fragColor = texture2D( gSampler0, outtexture );  
    }

    else {
      fragColor = color ;
    }


/*
    // deal with all the lights, right now.
     int i;
     vec3 diffuse, specular;
     vec3 normal, cameraDir;

     float dist, distFactor;

     vec3 lightDir, halfAngle, specularColor ;

     float specularDot ;
     float diffuseDot  ;

     // initialize diffuse/specular lighting
     diffuse  = vec3(0.15, 0.12, 0.11);
     specular = vec3(0.15, 0.12, 0.11);

     // normalize the fragment normal and camera direction
     normal = normalize(fragmentNormal);
     cameraDir = normalize(cameraVector);

     // vec4 sample ;

     // loop through each light
     for ( i = 0; i < numLights && i < 5; ++i) {

       // calculate distance between 0.0 and 1.0
       dist = min(dot(lightVector[i], lightVector[i]), MAX_DIST_SQUARED) / MAX_DIST_SQUARED;
       distFactor = 1.0 - dist;

       // diffuse
       lightDir = normalize(lightVector[i]);
       diffuseDot = dot(normal, lightDir);
       diffuse += lightColor[i] * clamp(diffuseDot, 0.0, 1.0) * distFactor;

       // specular
       halfAngle = normalize(cameraDir + lightDir);
       specularColor = min(lightColor[i] + 0.5, 1.0);
       specularDot = dot(normal, halfAngle);
       specular += specularColor * pow(clamp(specularDot, 0.0, 1.0), 16.0) * distFactor;

     }



     //sample = vec4(1.0, 1.0, 1.0, 1.0);

     fragColor =  vec4(clamp(fragColor.xyz * diffuse + specular, 0.0, 1.0), fragColor.a) ;

     // fragColor = fragColor * vec4(clamp(sample.rgb * diffuse  + specular, 0.0, 1.0), sample.a) ;
*/
/*
     float sunH;
     sunH = sunHeight.y;

     sunH = sunH / 500.0 ;

     if( sunH > 0.88) {
       gl_FragColor = fragColor  ;
     }

     else if( sunH > 0.1) {
       gl_FragColor = vec4( sunH * fragColor.rgb, fragColor.w ) ;
     }

     else {
       gl_FragColor = vec4( fragColor.rgb * 0.1, fragColor.w ) ;
     }

*/

gl_FragColor = fragColor;

}
