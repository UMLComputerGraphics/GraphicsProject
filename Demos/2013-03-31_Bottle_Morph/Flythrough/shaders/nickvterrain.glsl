////////////////////////////////////////////////////////////////////////////////
// Vertex shader for the terrain gen demo

// attributes
attribute vec4 vPosition;
attribute vec4 vColor;
// sent to the fshader
varying vec4 color;

// attribute vec4 vNormal;


// position/movement
uniform mat4 P;
uniform mat4 R;
uniform mat4 T;
uniform mat4 CTM; // CTM is either PRT or TRP, depending.

varying float color_scale ;

// Lighting 
//uniform int numLights ;

// these are going to remain unimplemented in this version of out Vertex shader
//uniform vec3 LightColor[];
//uniform vec3 LightAmbientArray[];
//uniform vec3 LightDiffuseArray[];
//uniform vec3 LightSpecularArray[];
//uniform vec3 LightPositionArray[] ;
//uniform vec3 LightDirectionArray[];
// vec3 LightDistance[];

void main() {

     float distanceX;
     float distanceY;
     float distanceZ;
     float distance;

     float h;

     vec4 temp ;

     gl_Position = CTM *  vPosition;
     color_scale = 0.375 * (vPosition.y+1.0);     


     gl_Position = CTM * vPosition;
     color = vColor;
     //color_scale = 0.5 * vPosition.y;     
     //color = color_scale * vec4( 0.65, 0.31, 0.0, 1.0 );
     //color = vec4( 0,0,0,1);

//begin new crazy stuff
//     temp = T*vec4(0.0,0.0,0.0,1.0);



//     distanceX =(T[0][3] < 0.0) ? T[0][3] + vPosition.x :  T[0][3] - vPosition.x ;
//     //distanceY =(T[1][3] < 0.0) ? T[1][3] + vPosition.y :  T[1][3] - vPosition.y ;
//     distanceZ =(T[2][3] < 0.0) ? T[2][3] + vPosition.z :  T[2][3] - vPosition.z ;

//end crazy
//     distanceX =(T[0][3] < 0.0) ? T[0][3] + vPosition.x :  T[0][3] - vPosition.x ;
//     distanceY =(T[1][3] < 0.0) ? T[1][3] + vPosition.y :  T[1][3] - vPosition.y ;
//     distanceZ =(T[2][3] < 0.0) ? T[2][3] + vPosition.z :  T[2][3] - vPosition.z ;

     distanceX = T[0][3] - vPosition.x ;
     distanceY = T[1][3] - vPosition.y ;
     distanceZ = T[2][3] - vPosition.z ;

     distance  = (distanceX*distanceX) + 
     	       	 (distanceY*distanceY) +
		 (distanceZ*distanceZ) ;

//     distance = dot(temp, temp);

     distance  = sqrt(distance);

     distance  = sqrt(distance); // fudge

     // this transforms it into a falloff
     //distance = 1.0 - distance;


     if ( distance > 0.00000000 ) {
        distance = 1.0 / distance ;
        //color = color - or *?  vec4(distance, distance, distance, 0.0);
        color =  vec4(distance, distance, distance, 0.0);
     
     } else {
      
        color = vec4(0.90, 0.90, 0.90, 1.0);

     }

//     color = color * vec4( 0.625, 0.375, 1.0-h, 1.0 ) ;

       h = vPosition.y;

//###
     color = vec4( 0.375*h, sqrt(h*h*0.625), (1.0-h)/2.0, 1.0 ) ;
//###


//PIC:
//     color = vec4( 0.375*h, 0.625, 1.0-h, 1.0 ) ;

//       color = vec4( h*h -0.675*h - 0.2,
//       	       	     0.625+( h * 0.125 ),
//		     1.0-h, 
//		     1.0 ) ;

//   color = color * color_scale * vec4( 1.0, 1.0, 0.0, 1.0 ) ;


     //emergency debug
     //color = vec4(0.4, sqrt(h), 0.4, 1.0);


     if( color.x < 0.000 ) color.x = 0.0;
     if( color.y < 0.000 ) color.y = 0.0;
     if( color.z < 0.000 ) color.z = 0.0;


}
