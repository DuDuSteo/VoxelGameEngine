#include "../shader/shader.hpp"
#include "../material/material.hpp"
#include "../file_handler/file_handler.hpp"

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <iostream>
#include <cstring>
#include <vector>

#define VOXEL_COUNT 255
#define MAX_RAY_RANGE 100.f

#ifndef OBJECT_HPP
#define OBJECT_HPP

class Object {
public:
  Object();
  void draw(MVP mvp, glm::vec3 cameraPosition, Light light);
  void addVoxel(glm::ivec3 pos, Material mat);
  void checkRay(glm::vec3 ray_origin, glm::vec3 ray_dir);
  std::vector<Voxel> getListOfVoxels();
private:
uint32_t m_VAO, m_VBO, m_EBO;
  Shader m_shader;
  std::vector<Vertex> m_vertices;
  std::vector<uint32_t> m_indices;
  std::vector<Voxel> m_voxels;
  bool m_hash_voxels [VOXEL_COUNT][VOXEL_COUNT][VOXEL_COUNT];
};

#endif