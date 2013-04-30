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
vec4 materialDiffuse = vec4(0.8,0.8,0.8,0.8);

uniform float ftime;

uniform sampler2D sampler[];
uniform bool fIsTextured;

//general light stuff
uniform bool letMeSeeThatPhong; //pha phong phong phong
const int maxNumberOfLights = 5;
uniform int uNumberOfLights;
uniform vec4 LightAmbient, uLightPositions[maxNumberOfLights], uLightDiffuse[maxNumberOfLights], uLightSpecular[maxNumberOfLights];

void main() { 

  vec4 baseColor;

  if (fIsTextured) {
    baseColor = texture2D( sampler[0], fTex );
  } else {
    baseColor = fColor;
  }
    
  if (letMeSeeThatPhong) {

    gl_FragColor = LightAmbient + baseColor;
    
    for(int i=0; i<uNumberOfLights; i++)
    {
      // Transform vertex position into eye coordinates
      vec3 pos = (CTM * fPosition).xyz;

      vec4 ambient = LightAmbient * baseColor;

      vec3 lightPos = (uLightPositions[i]).xyz;
          
      vec3 L = normalize(lightPos - pos);
      vec3 E = normalize(-pos);
      vec3 H = normalize(L + E);

      float Kd = max( dot(L, fNormal), 0.0 );
      vec4 diffuse = Kd * uLightDiffuse[i] * materialDiffuse;

      float Ks = pow( max(dot(fNormal, H), 0.0), materialShininess);
      vec4  specular = Ks * uLightSpecular[i] * materialSpecular;
      if( dot(L, fNormal) < 0.0) {
        specular = vec4(0.0, 0.0, 0.0, 1.0);
      }

      gl_FragColor = diffuse + specular + gl_FragColor;
    }
    gl_FragColor = (1.0 / gl_FragColor.a) * gl_FragColor;
  } else {
    gl_FragColor = baseColor;
  }
}