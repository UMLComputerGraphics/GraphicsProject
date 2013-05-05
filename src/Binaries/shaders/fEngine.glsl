#version 120
// Sent from the Vertex Shader
varying vec4 fColor;
varying vec4 fPosition;
varying vec2 fTex;
varying vec3 fNormal;

// position/movement
uniform mat4 P;
uniform mat4 R;
uniform mat4 T;
uniform mat4 CTM;
uniform mat4 OTM;

const float materialShininess=0.1;
vec4 materialSpecular = vec4(0.1,0.1,0.1,0.5);

uniform float ftime;

uniform sampler2D sampler[];
uniform bool fIsTextured;

//general light stuff
uniform bool letMeSeeThatPhong; //pha phong phong phong
const int maxNumberOfLights = 5;
uniform int uNumberOfLights;
uniform vec4 LightAmbient,
             uLightPositions[maxNumberOfLights],
             uLightDiffuse[maxNumberOfLights],
             uLightSpecular[maxNumberOfLights];

uniform float uLightIntensity[maxNumberOfLights];

void main() { 

  vec4 baseColor;

  vec3 N = normalize(fNormal);

  if (fIsTextured) {
    baseColor = texture2D( sampler[0], fTex );
  } else {
    baseColor = fColor;
  }
    
  if (letMeSeeThatPhong) {

    gl_FragColor = LightAmbient;
    
    for( int i = 0; i < uNumberOfLights; i++ )
    {
      // Transform vertex position into eye coordinates
      vec3 pos = (fPosition).xyz;

      vec4 ambient = LightAmbient * baseColor;

      vec3 lightPos = (uLightPositions[i]).xyz;
      
      float dist = length(vec3(lightPos - pos));

      float att = 4.0 / (dist*dist);
    
      vec3 L = normalize(lightPos);
      vec3 E = normalize(-pos);
      vec3 H = normalize(L + E);

      float Kd = max( dot(L, N), 0.0 );
      vec4 diffuse = Kd * uLightDiffuse[i] * baseColor;

      float Ks = pow( max(dot(N, H), 0.0), materialShininess);
      vec4  specular = Ks * uLightSpecular[i] * materialSpecular;
      if( dot(L, fNormal) < 0.0) {
        specular = vec4(0.0, 0.0, 0.0, 1.0);
      }

	 //gl_FragColor = diffuse;

      gl_FragColor += vec4( (ambient + diffuse + specular).xyz, 1.0 ) * att * uLightIntensity[i];
    }
    //gl_FragColor = (1.0 / gl_FragColor.a) * gl_FragColor;
  } else {
    gl_FragColor = baseColor;
  }
}
