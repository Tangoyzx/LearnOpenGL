#version 330 core

in vec2 v_uv;
in vec3 v_normal;
in vec3 v_tangent;
in vec3 v_viewDir;
in vec3 v_lightDir;

uniform sampler2D texDiffuse;
uniform sampler2D texSpecular;

uniform float roughness;
 
out vec4 color;

vec3 Fresnel(vec3 cspec, vec3 lightDir, vec3 half)
{
	return cspec + (1 - cspec) * pow(1 - dot(lightDir, half), 5);
}

//http://www.klayge.org/wiki/index.php/%E5%9F%BA%E4%BA%8E%E7%89%A9%E7%90%86%E7%9A%84BRDF

void main() 
{
	float a = pow(8192, 1- roughness);

	vec3 cdiff = texture(texDiffuse, v_uv).rgb;
	vec3 cspec = texture(texSpecular, v_uv).rgb;
	vec3 clight = vec3(1, 1, 1);
	cdiff = vec3(0.8, 0.2, 0.2);
	cspec = vec3(0.2, 0.2, 0.2);

	vec3 normal = normalize(v_normal);
	vec3 viewDir = normalize(v_viewDir);
	vec3 lightDir = normalize(v_lightDir);

	vec3 half = normalize(viewDir + lightDir);

	vec3 spec = (a + 2.0) * 0.125 * pow(dot(normal, half), a) * Fresnel(cspec, lightDir, half);

	float max_spec = max(max(spec.r, spec.g), spec.b);

	vec3 diff = cdiff * (1 - max_spec);

	color = vec4((diff + spec) * clight * max(dot(normal, lightDir), 0), 1);
}