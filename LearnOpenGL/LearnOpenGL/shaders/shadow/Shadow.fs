#version 330 core

in float v_viewDepth;

out vec4 color;

vec4 EncodeFloatRGBA( float v )
{
	vec4 kEncodeMul = vec4(1.0, 255.0, 65025.0, 16581375.0);
	float kEncodeBit = 1.0/255.0;
	vec4 enc = kEncodeMul * v;
	enc = fract(enc);
	enc -= enc.yzww * kEncodeBit;
	return enc;
}


void main() 
{
	color = EncodeFloatRGBA(v_viewDepth);
	//color = vec4(1, 0, 0, 1);
}