#version 150 
// VertexProgram

in vec2 vRayPosition;

uniform vec3 uCameraPosition;

out vec3 org, dir;

void main()
{
    gl_Position = vec4(vRayPosition, -1.0, 1.0);
    org = uCameraPosition;
    dir = normalize(vec3(vRayPosition.x, vRayPosition.y, -1.0));
}
