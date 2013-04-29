///////////////////////////////////////////////////////////////////////////////
// Geometry shader
//
//
#version 120
#extension GL_EXT_geometry_shader4 : enable

// ∆t


/*
void doPassThrough() {

     Fun fact: GLSL is incredibly thick. So thick, in fact, that we need this 
     mind-numbing function.

     

     color = color ;
     fPosition = fPosition ;
     fTex = fTex;
     cameraVector = cameraVector;
     fragmentNormal = fragmentNormal;

     for ( int i = 0 ; i < 8 ; ++i ) { lightVector[i] = lightVector[i] ; }

}
*/


void main() {

//  doPassThrough();

  for( int i = 0; i < gl_VerticesIn; ++i) {

    gl_FrontColor = gl_FrontColorIn[i];

    gl_Position = gl_PositionIn[i];

    EmitVertex();

  }

}