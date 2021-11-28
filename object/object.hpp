#include "../shader/shader.hpp"
#include "../material/material.hpp"
#include <glm/glm.hpp>
#include <iostream>
#include <cstring>
#include <vector>

#define VOXEL_COUNT 255

#ifndef OBJECT_HPP
#define OBJECT_HPP

struct Voxel {
  glm::vec3 pos;
  Material mat;
};

class Object {
public:
  Object();
  void draw(glm::mat4 model, Shader shader);
  void addVoxel(glm::ivec3 pos, Material mat);
  std::vector<Voxel> getListOfVoxels();
private:
  std::vector<Voxel> m_voxels;
  bool m_hash_voxels [VOXEL_COUNT][VOXEL_COUNT][VOXEL_COUNT];
};

#endif