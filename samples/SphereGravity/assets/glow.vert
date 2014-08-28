#version 150

uniform mat4	ciModelViewProjection;
uniform mat4	ciModelView;
uniform mat4	ciViewMatrix;
uniform mat3	ciNormalMatrix;

uniform vec4	uLightPos;

in vec4         ciPosition;
in vec3         ciNormal;

out vec4		Position;
out vec3		Normal;
out vec3		EyeDir;
out vec3		LightPos;
out vec3		LightDir;

void main()
{
	Normal			= ciNormalMatrix * ciNormal;
	Position		= ciModelView * ciPosition;
	
	EyeDir			= normalize( -Position.xyz );
	LightPos		= vec3( ciViewMatrix * uLightPos );
	LightDir		= normalize( LightPos - ( Position.xyz * uLightPos.w ) );
	
	gl_Position		= ciModelViewProjection * ciPosition;
}