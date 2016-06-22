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
 
out vec4 color;

float Pow5(float x)
{
	return x * x * x * x * x;
}

vec3 Fresnel(vec3 cspec, vec3 lightDir, vec3 half)
{
	return cspec + (1 - cspec) * Pow5(1 - dot(lightDir, half));
}

void main()
{
	float PI = 3.1415926535;
	
	vec3 cdiff = texture(texDiffuse, v_uv).rgb;
	float cmetallic = texture(texSpecular, v_uv).r * metallic;
	vec3 clight = vec3(0.8, 0.8, 0.8);
			
	vec3 normal = normalize(v_normal);
	vec3 viewDir = normalize(v_viewDir);
	vec3 lightDir = normalize(v_lightDir);
	vec3 halfDir = normalize(viewDir + lightDir);
	vec3 reflectDir = normalize(reflect(-viewDir, normal));

	float nv = clamp(dot(normal, viewDir), 0, 1);
	float nl = clamp(dot(normal, lightDir), 0, 1);
	float nh = clamp(dot(normal, halfDir), 0, 1);
	float hl = clamp(dot(lightDir, halfDir), 0, 1);

	vec3 realAlbedo = cdiff - cdiff * cmetallic;
	vec3 realSpecularColor = mix(vec3(0.03, 0.03, 0.03), cdiff, cmetallic);

	float a = pow(8192, 1 - roughness);

	float D = (a + 2) * 0.125 * pow(nh, a);

	vec3 F = realSpecularColor + (1 - realSpecularColor) * Pow5(1 - hl);

	float k = 2 / (sqrt(PI * (a + 2)));

	float V = 1 / (((nl) * (1 - k) + k) * ((nv) * (1 - k) + k));

	vec3 spec = max(D * F * V, 0);

	vec3 all = (spec + realAlbedo) * clight * nl;

	//color = vec4(all, 1);

	vec3 envColor = texture(texCube, reflectDir).rgb;
	float oma = 1.0f - a;
	vec3 envFresnel = (realSpecularColor + (max(vec3(oma, oma, oma), realSpecularColor) - realSpecularColor) * Pow5(1 - hl));
	color = vec4(envColor * envFresnel + all + 0.2 * realAlbedo, 1);
}