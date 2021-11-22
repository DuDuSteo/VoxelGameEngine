#include <iostream>
#include <vector>
#include "glm/glm.hpp"

#include "../items/items.hpp"

#define Byte unsigned char

#ifndef BLOCK_HPP
#define BLOCK_HPP

class Block {
public:
	void createCube(const glm::vec3 position, uint32_t &offset);
	void update(const Block& base, glm::vec3 position, uint32_t& offset);

	std::vector<Vertex> m_vertices;
	std::vector<uint32_t> m_indices;
private:
};

Block getBaseFace(BlockFace face);

#endif