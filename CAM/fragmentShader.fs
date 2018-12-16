#version 430 core

struct Material{
	sampler2D diffuse;
    sampler2D specular;
    float shininess;
};
struct PointLight{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
struct LineLight{
	vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
struct SpotLight{
	vec3 position;
	vec3 direction;
	float radian;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
in vec3 normal;
in vec3 worldObject;
in vec2 texCoor;
out vec4 FragColor;

uniform PointLight pointLight;
uniform vec3 viewPos;
uniform Material material;

vec4 calcPointLight(PointLight);
void main()
{
	FragColor = vec4(1.0f);
	vec4 pointColor = calcPointLight(pointLight);
	FragColor *= pointColor;
}

vec4 calcPointLight(PointLight light){
	//ambient
    vec3 ambient = vec3(texture(material.diffuse, texCoor)) * light.ambient;

    //diffuse
	vec3 lightDir = normalize(light.position - worldObject);
	vec3 norm = normalize(normal);
	float c = max(dot(lightDir, norm), 0.0f);
    vec3 diffuse = vec3(texture(material.diffuse, texCoor)) * light.diffuse * c;

    //specular
	vec3 viewDir = normalize(viewPos - worldObject);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(reflectDir, viewDir), 0.0f), material.shininess);
	vec3 specular = spec * vec3(texture(material.specular, texCoor)) * light.specular;
	vec3 result =ambient + diffuse + specular; 
	return vec4( result, 1.0f);
}