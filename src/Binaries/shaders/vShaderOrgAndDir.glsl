#version 150 
// VertexProgram

uniform mat4 uRotationMatrix;
uniform vec4 uCameraPosition;

in vec2 vRayPosition;
uniform int uDisplay;

out vec3 org, dir;

void main()
{

    float x = vRayPosition.x;
    float y = vRayPosition.y;

    org = (uRotationMatrix * (uCameraPosition + vec4(x, y, 0.0, 0.0))).xyz;
    dir = normalize(uRotationMatrix * vec4(0, 0, -1.0, 0.0)).xyz;
    
    float xPos = vRayPosition.x / 2;
    float yPos = vRayPosition.y;
    
    if(uDisplay == -1) {
		xPos -= 0.5;
    } else if(uDisplay == 1) {
    	xPos += 0.5;
    } else {
    	xPos = vRayPosition.x;
    }
    
    gl_Position = vec4(xPos, yPos, -1.0, 1.0);
}
