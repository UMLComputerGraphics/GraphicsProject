// Particle System Fragment Shader
// Author: Chris Compton, christopher_compton@student.uml.edu
// Source: http://ogldev.atspace.co.uk/www/tutorial27/tutorial27.html

uniform sampler2D gColorMap;
uniform float theta;

vec2 TexCoord; 
varying vec4 fColor;
varying float distance;
uniform int ftime;

vec4 hyper( float hue ) {

  #define S (1.0)
  #define V (1.0)
  #define M (0.0)
  #define K (F)

  float H, F, N, I;
  int i;

  H = (hue * 6.0);
  I = floor( H );
  F = (H - I);
  N = (1.0 - F);

  i = int(I);

 if (i == 0) return vec4( V, K, M, 1 );
 if (i == 1) return vec4( N, V, M, 1 );
 if (i == 2) return vec4( M, V, K, 1 );
 if (i == 3) return vec4( M, N, V, 1 );
 if (i == 4) return vec4( K, M, V, 1 );
 return vec4( V, M, N, 1 );

}

void main()
{
	//gl_FragColor = vec4(0.90, 0.45, 0.1, 1.0)/(distance*distance*64.0);
	//gl_FragColor = vec4(0.90, 0.45, distance, 1.0)/(distance*distance*64.0);

	// Nick St.Pierre's "Firework":
	gl_FragColor = vec4(0.90, 0.45, 0, 1.0)/(distance*8.0);
	
	// also pleasant!
	//gl_FragColor = vec4(1.0, 0.5, 0.0, 1.0)/(distance*distance*distance*1000.0);
	//gl_FragColor.b = distance*2.0;
}
