#pragma once

#include <glm/glm.hpp>
#include <fstream>
#include <iostream>

#define PATH "material/"

struct Material {
	std::string name;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;
};

void saveMaterial(Material mat, const std::string& matName);

void loadMaterial(Material& mat, const std::string& matName);