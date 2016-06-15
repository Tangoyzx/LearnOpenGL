#version 330 core

in vec3 v_pos;

out vec4 color;

uniform samplerCube cube_texture;

void main()
{
	color = texture(cube_texture, v_pos);

	//color = vec4(v_pos * 0.5 + 0.5, 1);
	//color = vec4(1, 0, 0, 1);
}