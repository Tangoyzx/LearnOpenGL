#version 330 core

in vec2 v_uv;

out float color;

uniform sampler2D tex_ssao;

const vec2 pixelUV = vec2( 1.0 / 640.0, 1.0 / 640.0);

void main() 
{
	float sum = 0;
	for(int i = -2; i < 2; i++)
	{
		for(int j = -2; j < 2; j++)
		{
			vec2 newUv = v_uv + pixelUV * vec2(i, j);
			sum += texture(tex_ssao, newUv).r;
		}
	}

	sum *= 1.0 / 16.0;
	color = sum;
}