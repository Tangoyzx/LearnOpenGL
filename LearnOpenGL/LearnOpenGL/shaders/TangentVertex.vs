#version 330 core
 
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 tangent;
layout (location = 3) in vec2 uv;

uniform mat4 model;			//4 * 16
uniform mat4 invModel;		//4 * 16

layout (std140) uniform Matrices
{
	mat4 projection;	//4 * 16
	mat4 view;			//4 * 16
	vec3 cameraPos;		//16
	vec4 mainLight;		//16
};

out vec2 v_uv;
out vec3 v_tangent_viewDir;
out vec3 v_tangent_lightDir;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0f);
	
	v_uv = uv;
	
	vec3 binormal = cross(tangent, normal);

	mat3 rotation = transpose(mat3(tangent, binormal, normal));

	vec3 world_pos = (model * vec4(position.xyz, 1.0)).xyz;

	vec3 world_viewDir = cameraPos - world_pos;
	vec3 world_lightDir = mainLight.xyz - world_pos * mainLight.w;

	vec3 obj_viewDir = (invModel * vec4(world_viewDir, 0)).xyz;
	vec3 obj_lightDir = (invModel * vec4(world_lightDir, 0)).xyz;

	v_tangent_viewDir = (rotation * obj_viewDir).xyz;
	v_tangent_lightDir = (rotation * obj_lightDir).xyz;
}