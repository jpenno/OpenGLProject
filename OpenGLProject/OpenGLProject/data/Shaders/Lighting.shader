#shader vertex
#version 410

layout(location = 0) in vec4 Position;
layout(location = 2) in vec2 TexCoord;

out vec2 vTexCoord;
out vec4 vPosition;

uniform mat4 ProjectionViewModel;

uniform mat4 ModelMatrix;

void main() {
	vPosition = ModelMatrix * Position;

	vTexCoord = TexCoord;
	gl_Position = ProjectionViewModel * Position;
}

#shader fragment
#version 410

in vec2 vTexCoord;
in vec4 vPosition;

//in vec3 vNormal;
uniform vec3 cameraPosition;

vec3 Ka; // ambient material colour
vec3 Kd; // diffuse material colour
vec3 Ks; // specular material colour
uniform float specularPower; // material specular power

// we need this matrix to transform the normal
uniform mat3 NormalMatrix;

uniform vec3 Ia; // ambient light colour
uniform vec3 Id; // diffuse light colour
uniform vec3 Is; // specular light colour
uniform vec3 LightDirection;

uniform sampler2D diffuseTexture;
uniform sampler2D normalTexture;
uniform sampler2D specularTexture;

out vec4 FragColour;

void main() {
	vec3 texColor = texture(diffuseTexture, vTexCoord).xyz;
	Kd = texColor;
	Ka = texColor;

	vec3 specColor = texture(specularTexture, vTexCoord).xyz;
	Ks = specColor;

	vec3 normalT = texture(normalTexture, vTexCoord).xyz;
	vec3 vNormal = NormalMatrix * normalT.xyz;

	// ensure normal and light direction are normalised
	vec3 N = normalize(vNormal);
	vec3 L = normalize(LightDirection);

	// calculate lambert term (negate light direction)
	float lambertTerm = max(0, min(1, dot(N, -L)));

	// calculate view vector and reflection vector
	vec3 V = normalize(cameraPosition - vPosition.xyz);
	vec3 R = reflect(L, N);

	// calculate specular term
	float specularTerm = pow(max(0, dot(R, V)), specularPower);

	vec3 diffuse = Id * Kd * lambertTerm;
	vec3 ambient = Ia * Ka;

	vec3 specular = Is * Ks * specularTerm;

	FragColour = vec4(ambient + diffuse + specular, 1);
}
