#include "../shader/shader.hpp"
#include "../material/material.hpp"
#include <glm/glm.hpp>
#include <iostream>
#include <cstring>
#include <vector>

#define VOXEL_COUNT 255

#ifndef OBJECT_HPP
#define OBJECT_HPP

class Object {
public:
  Object();
  void draw(glm::mat4 model, Shader shader);
  void addVoxel(glm::ivec3 pos, Material mat);
  void checkRay(glm::vec3 ray_start, glm::vec3 ray_dir);
  std::vector<Voxel> getListOfVoxels();
private:
  std::vector<Voxel> m_voxels;
  bool m_hash_voxels [VOXEL_COUNT][VOXEL_COUNT][VOXEL_COUNT];
};

#endif