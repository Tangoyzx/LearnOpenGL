#version 330 core

in vec2 v_uv;
in vec3 v_normal;
in vec3 v_tangent;
in vec3 v_viewDir;
in vec3 v_lightDir;

uniform sampler2D texDiffuse;
uniform sampler2D texSpecular;
uniform sampler2D texLut;
uniform samplerCube texCube;

uniform float roughness;
uniform float metallic;
uniform vec3 baseColor;
 
out vec4 color;

vec3 EnvBRDFApprox(vec3 SpecularColor, float Roughness, float NoV)
{
	const vec4 c0 = vec4(-1, -0.0275, -0.572, 0.022);
	const vec4 c1 = vec4( 1, 0.0425, 1.04, -0.04 );
	vec4 r = Roughness * c0 + c1;
	float a004 = min( r.x * r.x, exp2( -9.28 * NoV ) ) * r.x + r.y;
	vec2 AB = vec2( -1.04, 1.04 ) * a004 + r.zw;

	return SpecularColor * AB.x + AB.y;
}

float PhongApprox(float Roughness, float RoL)
{
	float a = Roughness * Roughness;
	a = max(a, 0.008);
	float a2 = a * a;
	float rcp_a2 = 1.0 / (a2);
	float c = 0.72134752 * rcp_a2 + 0.39674113;
	float p = rcp_a2 * exp2(c * RoL - c);

	return min(p, rcp_a2);
}

void main()
{
	float PI = 3.1415926535;

	vec3 cdiff = texture(texDiffuse, v_uv).rgb;
	float Specular = 1;
	vec3 clight = vec3(0.8, 0.8, 0.8);
	vec4 all = vec4(0, 0, 0, 1);

	float DielectricSpecular = 0.08 * Specular;
	vec3 DiffuseColor = baseColor - baseColor * metallic;
	vec3 SpecularColor = (DielectricSpecular - DielectricSpecular * metallic) + baseColor * metallic;

	vec3 normal = normalize(v_normal);
	vec3 viewDir = normalize(v_viewDir);
	vec3 lightDir = normalize(v_lightDir);
	vec3 halfDir = normalize(viewDir + lightDir);
	vec3 reflectDir = normalize(reflect(-viewDir, normal));

	float nv = clamp(dot(normal, viewDir), 0, 1);
	float nl = clamp(dot(normal, lightDir), 0, 1);
	float nh = clamp(dot(normal, halfDir), 0, 1);
	float lh = clamp(dot(lightDir, halfDir), 0, 1);
	float rl = clamp(dot(lightDir, reflectDir), 0, 1);

	SpecularColor = EnvBRDFApprox(SpecularColor, roughness, nv);

	all.rgb = all.rgb + nl * clight * (DiffuseColor + SpecularColor * PhongApprox(roughness, rl));


	//IBL
	vec3 SpecularIBL = texture(texCube, reflectDir).rgb * EnvBRDFApprox(SpecularColor, roughness, nv);

	all.rgb = all.rgb + SpecularIBL;

	color = all;

}