#version 330 core

uniform mat4	ciProjectionMatrix;
uniform mat4	ciModelView;
uniform mat4	ciViewMatrix;
uniform mat3	ciNormalMatrix;

uniform mat4	uShadowMvp;
uniform vec4	uLightPos;
uniform vec3	uLightPos1;
uniform vec3	uLightPos2;
uniform float	uRadius1;
uniform float	uRadius2;
uniform vec3	uGlowPos1;
uniform vec3	uGlowPos2;

layout (location = 0) in vec4       ciPosition;
layout (location = 1) in vec3       ciNormal;
layout (location = 2) in vec2		ciTexCoord0;
layout (location = 3) in mat4		model_matrix;

out vec4		Position;
out vec3		Normal;
out vec2		TexCoord;
out vec3		EyeDir;
out vec3		LightPos;
out vec3		LightDir;
out vec4		ShadowCoord;
out float		DistPer1;
out float		DistPer2;
out vec4		GlowPos1;
out vec4		GlowPos2;

/* Bias matrix alters the clip coordinates so that x & y
 * lie between 0.0 and 1.0 for texture sampling. */
const mat4 biasMat  = mat4(	0.5, 0.0, 0.0, 0.0,
						   0.0, 0.5, 0.0, 0.0,
						   0.0, 0.0, 0.5, 0.0,
						   0.5, 0.5, 0.5, 1.0 );

float getDistFromLight( vec3 pos, vec4 lightPos, float size )
{
	vec3 dir = pos - lightPos.xyz;
	float dist = length( dir ) - size;
	dist = 1.0 - clamp( dist/100.0, 0.0, 1.0 );
	return dist;
}

void main()
{
	TexCoord			= ciTexCoord0;
	mat3 normalMatrix	= mat3( ciViewMatrix * model_matrix );
	Normal				= normalMatrix * ciNormal;
	Position			= ciViewMatrix * model_matrix * ciPosition;
	
	EyeDir				= normalize( -Position.xyz );
	LightPos			= vec3( ciViewMatrix * uLightPos );
	LightDir			= normalize( LightPos - ( Position.xyz * uLightPos.w ) );
	
	GlowPos1			= ciViewMatrix * vec4( uLightPos1, 1.0 );
	GlowPos2			= ciViewMatrix * vec4( uLightPos2, 1.0 );
	DistPer1			= getDistFromLight( Position.xyz, GlowPos1, uRadius1 );
	DistPer2			= getDistFromLight( Position.xyz, GlowPos2, uRadius2 );
	
	gl_Position			= ciProjectionMatrix * ciViewMatrix * model_matrix * ciPosition;
	ShadowCoord			= (biasMat * uShadowMvp * model_matrix) * ciPosition;
}