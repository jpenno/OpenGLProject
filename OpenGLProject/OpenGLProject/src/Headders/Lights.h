#pragma once
#include <glm/vec3.hpp>

struct Light {
	glm::vec3 diffuse;
	glm::vec3 specular;
	glm::vec3 ambientLight;
};

struct DirLight : public Light {
	glm::vec3 direction;
};

struct PointLight : public Light {
	glm::vec3 pos;
	float constant;
	float linear;
	float quadratic;
};
