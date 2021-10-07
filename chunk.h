#pragma once
#include <iostream>
#include "block.h"

#define CHUNK_SIZE 4



class Chunk {
public:
	void addBlock();
	void bind();
	void draw();
	void fill();

	std::vector<Vertex> m_vertices;
	std::vector<uint32_t> m_indices;
private:

};

