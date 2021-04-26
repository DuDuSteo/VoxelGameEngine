#pragma once
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#define Byte unsigned char

struct Vertex {
	glm::vec3 pos;
	glm::vec3 normals;
};

class Block {
public:
	void createCube(const glm::vec3 position, uint32_t &offset);

	std::vector<Vertex> m_vertices;
	std::vector<uint32_t> m_indices;
private:
};

