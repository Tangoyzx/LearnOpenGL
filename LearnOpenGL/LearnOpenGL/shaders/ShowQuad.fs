#version 330 core

in vec2 v_uv;

uniform sampler2D customTex;

out vec4 color;

void main() 
{
	color = texture(customTex, v_uv);
}