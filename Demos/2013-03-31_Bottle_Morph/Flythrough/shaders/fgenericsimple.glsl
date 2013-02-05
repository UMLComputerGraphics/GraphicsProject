// GENERIC Fragment SHADER
// Adadapted from this magnificent web site
// http://ogldev.atspace.co.uk/www/tutorial20/tutorial20.html

// attributes
//attribute vec4 vPosition ;
//attribute vec4 vNormal   ;

// sent to the fshader
varying vec4 color;
varying vec3 cameraVector;
varying vec3 fragmentNormal;
varying vec3 lightVector[8];

const float MAX_DIST = 2.5;
const float MAX_DIST_SQUARED = MAX_DIST * MAX_DIST;

// position/movement
//uniform mat4 P   ;
//uniform mat4 R   ;
//uniform mat4 T   ;
//uniform mat4 CTM ; // CTM is either PRT or TRP, depending.

// Lighting
uniform int numLights ;


uniform vec3 lightColor[8];
//uniform vec3 LightAmbientArray[];
//uniform vec3 LightDiffuseArray[];
//uniform vec3 LightSpecularArray[];

uniform vec3 LightPositionArray[12] ;
uniform vec3 LightDirectionArray[8];



void main() {

     int i;
     vec3 diffuse, specular;
     vec3 normal, cameraDir;

     float dist, distFactor;

     vec3 lightDir, halfAngle, specularColor ;

     float specularDot ;
     float diffuseDot ;

     // initialize diffuse/specular lighting
     diffuse = vec3(0.0, 0.0, 0.0);
     specular = vec3(0.0, 0.0, 0.0);

     // normalize the fragment normal and camera direction
     normal = normalize(fragmentNormal);
     cameraDir = normalize(cameraVector);

     vec4 sample ;

     // loop through each light
     for ( i = 0; i < numLights && i < 8; ++i) {

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

     sample = vec4(1.0, 1.0, 1.0, 1.0);
     gl_FragColor = vec4(clamp(sample.rgb * ( diffuse  ) + specular, 0.0, 1.0), sample.a);


     
     //color_scale = 0.375 * (vPosition.y+1.0);     
     //emergency debug
     //color = vec4(0.4, vPosition.y, 0.4, 1.0);


  //   if( color.x < 0.0 ) color.x = 0.0;
   // if( color.y < 0.0 ) color.y = 0.0;
    //if( color.z < 0.0 ) color.z = 0.0;


}
