#version 330 core

in vec2 v_uv;
in vec3 v_normal;
in vec3 v_tangent;
in vec3 v_viewDir;
in vec3 v_lightDir;
 
out vec4 color;

uniform samplerCube cube_texture;

void main() 
{
	vec3 normal = normalize(v_normal);
	vec3 viewDir = normalize(v_viewDir);
	vec3 lightDir = normalize(v_lightDir);

	vec3 reflect = normalize(reflect(-viewDir, normal));

	color = textureLod(cube_texture, reflect, 0.1);
}