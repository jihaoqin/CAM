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
uniform LineLight lineLight;
uniform vec3 viewPos;
uniform Material material;

vec4 calcPointLight(PointLight);
vec4 calcLineLight(LineLight);
void main()
{
	FragColor = vec4(1.0f);
	vec4 pointColor = calcPointLight(pointLight);
    vec4 lineColor = calcLineLight(lineLight);
	FragColor = FragColor * pointColor + FragColor * lineColor;
}

vec4 calcPointLight(PointLight light){
	//ambient
    vec3 ambient = vec3(texture(material.diffuse, texCoor)) * light.ambient;

    //diffuse
	vec3 lightDir = normalize(worldObject - light.position);
	vec3 norm = normalize(normal);
	float c = max(dot((-1.0f)*lightDir, norm), 0.0f);
    vec3 diffuse = vec3(texture(material.diffuse, texCoor)) * light.diffuse * c;

    //specular
	vec3 viewDir = normalize(viewPos - worldObject);
	vec3 reflectDir = reflect(lightDir, norm);
	float spec = pow(max(dot(reflectDir, viewDir), 0.0f), material.shininess);
	vec3 specular = spec * vec3(texture(material.specular, texCoor)) * light.specular;
	vec3 result =ambient + diffuse + specular; 
	return vec4( result, 1.0f);
}
vec4 calcLineLight(LineLight light){
    //ambient
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoor));
    //diffuse
	vec3 norm = normalize(normal);
    float diff = max(dot(light.direction*(-1.0f), norm), 0.0f);
    vec3 diffuse = vec3(texture(material.diffuse, texCoor)) * light.diffuse * diff;
    //specular
	vec3 viewDir = normalize(viewPos - worldObject);
	vec3 reflectDir = reflect(light.direction, norm);
	float spec = pow(max(dot(reflectDir, viewDir), 0.0f), material.shininess);
	vec3 specular = spec * vec3(texture(material.specular, texCoor)) * light.specular;
	vec3 result =ambient + diffuse + specular; 
	return vec4( result, 1.0f);
}