#version 330 core

in vec2 v_uv;
in vec3 v_cameraPos;

out vec4 color;

struct Light {
	vec4 Position;
	vec3 Color;

	float isLinear;
};

const int LIGHT_NUM = 6;
uniform Light lights[LIGHT_NUM];

uniform sampler2D tex_position;
uniform sampler2D tex_normal;
uniform sampler2D tex_albedoSpec;

layout (std140) uniform Matrices
{
	mat4 projection;	//4 * 16
	mat4 view;			//4 * 16
	vec3 cameraPos;		//16
	vec4 mainLight;		//16
};


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
	vec3 position = (texture(tex_position, v_uv).rgb - 0.5) * 20;
	vec3 normal = normalize(texture(tex_normal, v_uv).rgb * 2 - 1);
	vec4 albedoSpec = texture(tex_albedoSpec, v_uv);

	vec3 viewDir = normalize(cameraPos - position);

	vec3 lighting = albedoSpec.rgb * 0.1;

	color = albedoSpec.rgba;

	for(int i = 0; i < LIGHT_NUM; i++)
	{
		// Diffuse
        vec3 lightDir = (lights[i].Position.xyz - position * lights[i].Position.w);
		float distance = length(lightDir);
		lightDir = normalize(lightDir);

		vec3 diffuse = max(dot(normal, lightDir), 0.0) * albedoSpec.rgb * lights[i].Color;
        
        vec3 halfwayDir = normalize(lightDir + viewDir);  
        float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
        vec3 specular = lights[i].Color * spec * albedoSpec.a;
        
		float distance_factor = lights[i].Position.w * (lights[i].isLinear * distance + (1 - lights[i].isLinear) * distance * distance);
        float attenuation = 1.0 / (1.0 + distance_factor);
		//float attenuation = 1.0;
        diffuse *= attenuation;
        specular *= attenuation;
        lighting += diffuse + specular;
	}
	color = vec4(lighting, 1);
}