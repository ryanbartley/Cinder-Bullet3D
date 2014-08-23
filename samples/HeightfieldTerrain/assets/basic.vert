#version 330

in vec3 position;

uniform mat4 ciModelViewProjection;

void main() {
	gl_Position = ciModelViewProjection * vec4(position, 1.0);
}