#version 450
layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inUV;

out vec3 vertNormal;
out vec2 vertUV;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

void main()
{

	vertNormal = inNormal;
	vertUV = inUV;

	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(inPosition, 1.0);
}