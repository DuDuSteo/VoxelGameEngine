#include "../shader/shader.hpp"
#include "../material/material.hpp"
#include "../file_handler/file_handler.hpp"

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <iostream>
#include <cstring>
#include <vector>

#ifndef OBJECT_HPP
#define OBJECT_HPP

class Object
{
public:
  Object();
  void Draw(MVP mvp, glm::vec3 cameraPosition, Light light);
  void AddVoxel(glm::ivec3 pos, Material mat);
  void ChangeColor(Voxel *voxel, Material mat);
  void RemoveVoxel(Voxel *voxel);
  void RemoveVoxel(glm::vec3 pos);
  void Reset();
  void Save();
  void Load(std::string objectPath);
  Voxel *CheckRay(glm::vec3 ray_origin, glm::vec3 ray_dir, glm::vec3 &newBlockLoc);
  std::vector<Voxel> GetListOfVoxels();

  std::string name;

private:
  uint32_t m_VAO, m_VBO, m_EBO;
  Shader m_shader;
  std::vector<Vertex> m_vertices;
  std::vector<uint32_t> m_indices;
  std::vector<Voxel> m_voxels;
  bool m_hashVoxels[VOXEL_COUNT][VOXEL_COUNT][VOXEL_COUNT];
};

#endif