#version 330 core

in vec2 v_uv;

out float color;

uniform vec3 samples[64];

const vec2 noiseScale = vec2(640.0 * 0.25, 640.0 * 0.25);
const float radius = 1.0;

uniform sampler2D tex_position;
uniform sampler2D tex_normal;
uniform sampler2D tex_noise;

layout (std140) uniform Matrices
{
	mat4 projection;	//4 * 16
	mat4 view;			//4 * 16
	vec3 cameraPos;		//16
	vec4 mainLight;		//16
	vec4 projParams;	//16
	mat4 invProjection;	//4 * 16
	mat4 invView;		//4 * 16
};

float DecodeFloatRGBA( vec4 enc )
{
	vec4 kDecodeDot = vec4(1.0, 1/255.0, 1/65025.0, 1/16581375.0);
	return dot( enc, kDecodeDot );
}

vec3 getViewPos(vec2 screenUv, float depth)
{
	vec2 newUv = screenUv * 2 - 1;
	float offset = projParams.y - projParams.x;
	float a = -projParams.z / offset;
	float b = (-2* projParams.w) / offset;

	float newZ = a * -depth + b;

	vec4 newP = vec4(newUv.x * depth, newUv.y * depth, newZ, depth);
	return (invProjection * newP).xyz;
}

float DotClamped(vec3 a, vec3 b)
{
	return max(0.0, dot(a, b));
}

float Pow5(float x)
{
	return x * x * x * x * x;
}

void main() 
{
	vec3 position = getViewPos(v_uv, (1 - DecodeFloatRGBA(texture(tex_position, v_uv))) * projParams.y);
	
	vec3 normal = normalize(texture(tex_normal, v_uv).rgb * 2 - 1);

	vec3 randomVec = texture(tex_noise, v_uv * noiseScale).xyz;

	vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
	vec3 bitangent = cross(normal, tangent);

	mat3 TBN = mat3(tangent, bitangent, normal);

	float occlusion = 0.0;

	for(int i = 0; i < 64; i++)
	{
		vec3 sample = TBN * samples[i];

		sample = position + sample * radius;

		vec4 offset = projection * vec4(sample, 1.0);
		offset.xyz /= offset.w;
		offset.xyz = offset.xyz * 0.5 + 0.5;

		float sampleDepth = -(1 - DecodeFloatRGBA(texture(tex_position, offset.xy))) * projParams.y;

		float rangeCheck = smoothstep(0.0, 1.0, radius / abs(position.z - sampleDepth));

		occlusion += (sampleDepth >= sample.z ? 1.0 : 0.0) * rangeCheck;
		//occlusion += (sampleDepth >= sample.z ? 1.0 : 0.0);
	}

	occlusion = 1.0 - (occlusion / 64);

	color = occlusion;
}