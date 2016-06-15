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

//http://blog.selfshadow.com/publications/s2013-shading-course/lazarov/s2013_pbs_black_ops_2_slides_v2.pdf

void main() 
{
	float a = 64.0;
	float pi = 3.1415926535;

	vec3 cdiff = texture(texDiffuse, v_uv).rgb;
	vec3 cspec = texture(texSpecular, v_uv).rgb;
	vec3 clight = vec3(1, 1, 1);

	vec3 normal = normalize(v_normal);
	vec3 viewDir = normalize(v_viewDir);
	vec3 lightDir = normalize(v_lightDir);

	vec3 half = normalize(viewDir + lightDir);

	float D = (a + 2) * 0.125 * pow(dot(normal, half), a) * 0.25 * pi;
	vec3 F = Fresnel(cspec, lightDir, half);
	float K = 2.0 / sqrt(pi * (a + 2));
	float V = 1.0 / ((dot(normal, lightDir) * (1 - K) + K) * (dot(normal, viewDir) * (1 - K) + K)); 

	vec3 spec = D * F * V;

	color = vec4((cdiff + spec) * clight * max(dot(normal, lightDir), 0), 1);
}