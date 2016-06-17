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
 
out vec4 color;

void main()
{
	float pi = 3.1415926535;
	
	vec3 cdiff = texture(texDiffuse, v_uv).rgb;
	vec3 cspec = texture(texSpecular, v_uv).rgb;
	vec3 clight = vec3(0.8, 0.8, 0.8);

	cdiff = vec3(0.5, 0.2, 0.2);
	cspec = vec3(0.2, 0.2, 0.2);
			
	vec3 normal = normalize(v_normal);
	vec3 viewDir = normalize(v_viewDir);
	vec3 lightDir = normalize(v_lightDir);
	vec3 halfDir = normalize(viewDir + lightDir);
	vec3 reflectDir = normalize(reflect(-viewDir, normal));

	float nv = clamp(dot(normal, viewDir), 0, 1);
	float nl = clamp(dot(normal, lightDir), 0, 1);
	float nh = clamp(dot(normal, halfDir), 0, 1);
	float lh = clamp(dot(lightDir, halfDir), 0, 1);

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
	vec3 F = cspec + (1 - cspec) * olh5;

	vec3 spec = max(D * F * G / (4 * nl * nv), 0);
	
	float max_spec = max(max(spec.r, spec.g), spec.b);
	vec3 diff = cdiff * (1 - max_spec);

	vec3 all = ( diff + spec) * clight * nl * 0.8 + 0.2 * cdiff;

	//color = vec4(all, 1);


	vec2 EnvBRDF = texture(texLut, vec2(0.1, nv)).rg;
	//color = texture(texLut, v_uv);
	//color = vec4(EnvBRDF, 0, 1);

	vec3 ibl_color = texture(texCube, reflectDir).rgb * (cspec * EnvBRDF.x + EnvBRDF.y) * 0.5;
	
	color = vec4(ibl_color + all, 1);
}