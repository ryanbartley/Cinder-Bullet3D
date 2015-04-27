#version 330 core

layout (location = 0) in vec3       ciPosition;
layout (location = 1) in mat4		uModelMatrix;

uniform mat4 ciProjectionView;

void main() {
	gl_Position = ciProjectionView * uModelMatrix * vec4(ciPosition, 1.0);
}