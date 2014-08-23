#version 150

in VertexData	{
	vec4 position;
	vec3 normal;
	vec4 color;
} vVertexIn;

out vec4 oColor;

void main(void) {
	// set diffuse and specular colors
	vec3 cDiffuse = vVertexIn.color.rgb;
	vec3 cSpecular = vec3(0.3, 0.3, 0.3);

	// light properties in view space
	vec3 vLightPosition = vec3(0.0, 0.0, 0.0);

	// lighting calculations
	vec3 vVertex = vVertexIn.position.xyz;
	vec3 vNormal = normalize( vVertexIn.normal );
	vec3 vToLight = normalize( vLightPosition - vVertex );
	vec3 vToEye = normalize( -vVertex );
	vec3 vReflect = normalize( -reflect(vToLight, vNormal) );

	// diffuse coefficient
	vec3 diffuse = max( dot( vNormal, vToLight ), 0.0 ) * cDiffuse;

	// specular coefficient with energy conservation
	const float shininess = 20.0;
	const float coeff = (2.0 + shininess) / (2.0 * 3.14159265);
	vec3 specular = pow( max( dot( vReflect, vToEye ), 0.0 ), shininess ) * coeff * cSpecular;

	// to conserve energy, diffuse and specular colors should not exceed one
	float maxDiffuse = max(diffuse.r, max(diffuse.g, diffuse.b));
	float maxSpecular = max(specular.r, max(specular.g, specular.b));
	float fConserve = 1.0 / max(1.0, maxDiffuse + maxSpecular);

	// final color
	oColor.rgb = (diffuse + specular) * fConserve;
	oColor.a = 1.0;
}