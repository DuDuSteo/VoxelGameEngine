#include "glm/glm.hpp"

#ifndef ITEMS_H
#define ITEMS_H

struct Vertex {
  glm::vec3 pos;
  glm::vec3 normals;
};

enum class BlockFace {
	Top, Bottom, Front, Back, Right, Left
};

#endif