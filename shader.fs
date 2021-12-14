#version 460 core

struct Material
{
	sampler2D texture_diffuse1;
	sampler2D texture_specular1;
	sampler2D texture_normal1;
	sampler2D texture_height1;
	float shininess;
};

struct PointingLight
{
	vec3 position;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

struct DirectionLight
{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};


in vec3 Normal;
in vec2 TexCoord;
in vec3 ObjectPosition;

out vec4 FragColor;

uniform Material material;
uniform DirectionLight light;
uniform vec3 viewPosition;


void main()
{
	vec3 normal = normalize(Normal);
	vec3 diffuseTex = texture(material.texture_diffuse1, TexCoord).rgb;
	vec3 specularTex = texture(material.texture_specular1, TexCoord).rgb;

//	vec3 lightDirection = normalize(light.position - ObjectPosition);
	vec3 lightDirection = normalize(-light.direction);

	vec3 ambient = light.ambient * diffuseTex;

	float diff = max(dot(lightDirection, normal), 0.0f);
	vec3 diffuse = light.diffuse * diff * diffuseTex;

	vec3 reflectDirection = normalize(reflect(-lightDirection, normal));
	float spec = pow(max(dot(reflectDirection, viewPosition), 0.0f), material.shininess);
	vec3 specular = light.specular * spec * specularTex;

//	float distances = length(light.position - ObjectPosition);
//	float attenuation = 1.0f / (light.constant + light.linear * distances + light.quadratic * pow(distances, 2));
//
//	diffuse *= attenuation;
//	specular *= attenuation;

	vec3 result = ambient + diffuse + specular;
	
//	FragColor = vec4(texture(material.texture_diffuse1, TexCoord));
	FragColor = vec4(result, 1.0f);
}