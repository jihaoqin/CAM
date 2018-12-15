#version 430 core
in vec3 normal;
in vec3 worldObject;
out vec4 FragColor;

uniform vec3 viewPos;
struct Material{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};
uniform Material material;
struct Light{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Light light;

void main()
{
    //ambient
    vec3 ambient = material.ambient * light.ambient;

    //diffuse
	vec3 lightDir = normalize(light.position - worldObject);
	vec3 norm = normalize(normal);
	float c = max(dot(lightDir, norm), 0.0f);
    vec3 diffuse = material.diffuse * light.diffuse * c;
    
    //specular
	float specularStrength = 1.0f;
	vec3 viewDir = normalize(viewPos - worldObject);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(reflectDir, viewDir), 0.0f), material.shininess);
	vec3 specular = spec * specularStrength * material.specular * light.specular;

	FragColor = vec4(ambient + diffuse + specular , 1.0f);
}