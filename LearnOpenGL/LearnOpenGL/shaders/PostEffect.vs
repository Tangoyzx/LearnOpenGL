#version 330 core
 
layout (location = 0) in vec3 position;

out vec2 v_uv;

void main() 
{
	gl_Position = position;
	v_uv = position.xy * 0.5 + 0.5;
}