#pragma once
#include <iostream>
#include <glm/glm.hpp>
#define Byte unsigned char



struct OctNode {
	OctNode* children[8];
	bool leaf;
	glm::vec3 boundaryBoxMin;
	glm::vec3 boundaryBoxMax;
	//void* data;
};

class Octree  {
public:
	Octree(uint16_t depth, glm::vec3 startPos);	//Creating Tree
private:
	OctNode* m_root;
	void GenerateTree(OctNode* child, uint16_t depth, uint16_t diameter, glm::vec3 position);
	void GenerateBoundaryBox(OctNode* node, Byte bit, uint16_t diameter, glm::vec3 position);
};