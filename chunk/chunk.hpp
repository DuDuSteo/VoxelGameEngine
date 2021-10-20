#pragma once
#include <iostream>
#include <cstring>
#include "block.hpp"

#define CHUNK_SIZE 2

class Voxel {
public:
	glm::vec3 pos;
	bool visible;
private:
};

class Chunk {
public:
	void addBlock();
	void draw();
	void fill();
	void fill(std::vector<Voxel> voxels);

	std::vector<Vertex> m_vertices;
	std::vector<uint32_t> m_indices;
private:

};

