#version 330 core

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in vec2 v_uv;
in vec3 v_normal;
in vec3 v_pos;

uniform sampler2D tex_diffuse;
uniform sampler2D tex_spec;

void main()
{
	gPosition = v_pos * 0.05 + 0.5;
	gNormal = normalize(v_normal) * 0.5 + 0.5;
	gAlbedoSpec.rgb = texture(tex_diffuse, v_uv).rgb;
	gAlbedoSpec.a = texture(tex_spec, v_uv).r;
}