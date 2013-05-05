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

    float width = 300;
    float height = 300;
    
    float invWidth = 1 / width;
    float invHeight = 1 / height;

    float fov = 30;
    float aspectratio = width / height;
    float angle = tan(45 * fov / 180);
    
    float x = vRayPosition.x * 150 + 150;
    float y = vRayPosition.y * 150 + 150;
    
    float xx = (2 * ((x + 0.5) * invWidth) - 1) * angle * aspectratio;
    float yy = (1 - 2 * ((y + 0.5) * invHeight)) * angle;
    
    org = (T * vec4(0,0,0,-1)).xyz;
    dir = normalize(R * vec4(xx, yy, -1.0, 0.0)).xyz;
    
    float xPos = vRayPosition.x;
    float yPos = vRayPosition.y;
    
    gl_Position = vec4(xPos, -yPos, -1.0, 1.0);
}
