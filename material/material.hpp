#include "../items/items.hpp"
#include <glm/glm.hpp>
#include <fstream>
#include <iostream>

#ifndef MATERIAL_HPP
#define MATERIAL_HPP

void saveMaterial(Material mat, const std::string& matName);

Material loadMaterial(const std::string& matName);

#endif