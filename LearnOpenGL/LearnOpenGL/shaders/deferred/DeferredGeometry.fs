#version 330 core

layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in vec2 v_uv;
in vec3 v_normal;
in vec3 v_pos;

uniform sampler2D tex_diffuse;
uniform sampler2D tex_spec;

layout (std140) uniform Matrices
{
	mat4 projection;	//4 * 16
	mat4 view;			//4 * 16
	vec3 cameraPos;		//16
	vec4 mainLight;		//16
	vec4 projParams;	//16
};

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
	//gPosition = v_pos * 0.05 + 0.5;
	gPosition = EncodeFloatRGBA(1 + v_pos.z / projParams.y);
	//gPosition = vec4(1, 1, 0, 1);
	gNormal = normalize(v_normal) * 0.5 + 0.5;
	gAlbedoSpec.rgb = texture(tex_diffuse, v_uv).rgb;
	gAlbedoSpec.a = texture(tex_spec, v_uv).r;
}