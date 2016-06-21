#version 330 core
 
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 tangent;
layout (location = 3) in vec2 uv;

layout (std140) uniform Matrices
{
	mat4 projection;	//4 * 16
	mat4 view;			//4 * 16
	vec3 cameraPos;		//16
	vec4 mainLight;		//16
	vec4 projParams;	//16
};


out vec2 v_uv;
out vec3 v_cameraPos;

void main()
{
	gl_Position = vec4(position.xy, 0, 1);

	v_uv = uv;
	v_cameraPos = cameraPos;
}