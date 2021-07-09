#include "chunk.h"

bool voxels[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];

void Chunk::bind() {

}

void Chunk::fill() {
	memset(voxels, true, sizeof(voxels));
	
	uint32_t offset = 0;
	int count = 0;
	for (size_t x = 0; x < CHUNK_SIZE; x++) {
		for (size_t y = 0; y < CHUNK_SIZE; y++) {
			for (size_t z = 0; z < CHUNK_SIZE; z++) {
				if(voxels[x][y][z]) {
					Block t_block;
					//t_block.createCube(glm::vec3(x, y, z), offset);
					t_block.update(getBaseFace(BlockFace::Top), glm::vec3(x, y, z), offset);
					t_block.update(getBaseFace(BlockFace::Bottom), glm::vec3(x, y, z), offset);
					t_block.update(getBaseFace(BlockFace::Left), glm::vec3(x, y, z), offset);
					t_block.update(getBaseFace(BlockFace::Right), glm::vec3(x, y, z), offset);
					t_block.update(getBaseFace(BlockFace::Front), glm::vec3(x, y, z), offset);
					t_block.update(getBaseFace(BlockFace::Back), glm::vec3(x, y, z), offset);

					m_vertices.insert(m_vertices.end(), t_block.m_vertices.begin(), t_block.m_vertices.end());
					m_indices.insert(m_indices.end(), t_block.m_indices.begin(), t_block.m_indices.end());
				}
			}
		}
	}
}
