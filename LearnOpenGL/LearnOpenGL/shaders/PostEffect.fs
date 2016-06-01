#version 330 core

in vec2 v_uv;

out vec4 color;

uniform sampler2D screen_texture;

void main()
{
	//color = vec4(v_uv, 0, 1);
	color = texture(screen_texture, v_uv);
}