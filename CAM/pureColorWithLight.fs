#version 430 core
struct Material{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
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
uniform LineLight lineLight;
uniform vec3 viewPos;
uniform Material material;

vec4 calcPointLight(PointLight);
vec4 calcLineLight(LineLight);

out vec4 FragColor;
in vec3 worldObject;
in vec3 normal;
void main(){
	vec4 lineColor = calcLineLight(lineLight);
	FragColor =  lineColor;
}

vec4 calcLineLight(LineLight light){
    //ambient
//    vec3 ambient = light.ambient * material.ambient;
	vec3 ambient = vec3(0.0f);
    //diffuse
	vec3 norm = normalize(normal);
    float diff = max(dot(light.direction*(-1.0f), norm), 0.0f);
    vec3 diffuse = material.diffuse * light.diffuse * diff;
	//diffuse = vec3(0.0f);
    //specular
	vec3 viewDir = normalize(viewPos - worldObject);
	vec3 reflectDir = reflect(light.direction, norm);
	float spec = pow(max(dot(reflectDir, viewDir), 0.0f), material.shininess);
	vec3 specular = spec * material.specular * light.specular;
	specular = vec3(0.0f);
	vec3 result =ambient + diffuse + specular; 
	return vec4( result, 1.0f);
}