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
	vec4 projParams;	//16
	mat4 invProjection;	//4 * 16
	mat4 invView;		//4 * 16
};


out vec2 v_uv;
out vec3 v_pos;
out vec3 v_normal;

void main()
{
	v_pos = (view * model * vec4(position, 1.0f)).xyz;
	gl_Position = projection * vec4(v_pos, 1);

	//v_pos = (view * model * vec4(position, 1.0f)).xyz;
	//gl_Position = projection * vec4(v_pos, 1);

	v_uv = uv;

	vec3 worldNormal;
	
	worldNormal.x = invModel[0].x * normal.x + invModel[0].y * normal.y + invModel[0].z * normal.z;
	worldNormal.y = invModel[1].x * normal.x + invModel[1].y * normal.y + invModel[1].z * normal.z;
	worldNormal.z = invModel[2].x * normal.x + invModel[2].y * normal.y + invModel[2].z * normal.z;

	v_normal = (view * vec4(worldNormal, 0)).xyz;
}