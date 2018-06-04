#shader vertex
#version 410

layout(location = 0) in vec4 Position;
layout(location = 2) in vec2 TexCoord;

out vec2 vTexCoord;
out vec4 vPosition;
out vec4 fragPos;

uniform mat4 ProjectionViewModel;

uniform mat4 ModelMatrix;

void main() {
	vPosition = ModelMatrix * Position;

	vTexCoord = TexCoord;
	fragPos = ProjectionViewModel * Position;
	gl_Position = ProjectionViewModel * Position;
}

#shader fragment
#version 410

in vec2 vTexCoord;
in vec4 vPosition;
in vec4 fragPos;

uniform vec3 cameraPosition;

// we need this matrix to transform the normal
uniform mat3 NormalMatrix;

uniform sampler2D diffuseTexture;
uniform sampler2D normalTexture;
uniform sampler2D specularTexture;

uniform float specularPower; // material specular power

struct DirLight {
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform DirLight dirLight;

struct PointLight {
	vec3 position;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform PointLight pointLight;

out vec4 FragColor;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main() {
	vec3 normalT = texture(normalTexture, vTexCoord).xyz;
	vec3 vNormal = NormalMatrix * normalT.xyz;

	// properties
	vec3 norm = normalize(vNormal);
	vec3 viewDir = normalize(cameraPosition - vPosition.xyz);

	// phase 1: Directional lighting
	vec3 result = CalcDirLight(dirLight, norm, viewDir);

	// point light
	result += CalcPointLight(pointLight, norm, fragPos.xyz, viewDir);

	FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);

	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);

	// specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), specularPower);

	// combine results
	vec3 ambient = light.ambient  * vec3(texture(diffuseTexture, vTexCoord));
	vec3 diffuse = light.diffuse  * diff * vec3(texture(diffuseTexture, vTexCoord));
	vec3 specular = light.specular * spec * vec3(texture(specularTexture, vTexCoord));
	return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);
	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);

	// specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), specularPower);

	// attenuation
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance +
		light.quadratic * (distance * distance));

	// combine results
	vec3 ambient = light.ambient  * vec3(texture(diffuseTexture, vTexCoord));
	vec3 diffuse = light.diffuse  * diff * vec3(texture(diffuseTexture, vTexCoord));
	vec3 specular = light.specular * spec * vec3(texture(specularTexture, vTexCoord));
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	return (ambient + diffuse + specular);
}