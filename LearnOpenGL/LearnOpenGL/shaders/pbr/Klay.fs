#version 330 core

in vec2 v_uv;
in vec3 v_normal;
in vec3 v_tangent;
in vec3 v_viewDir;
in vec3 v_lightDir;
 
out vec4 color;

vec3 Fresnel(vec3 cspec, vec3 lightDir, vec3 half)
{
	return cspec + (1 - cspec) * pow(1 - dot(lightDir, half), 5);
}

//http://www.klayge.org/wiki/index.php/%E5%9F%BA%E4%BA%8E%E7%89%A9%E7%90%86%E7%9A%84BRDF

void main() 
{
	float a = 64.0;

	vec3 cdiff = vec3(0.3, 0.58, .8);
	vec3 cspec = vec3(1, 1, 1);
	vec3 clight = vec3(1, 1, 1);

	vec3 normal = normalize(v_normal);
	vec3 viewDir = normalize(v_viewDir);
	vec3 lightDir = normalize(v_lightDir);

	vec3 half = normalize(viewDir + lightDir);

	vec3 spec = (a + 2.0) * 0.125 * pow(dot(normal, half), a) * Fresnel(cspec, lightDir, half);

	color = vec4((cdiff + spec) * clight * max(dot(normal, lightDir), 0), 1);
}