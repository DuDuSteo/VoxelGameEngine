#pragma once
#include <iostream>
#include "block.h"
#include "voxel.h"

#define CHUNK_SIZE 2

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

