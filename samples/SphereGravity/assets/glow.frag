#version 150

uniform vec4		uLightPos;
uniform vec3		uColor;

in vec4				Position;
in vec3				Normal;
in vec3				EyeDir;
in vec3				LightPos;
in vec3				LightDir;

out vec4			FragColor;

float getAttenuation( vec3 lightPos, float lightRadius, float lightCutoff )
{
	vec3 dir		= Position.xyz - lightPos.xyz;
	float dist		= max( length( dir ) - lightRadius, 0.0 );
	float atten		= 1.0 / ( 1.0 + lightCutoff * pow( dist, 2.0 ) );
	
	return atten;
}

void main()
{
	vec3 N		= normalize( Normal );				// Normal in view space
	vec3 L		= normalize( LightDir );			// Light Direction in view space
//	vec3 C		= normalize(-Position.xyz );		// Vector towards camera in view space
//	vec3 R		= normalize(-reflect( L, N ) );		// Light reflection off surface vector
	
	// Diffuse factor
	vec3 A		= vec3( 0.1 );						// Ambient
	float NdotL = max( dot( N, L ), 0.0 );		
	vec3 D		= vec3( NdotL );					// Diffuse
//	vec3 S		= pow( max( dot( R, C ), 0.0 ), 10.0 ) * vec3(1.0);	// Specular
		
	float att			= getAttenuation( LightPos.xyz, 10.0, 0.0005 );
	
	FragColor.rgb		= uColor + att * uColor * D + A;
	FragColor.a			= 1.0;
}


