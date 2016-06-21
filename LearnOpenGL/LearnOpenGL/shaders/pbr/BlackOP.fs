#version 330 core

in vec2 v_uv;
in vec3 v_normal;
in vec3 v_tangent;
in vec3 v_viewDir;
in vec3 v_lightDir;

uniform sampler2D texDiffuse;
uniform sampler2D texSpecular;
 
out vec4 color;

vec3 Fresnel(vec3 cspec, vec3 lightDir, vec3 half)
{
	return cspec + (1 - cspec) * pow(1 - dot(lightDir, half), 5);
}

float Pow5(float x)
{
	return x * x * x * x * x;
}

//http://blog.selfshadow.com/publications/s2013-shading-course/lazarov/s2013_pbs_black_ops_2_slides_v2.pdf

void main() 
{
	float gloss = 0.8;
	float PI = 3.1415926535;
	float a = pow(8192, gloss);

	vec3 cdiff = texture(texDiffuse, v_uv).rgb;
	vec3 cspec = texture(texSpecular, v_uv).rgb;
	vec3 clight = vec3(1, 1, 1);

	vec3 normal = normalize(v_normal);
	vec3 viewDir = normalize(v_viewDir);
	vec3 lightDir = normalize(v_lightDir);

	vec3 halfDir = normalize(viewDir + lightDir);

	float nh = max(dot(normal, halfDir), 0);
	float hl = max(dot(halfDir, lightDir), 0);
	float nl = max(dot(normal, lightDir), 0);
	float nv = max(dot(normal, viewDir), 0);

	float D = (a + 2) * 0.125 * pow(nh, a);

	vec3 F = cspec + (1 - cspec) * Pow5(1 - hl);

	float k = 2 / (sqrt(PI * (a + 2)));

	float V = 1 / (((nl) * (1 - k) + k) * ((nv) * (1 - k) + k));

	vec3 spec = max(D * F * V, 0);

	vec3 all = (spec + cdiff) * clight * nl;

	color = vec4(all, 1);
}