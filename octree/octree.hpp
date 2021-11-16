#include <iostream>
#include <glm/glm.hpp>
#include <vector>
#include "../chunk/block.hpp"
#include "material.hpp"
#define Byte unsigned char



struct OctNode {
	OctNode* children[8];
	bool leaf;
	glm::vec3 boundaryBoxMin;
	glm::vec3 boundaryBoxMax;
	bool filled;
	Block* voxel;
};

class Octree {
public:
	Octree(uint16_t depth, glm::vec3 midPos);	//Creating Tree
	void AddObject(void* object);
	
private:
	OctNode* m_root;
	uint16_t m_depth;
	void GenerateTree(OctNode* child, uint16_t depth, uint16_t diameter, glm::vec3 position);
	void CalculatePosition(Byte bit, uint16_t diameter, glm::vec3& position);
	void GenerateBoundaryBox(OctNode* node, uint16_t diameter, glm::vec3 position);
	void GenerateVoxel(OctNode* node);
};

