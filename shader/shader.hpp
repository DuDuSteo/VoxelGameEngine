#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "../items/items.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

#ifndef SHADER_HPP
#define SHADER_HPP

class Shader
{
public:
  void Init(const std::string &vertFileName, const std::string &fragFileName);

  void Use();
  void SetMat4(const std::string &name, const glm::mat4 &mat) const;
  void SetVec3(const std::string &name, const glm::vec3 &vec) const;
  void SetVec4(const std::string &name, const glm::vec4 &vec) const;
  void SetFloat(const std::string &name, const float &value) const;

private:
  uint32_t shaderID;

  void checkCompileErrors(uint32_t shader, std::string type);
};

#endif