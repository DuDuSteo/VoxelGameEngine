#include "chunk.h"

void Chunk::bind() {

}

void Chunk::fill() {
	Block t_block;
	uint32_t offset = 0;
	for (size_t i = 0; i < CHUNK_SIZE; i++) {
		for (size_t j = 0; j < CHUNK_SIZE; j++) {
			for (size_t k = 0; k < CHUNK_SIZE; k++) {
				t_block.createCube(glm::vec3(i, j, k), offset);
				m_vertices.insert(m_vertices.end(), t_block.m_vertices.begin(), t_block.m_vertices.end());
				m_indices.insert(m_indices.end(), t_block.m_indices.begin(), t_block.m_indices.end());
			}
		}
	}
}
