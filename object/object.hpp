#include "../shader/shader.hpp"
#include <glm/glm.hpp>
#include <iostream>

#define VOXEL_COUNT 256

#ifndef OBJECT_HPP
#define OBJECT_HPP

class Object {
public:
  void draw(glm::mat4 model, Shader shader);

private:
  uint8_t m_voxels[VOXEL_COUNT][VOXEL_COUNT][VOXEL_COUNT];
};

#endif