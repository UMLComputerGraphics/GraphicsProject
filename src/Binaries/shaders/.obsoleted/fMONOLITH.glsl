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

uniform float ftime; //for Eric's 1337 h@x

//general light stuff
uniform bool letMeSeeThatPhong; //pha phong phong phong
const vec4 LightAmbient = vec4(0.5, 0.5, 0.5, 1.0);
//TODO: MAKE THIS MOVE WITH TOP OF CANDLE
vec4 LightPosition = vec4(1.0, 1.0, 1.0, 0.0);
uniform vec4 MaterialAmbient, MaterialDiffuse, MaterialSpecular;
uniform float MaterialShininess;

//general candle stuff
const float dlight      = 0.025;
float lightness = 0.5;
float lasttime = 0.0;

//returns a vec4 containing the camera's light contribution
vec4 aRomanticEvening()
{
  if (lasttime != ftime)
  {
    float r = fract(sin(dot(vec2(ftime, ftime) ,vec2(12.9898,78.233))) * 43758.5453);
    lightness += ((floor(r) == floor(r + 0.5)) ? dlight : -dlight);
    lasttime = ftime;
    if (lightness < 0.0) lightness = 0.0;
    if (lightness > 1.0) lightness = 1.0;    
  }
  return lightness * vec4(1.0,1.0,1.0,1.0);
}

void main() 
{
    // Transform vertex position into eye coordinates
    vec3 pos = (fPosition).xyz;

    // Compute terms in the illumination equation    
    
    gl_FragColor = LightAmbient * fColor * aRomanticEvening();
    
    if (letMeSeeThatPhong)
    {          
      vec4 ambient = LightAmbient * fColor;
      gl_FragColor = ambient;
    
        vec3 lightPos = (CTM * OTM * LightPosition).xyz;
        
        vec3 L = normalize(lightPos - pos);
        vec3 E = normalize(-pos);
        vec3 H = normalize(L + E);
          
        float Kd = max( dot(L, fNormal), 0.0 );
        vec4  diffuse = Kd * aRomanticEvening() * ambient;
    
        float Ks = pow( max(dot(fNormal, H), 0.0), 0.5);
        vec4  specular = Ks * vec4(0.0,0.0,0.0,1.0) * fColor;
        if( dot(L, fNormal) < 0.0) {
            specular = vec4(0.0, 0.0, 0.0, 1.0);
        } 
    
        gl_FragColor = gl_FragColor + diffuse + specular;      
    }
} 

