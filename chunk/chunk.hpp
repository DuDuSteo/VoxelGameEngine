#include <iostream>
#include <cstring>
#include "block.hpp"

#define CHUNK_SIZE 1

class Voxel {
public:
	glm::vec3 pos;
	bool visible;
private:
};

class Chunk {
public:
	void addBlock(glm::vec3 position);
	void draw();
	void fill();

	std::vector<Vertex> m_vertices;
	std::vector<uint32_t> m_indices;
private:

};

