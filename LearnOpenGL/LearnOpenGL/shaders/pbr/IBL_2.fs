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

void main()
{
	float pi = 3.1415926535;
	
	vec3 cdiff = texture(texDiffuse, v_uv).rgb;
	vec3 clight = vec3(0.8, 0.8, 0.8);
	cdiff = vec3(0.7, 0.2, 0.2);

			
	vec3 normal = normalize(v_normal);
	vec3 viewDir = normalize(v_viewDir);
	vec3 lightDir = normalize(v_lightDir);
	vec3 halfDir = normalize(viewDir + lightDir);
	vec3 reflectDir = normalize(reflect(-viewDir, normal));

	float nv = clamp(dot(normal, viewDir), 0, 1);
	float nl = clamp(dot(normal, lightDir), 0, 1);
	float nh = clamp(dot(normal, halfDir), 0, 1);
	float lh = clamp(dot(lightDir, halfDir), 0, 1);

	vec3 realAlbedo = cdiff - cdiff * metallic;
	vec3 realSpecularColor = mix(vec3(0.03, 0.03, 0.03), cdiff, metallic);

	float a = roughness * roughness;
	float a2 = a * a;

	float tmp1 = nh * nh * (a2 - 1) + 1;

	float D = a2 / (pi * tmp1 * tmp1);

	float k = (roughness + 1) * (roughness + 1) * 0.125;

	float Gl = nl / (nl * (1 - k) + k);
	float Gv = nv / (nv * (1 - k) + k);
	float G = Gl * Gv;

	float olh = 1 - lh;
	float olh5 = olh * olh * olh * olh * olh;
	vec3 F = realSpecularColor + (1 - realSpecularColor) * olh5;

	vec3 spec = max(D * F * G / (4 * nl * nv), 0);
	
	float max_spec = max(max(spec.r, spec.g), spec.b);
	vec3 diff = realAlbedo * (1 - spec);

	vec3 all = ( diff + spec) * clight * nl * 0.8;

	//color = vec4(all, 1);

	vec3 envColor = texture(texCube, reflectDir).rgb;
	float oma = 1.0f - a;
	vec3 envFresnel = (realSpecularColor + (max(vec3(oma, oma, oma), realSpecularColor) - realSpecularColor) * olh5);
	color = vec4(envColor * envFresnel + all + 0.2 * realAlbedo, 1);
}