#include <glm/glm.hpp>
#include <iostream>

#ifndef ITEMS_H
#define ITEMS_H

struct Vertex {
  glm::vec3 pos;
  glm::vec3 normals;
};

struct Material {
	std::string name;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;
};

struct Voxel {
  glm::vec3 pos;
  Material mat;
};

struct MVP {
  glm::mat4 model;
  glm::mat4 view;
  glm::mat4 projection;
};

enum class BlockFace {
	Top, Bottom, Front, Back, Right, Left
};

struct Light {
  glm::vec3 direction;
  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;
};

#endif