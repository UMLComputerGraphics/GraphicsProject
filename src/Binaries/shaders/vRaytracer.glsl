#version 150 
// VertexProgram

uniform mat4 uRotationMatrix;
uniform vec4 uCameraPosition;

in vec2 vRayPosition;
uniform int uDisplay;

out vec3 org, dir;

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

    // Convert the Translational matrix T into a world-coordinate vec3.
    // Multiply by vec4(0,0,0,1) to get (-x,-y,-z,1),
    // Then multiply by -1 to get (x,y,z,-1).
    // Then take (x,y,z).    
    org = (uCameraPosition * vec4(0,0,0,1) * -1).xyz;
    dir = normalize(uRotationMatrix * vec4(xx, yy, -1.0, 0.0)).xyz;
    
    float xPos = vRayPosition.x;
    float yPos = vRayPosition.y;
    
    gl_Position = vec4(xPos, -yPos, -1.0, 1.0);
    
}
