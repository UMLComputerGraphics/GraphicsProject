///////////////////////////////////////////////////////////////////////////////
// Geometry shader
//
//
#version 120
#extension GL_EXT_geometry_shader4 : enable

// ∆t

varying vec4 gColor;
varying vec4 fColor;

void doPassThrough() {

     /*Fun fact: GLSL is incredibly thick. So thick, in fact, that we need this 
     mind-numbing function.*/     

     fColor = gColor ;

}

void main() {

  doPassThrough();

  for( int i = 0; i < gl_VerticesIn; ++i) {

    //gl_FrontColor = gl_FrontColorIn[i];
    gl_Position = gl_PositionIn[i];

    //EmitVertex();

    gl_Position.x += 20.0 ;

    EmitVertex();

  }

}