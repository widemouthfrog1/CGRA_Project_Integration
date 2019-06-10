#version 330 core

uniform mat4 uModelViewMatrix;
uniform mat4 uProjectionMatrix;
uniform vec3 uCameraPosition;

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;

out vec3 fragmentPosition;
out vec3 fragmentNormal;
out vec3 viewDirection;
out vec3 fragmentColor;
out vec3 actualPosition;

void main(){
    
    vec3 viewSpacePosition = (uModelViewMatrix * vec4(vertexPosition, 1)).xyz;
	fragmentPosition = viewSpacePosition;

	vec3 viewSpaceNormal = normalize(uModelViewMatrix * vec4(vertexNormal, 0)).xyz;
	fragmentNormal = viewSpaceNormal;

	vec3 viewSpaceCameraPosition = (uModelViewMatrix * vec4(uCameraPosition, 1)).xyz;
	viewDirection = normalize(viewSpacePosition - viewSpaceCameraPosition);

	actualPosition = vertexPosition;

    gl_Position = uProjectionMatrix * uModelViewMatrix * vec4(vertexPosition, 1);
}
