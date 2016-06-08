#version 330 core
 
layout (location = 0) in vec3 position;

layout (std140) uniform Matrices
{
	mat4 projection;	//4 * 16
	mat4 view;			//4 * 16
	vec3 cameraPos;		//16
	vec4 mainLight;		//16
};

out vec3 v_pos;

void main() 
{
	vec3 worldPos = position + cameraPos;

	v_pos = position;
	gl_Position = projection * view * vec4(worldPos, 1);
}