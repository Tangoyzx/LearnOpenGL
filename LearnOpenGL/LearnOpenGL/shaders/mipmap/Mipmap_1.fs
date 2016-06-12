#version 330 core

#extension GL_ARB_texture_query_lod : enable


in vec2 v_uv;
in vec3 v_normal;
in vec3 v_tangent;
in vec3 v_viewDir;
in vec3 v_lightDir;

uniform sampler2D tex_diffuse;

 
out vec4 color;

void main() 
{
	float miplevel = textureQueryLOD(tex_diffuse, v_uv).x;
	color = textureLod(tex_diffuse, v_uv, miplevel);
}