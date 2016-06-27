#version 330 core

layout (std140) uniform Matrices
{
	mat4 projection;	//4 * 16
	mat4 view;			//4 * 16
	vec3 cameraPos;		//16
	vec4 mainLight;		//16
	vec4 projParams;	//16
};

in vec3 v_normal;
in vec3 v_lightDir;
in vec3 worldPos;

out vec4 color;

uniform sampler2D texShadowMap;
uniform mat4 shadowVP;

float DecodeFloatRGBA( vec4 enc )
{
	vec4 kDecodeDot = vec4(1.0, 1/255.0, 1/65025.0, 1/16581375.0);
	return dot( enc, kDecodeDot );
}

void main() 
{
	vec3 lightDir = normalize(v_lightDir);
	vec3 normal = normalize(v_normal);

	float nl = clamp(dot(lightDir, normal), 0, 1);
	
	vec4 shadowP = shadowVP * vec4(worldPos, 1);
	vec2 shadowUV = shadowP.xy / shadowP.w * 0.5 + 0.5;

	float nowDepth = shadowP.w / 50.0;

	float depth = 1 - DecodeFloatRGBA(texture(texShadowMap, shadowUV));

	float atten = 1;
	if (depth + 0.002 < nowDepth)
		atten = 0;
	
	color = vec4(nl * atten);
}