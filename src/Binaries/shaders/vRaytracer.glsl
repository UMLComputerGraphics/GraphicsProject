#version 150 
// VertexProgram

// Standard Attributes
in vec2 vRayPosition;
in vec4 vPosition;
in vec4 vColor;
in vec3 vNormal;
in vec2 vTex;

// sent to the fshader
out vec4 fColor;
out vec4 fPosition;
out vec2 fTex;
out vec3 fNormal;
out vec3 org, dir;

// position/movement
uniform mat4 P;   // Camera Perspective: Vertices are transformed to Screen coordinates.
uniform mat4 R;   // Camera Rotations only.
uniform mat4 T;   // Camera Translations only.
uniform mat4 CTM; // Camera transformations: Vertices are transformed to Camera coordinates with world scaling.
uniform mat4 OTM; // Object Transformations: Objects are adjusted to world coordinates.

// IsTextured boolean.
uniform bool fIsTextured;


void main() {

    float x = vRayPosition.x;
    float y = vRayPosition.y;
    
    org = (T * vec4(0,0,0,-1)).xyz;
    dir = normalize(transpose(R) * vec4(x, y, -1.0, 0.0)).xyz;
    
    float xPos = vRayPosition.x;
    float yPos = vRayPosition.y;
    
    gl_Position = vec4(xPos, yPos, -1.0, 1.0);
}
