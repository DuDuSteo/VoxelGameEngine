#include <cstdint>
#include <glm/glm.hpp>
#include <fstream>
#include <iostream>
#include <vector>

#include "../items/items.hpp"

void saveBuffers(std::vector<Vertex> vert, std::vector<uint32_t> ind);

void loadVertexBuffer(std::vector<Vertex> &vert);

void loadIndexBuffer(std::vector<uint32_t> &ind);