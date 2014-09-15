#version 330 core

layout (location = 0) in vec3       ciPosition;
layout (location = 1) in vec3       ciNormal;
layout (location = 2) in vec2       ciTexCoord;
layout (location = 3) in mat4		model_matrix;

uniform mat4 ciProjectionMatrix;
uniform mat4 ciViewMatrix;

void main() {
	gl_Position = ciProjectionMatrix * ciViewMatrix * model_matrix * vec4(ciPosition, 1.0);
}