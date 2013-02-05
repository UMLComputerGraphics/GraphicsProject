// attributes
attribute vec4 vPosition;
attribute vec4 vColor;
attribute vec3 vNormal;

uniform bool vIsTextured;
uniform mat4 vObjMat;

// sent to the fshader
varying vec4 color;

// position/movement
uniform mat4 P;
uniform mat4 R;
uniform mat4 T;
uniform mat4 CTM; // CTM is either PRT or TRP, depending.


// color/lighting
uniform vec4 AmbientProduct,  DiffuseProduct,  SpecularProduct  ;
uniform vec4 AmbientProduct2, DiffuseProduct2, SpecularProduct2 ;
uniform vec4 LightPosition ;
uniform vec4 LightPosition2;
uniform float Shininess;
varying vec4 finalLight;


void main() {

   // Positioning Code
   #ifdef POSTMULT
   gl_Position = vPosition * CTM;
   #else
   gl_Position = CTM * vPosition;
   #endif
   
  // phong model for lighting.
  // ... I think.

    //vec3 pos = normalize(vPosition.xyz);
    vec3 pos = normalize(vPosition.xyz);

    vec3 L = normalize( (LightPosition.xyz) - pos );
    vec3 E = normalize( -(T * vec4(0.0, 0.0, 0.0, 1.0)).xyz );
    vec3 H = normalize( L + E );

    vec3 L2 = normalize( (LightPosition2.xyz) - pos );
    vec3 E2 = normalize( -(T * vec4(0.0, 0.0, 0.0, 1.0)).xyz );
    vec3 H2 = normalize( L2 + E2 );

    // Transform vertex normal into eye coordinates
    vec3 N = vNormal;


    // Compute terms in the illumination equation
    vec4 ambient = AmbientProduct;

    float Kd = max( dot(L, N), 0.0 );
    vec4  diffuse = Kd*DiffuseProduct;

    //float Ks = pow( max(dot(N, H), 0.0), Shininess );
    //vec4  specular = Ks * SpecularProduct;

    vec4 specular = max( pow( max( dot(N, H), 0.0),
    	 	    	      Shininess) * SpecularProduct,
    	 	    	 0.0);


    // if ( specular.x < 0.0 ) specular= -specular;
    if( dot(L, N) < 0.0 ) {
      specular = vec4(0.0, 0.0, 0.0, 1.0);
    }


    // same procedure, second light.

    vec4 ambient2 = AmbientProduct2;

    float Kd2 = max( dot(L2, N), 0.0 );
    vec4  diffuse2 = Kd2*DiffuseProduct2;

    vec4 specular2 = max( pow( max( dot(N, H2), 0.0),
    	 	    	      Shininess) * SpecularProduct2,
    	 	    	 0.0);

    // if ( specular2.x < 0.0 ) specular2= -specular2;
    if( dot(L2, N) < 0.0 ) {
      specular2 = vec4(0.0, 0.0, 0.0, 1.0);
    }


   // compute the combination of these two sources.

    //vec3 lightsum = vec3( (ambient + diffuse + specular).xyz) ;
    vec3 lightsum = vec3( (ambient + diffuse + specular + ambient2 + diffuse2 + specular2).xyz) ;

    // We need to make sure that lightsum's elements are floats between zero and (positive) one:
    // New lighting strategies are welcome!

    if (lightsum.x >= 1.0 ) lightsum.x = 1.0;
    if (lightsum.y >= 1.0 ) lightsum.y = 1.0;
    if (lightsum.z >= 1.0 ) lightsum.z = 1.0;


// COMMENT WAD
/*   vec3 lightsum = vec3(ambient.xyz); // debug setup for lighting
    lightsum = lightsum + vec3(specular.xyz);
    lightsum = lightsum + vec3(diffuse.xyz);   */
    // This right here is how we take the original color of the surface into consideration.
    // We should talk about a high level way to manage this. Lighting will require some organized code.
    // ACTUALLY NAW, we will do this inside the fragment shader and it will be faster?
   /* finalLight = vec4( vColor.x * lightsum.x,
        	       vColor.y * lightsum.y,
		       vColor.z * lightsum.z,
		       1.0  ) ;
*//*

    color = vec4( vColor.x * lightsum.x,
    	    	  vColor.y * lightsum.y,
		  vColor.z * lightsum.z,
		  1.0  ) ;                */

// END COMMENT WAD


   // Remove this extra 'lightsum' layer when you have some time (replace all references to lightsum with finalLight.)

   finalLight.xyz = lightsum ;
   finalLight.w = 1.0;

   color = vColor;

   color.a = 1.0; // do we need this? was it a workaround?

}
