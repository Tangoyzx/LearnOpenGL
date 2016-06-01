#version 330 core

in vec2 v_uv;
in vec3 v_normal;
in vec3 v_tangent;
in vec3 v_viewDir;
in vec3 v_lightDir;
 
out vec4 color;

uniform sampler2D tex_diffuse;
uniform sampler2D tex_spec;

uniform vec4 _ProjParams;

float LinearDepth(float depth)
{
	float near = _ProjParams.x;
	float far = _ProjParams.y;
	float z = depth * 2.0 - 1.0;
	return (2.0 * near) / (far + near - z * (far - near));
}

float DotClamped(vec3 a, vec3 b)
{
	return max(0.0, dot(a, b));
}

float Pow5(float x)
{
	return x * x * x * x * x;
}

float DisneyDiffuse(vec3 lightDir, vec3 viewDir, vec3 normal)
{
	float nl = max(0, dot(lightDir, normal));
	float oneMinusCosL = 1.0f - nl;
	float nlPow5 = oneMinusCosL * oneMinusCosL * oneMinusCosL * oneMinusCosL * oneMinusCosL;

	float nv = max(0, dot(viewDir, normal));
	float oneMinusCosV = 1.0f - nv;
	float nvPow5 = oneMinusCosV * oneMinusCosV * oneMinusCosV * oneMinusCosV * oneMinusCosV;

	vec3 half = normalize(lightDir + viewDir);
	float lh = max(0, dot(lightDir, half));
	float F_D90 = 0.5f + 2.0f * lh * lh * 1;

	return (1 + (F_D90 - 1) * nlPow5) * (1 + (F_D90 - 1) * nvPow5) * nl;
}
 
void main()
{
	vec3 normal = normalize(v_normal); 
	vec3 lightDir = normalize(v_lightDir);
	vec3 viewDir = normalize(v_viewDir);

	vec3 half = normalize(lightDir + viewDir);

	float nl = DotClamped(normal, lightDir);
	float nv = DotClamped(normal, viewDir);

	float lh = DotClamped(lightDir, half);

	float nlPow5 = Pow5(1 - nl);
	float nvPow5 = Pow5(1 - nv);
	float Fd90 = 0.5 + 2.0 * lh * lh * 0.2;
	float disneyDiffuse = (1 + (Fd90 - 1) * nlPow5) * (1 + (Fd90 - 1) * nvPow5);

	float diffuseTerm = disneyDiffuse * nl;

	color = vec4(diffuseTerm, diffuseTerm, diffuseTerm, 1);
}