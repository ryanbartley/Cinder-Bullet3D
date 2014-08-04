uniform mat4	ciModelViewProjection;
uniform mat4	ciModelView;
uniform mat3	ciNormalMatrix;

attribute vec4		ciPosition;
attribute vec3		ciNormal;
attribute vec4		ciColor;

varying vec4 vPosition;
varying vec3 vNormal;
varying vec4 vColor;

void main(void) {
	vPosition = ciModelView * ciPosition;
	vNormal = ciNormalMatrix * ciNormal;
	vColor = ciColor;
	gl_Position = ciModelViewProjection * ciPosition;
}