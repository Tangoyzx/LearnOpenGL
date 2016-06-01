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
out vec3 v_normal;
out vec3 v_tangent;
out vec3 v_viewDir;
out vec3 v_lightDir;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0f);
	
	v_uv = uv;
	
	v_normal.x = invModel[0].x * normal.x + invModel[0].y * normal.y + invModel[0].z * normal.z;
	v_normal.y = invModel[1].x * normal.x + invModel[1].y * normal.y + invModel[1].z * normal.z;
	v_normal.z = invModel[2].x * normal.x + invModel[2].y * normal.y + invModel[2].z * normal.z;

	v_tangent = (model * vec4(tangent, 0.0)).xyz;

	vec3 world_pos = (model * vec4(position.xyz, 1.0)).xyz;

	v_viewDir = cameraPos - world_pos;
	v_lightDir = mainLight.xyz - world_pos * mainLight.w;
}