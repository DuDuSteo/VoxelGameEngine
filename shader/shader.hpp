#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "../items/items.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

#ifndef SHADER_HPP
#define SHADER_HPP

class Shader {
public:
  void init(const std::string &vertFileName, const std::string &fragFileName);

  void use();
  void setMat4(const std::string &name, const glm::mat4 &mat) const;
  void setVec3(const std::string &name, const glm::vec3 &vec) const;
  void setVec4(const std::string &name, const glm::vec4 &vec) const;
  void setFloat(const std::string &name, const float &value) const;

private:
  uint32_t shaderID;

  void checkCompileErrors(uint32_t shader, std::string type);
};

#endif