///////////////////////////////////////////////////////////////////////////////
// Geometry shader
//
//
#version 120
#extension GL_EXT_geometry_shader4 : enable

// ∆t


// These are passed-through to the fragment shader.

varying vec4 color;
varying vec4 fPosition;
varying vec2 outtexture;
varying vec3 cameraVector;
varying vec3 fragmentNormal;
varying vec3 lightVector[8];


void doPassThrough() {

     /*

     Fun fact: GLSL is incredibly thick. So thick, in fact, that we need this 
     mind-numbing function.

     */

     color = color ;
     fPosition = fPosition ;
     outtexture = outtexture;
     cameraVector = cameraVector;
     fragmentNormal = fragmentNormal;

     for ( int i = 0 ; i < 8 ; ++i ) { lightVector[i] = lightVector[i] ; }

}



void main() {

  doPassThrough();

  for( int i = 0; i < gl_VerticesIn; ++i) {

    gl_FrontColor = gl_FrontColorIn[i];

    gl_Position = gl_PositionIn[i];

    EmitVertex();

  }

}