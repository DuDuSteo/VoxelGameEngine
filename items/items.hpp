#include <glm/glm.hpp>
#include <iostream>

#ifndef ITEMS_H
#define ITEMS_H

#define MATERIALS_LIST_FILE_NAME "material_list"

#define FILES_PATH "files/"
#define GLSL_FRAGMENT_FILE_EXTENSION ".frag"
#define GLSL_VERTEX_FILE_EXTENSION ".vert"
#define MATERIAL_FILE_EXTENSION ".mat"
#define CONFIG_FILE_EXTENSION ".config"
#define SCR_WIDTH 1280
#define SCR_HEIGHT 720
#define APPLICATION_NAME "Voxel Editor"
#define FRAME_TIME_SIZE 60 * 20

#define VOXEL_COUNT 255
#define MAX_RAY_RANGE 100.f

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