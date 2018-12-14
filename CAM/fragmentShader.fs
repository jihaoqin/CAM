#version 430 core
in vec3 normal;
in vec3 worldObject;
out vec4 FragColor;

uniform vec3 ambientColor;
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
	vec3 lightDir = normalize(lightPos - worldObject);
	vec3 norm = normalize(normal);
	float c = max(dot(lightDir, norm), 0.0f);
	vec3 diffuse = lightColor * c;
	float specularStrength = 1f;
	vec3 viewDir = normalize(viewPos - worldObject);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(reflectDir, viewDir), 0.0f), 32);
	vec3 specular = spec * specularStrength * lightColor;
	FragColor = vec4((ambientColor * 0.2 + diffuse + specular) * objectColor, 1.0f);
}