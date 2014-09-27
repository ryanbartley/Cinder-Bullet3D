#version 330 core

uniform float ciElapsedSeconds;

uniform sampler2DShadow	uShadowMap;
uniform sampler2D	uStripeTex;
uniform vec4		uLightPos;
uniform float		uDepthBias;
uniform float		uRadius1;
uniform float		uRadius2;

in vec4				Position;
in vec3				Normal;
in vec2				TexCoord;
in vec3				EyeDir;
in vec4				ShadowCoord;
in vec3				LightPos;
in vec3				LightDir;
in float			DistPer1;
in float			DistPer2;
in vec4				GlowPos1;
in vec4				GlowPos2;

out vec4			FragColor;

const vec3 GRAY = vec3( 0.8, 0.8, 0.8 );
const vec3 BLUE = vec3( 0.0, 0.0, 1.0 );
const vec3 RED  = vec3( 1.0, 0.0, 0.0 );

float sampleBasic( vec4 sc ) {
	float shadow = textureProj( uShadowMap, sc );
	return shadow;
}

float getAttenuation( vec3 lightPos, float lightRadius, float lightCutoff )
{
	vec3 dir		= Position.xyz - lightPos.xyz;
	float dist		= max( length( dir ) - lightRadius, 0.0 );
	float atten		= 1.0 / ( 1.0 + lightCutoff * pow( dist, 2.0 ) );
	
	return atten;
}

void main()
{
	vec3	N = normalize( Normal );
	vec3	L = normalize( LightDir );
	vec3	C = normalize( -Position.xyz );
	vec3	R = normalize( -reflect( L, N ) );
	
	vec3	A = vec3( 0.15, 0.15, 0.15 );
	float NdotL = max( dot( N, L ), 0.0 );
	vec3	D = vec3( NdotL );
	vec3	S = pow( max( dot( R, C ), 0.0 ), 150.0 ) * vec3(1.0);
	
	float shadow = 1.0f;
	vec4 sc = ShadowCoord;
	sc.z	+= uDepthBias;
	shadow	= sampleBasic( sc );
	
	float eyeDiff = max( dot( Normal, EyeDir ), 0.0 );
	float eyeRim = pow( 1.0 - eyeDiff, 1.0 );
	
	vec3 lightCol		= getAttenuation( LightPos, 1.0, 0.00005 ) * GRAY;
	vec3 glowCol1		= getAttenuation( GlowPos1.xyz, uRadius1, 1.1 ) * RED + pow( DistPer1, 10.0 ) * RED;
	vec3 glowCol2		= getAttenuation( GlowPos2.xyz, uRadius2, 1.1 ) * BLUE + pow( DistPer2, 10.0 ) * BLUE;
	float stripeVal		= texture( uStripeTex, TexCoord ).r;
	
	vec3 finalGlowCol	= glowCol1 + glowCol2;
	vec3 finalDiffuse	= D * shadow * lightCol;
	vec3 finalSpecular	= S * shadow;

	FragColor.rgb		= finalGlowCol + finalGlowCol * stripeVal + finalDiffuse + finalSpecular;
	FragColor.a			= 1.0;
}


