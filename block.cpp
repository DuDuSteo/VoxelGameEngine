#include "block.h"

const glm::vec3 axes[] = {
	glm::vec3(-1.f, 0.f, 0.f),	// Left
	glm::vec3(1.f, 0.f, 0.f),	// Right
	glm::vec3(0.f, -1.f, 0.f),	// Back
	glm::vec3(0.f, 1.f, 0.f),	// Front
	glm::vec3(0.f, 0.f, -1.f),	// Down
	glm::vec3(0.f, 0.f, 1.f)	// Up
};

void Block::createCube(const glm::vec3 position, uint32_t &offset) {
	m_vertices.clear();
	m_indices.clear();
	// 6 faces in order: x- x+ y- y+ z- z+ 
	for (Byte i = 0; i < 8; i++) {
		Vertex t_vertex{ {position}, {glm::vec3(0.f)} };
		Byte bit = i;
		if (bit % 2 == 1)	// z == true
			t_vertex.pos.z++;
		bit = bit >> 1;
		if (bit % 2 == 1)	// y == true
			t_vertex.pos.y++;
		bit = bit >> 1;
		if (bit % 2 == 1)	// x == true
			t_vertex.pos.x++;
		if (i < 4)
			t_vertex.normals.x--;
		else
			t_vertex.normals.x++;
		m_vertices.push_back(t_vertex);
	}

	for (Byte i = 0; i < 8; i++) {
		Vertex t_vertex{ {position}, {glm::vec3(0.f)} };
		Byte bit = i;
		if (bit % 2 == 1)	// y == true
			t_vertex.pos.y++;
		bit = bit >> 1;
		if (bit % 2 == 1)	// x == true
			t_vertex.pos.x++;
		bit = bit >> 1;
		if (bit % 2 == 1)	// z == true
			t_vertex.pos.z++;
		if (i < 4)
			t_vertex.normals.y--;
		else
			t_vertex.normals.y++;
		m_vertices.push_back(t_vertex);
	}

	for (Byte i = 0; i < 8; i++) {
		Vertex t_vertex{ {position}, {glm::vec3(0.f)} };
		Byte bit = i;
		if (bit % 2 == 1)	// x == true
			t_vertex.pos.x++;
		bit = bit >> 1;
		if (bit % 2 == 1)	// z == true
			t_vertex.pos.z++;
		bit = bit >> 1;
		if (bit % 2 == 1)	// y == true
			t_vertex.pos.y++;
		if (i < 4)
			t_vertex.normals.z--;
		else
			t_vertex.normals.z++;
		m_vertices.push_back(t_vertex);
	}
	
	for (Byte itr = 0; itr < 6; itr++) {
		m_indices.push_back(offset);
		m_indices.push_back(offset + 2);
		m_indices.push_back(offset + 3);
		m_indices.push_back(offset + 3);
		m_indices.push_back(offset + 1);
		m_indices.push_back(offset);
		offset = offset + 4;
	}
}
