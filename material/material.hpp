#include <glm/glm.hpp>
#include <fstream>
#include <iostream>

#ifndef MATERIAL_HPP
#define MATERIAL_HPP

struct Material {
	std::string name;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;
};

void saveMaterial(Material mat, const std::string& matName);

Material loadMaterial(const std::string& matName);

#endif