#version 330 core

in vec2 v_uv;
in vec3 v_tangent_viewDir;
in vec3 v_tangent_lightDir;

out vec4 FragColor;

uniform sampler2D parallax_map;
uniform sampler2D normal_map;
uniform sampler2D diffuse_map;


vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir)
{ 
	const float height_scale = 0.1;

	const float minLayers = 8;
	const float maxLayers = 32;
	float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));
	
	float layerDepth = 1.0 / numLayers;
	float currentLayerDepth = 0.0;

	vec2 P = viewDir.xy * height_scale;
	vec2 deltaTexCoords = P / numLayers;

	vec2 currentTexCoords = texCoords;
	float currentDepthMapValue = texture(parallax_map, currentTexCoords).r;

	while (currentLayerDepth < currentDepthMapValue)
	{
		currentTexCoords -= deltaTexCoords;
		currentDepthMapValue = texture(parallax_map, currentTexCoords).r;
		currentLayerDepth += layerDepth;
	}
	return currentTexCoords;
}

void main()
{
	vec3 viewDir = normalize(v_tangent_viewDir);
	vec3 lightDir = normalize(v_tangent_lightDir);

	vec2 texCoords = ParallaxMapping(v_uv,  viewDir);

	vec3 normal = texture(normal_map, texCoords).rgb;
	normal = normalize(normal * 2.0 - 1.0);

	vec3 color = texture(diffuse_map, texCoords).rgb;

	vec3 ambient = 0.1 * color;

	float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = diff * color;

	vec3 reflectDir = reflect(-lightDir, normal);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

	vec3 specular = vec3(0.2) * spec;
	FragColor = vec4(ambient + diffuse + specular, 1.0f);
}