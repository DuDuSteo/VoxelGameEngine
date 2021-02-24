#include "octree.h"


Octree::Octree(uint16_t depth, glm::vec3 midPos) {
	m_root = new OctNode();
	m_depth = depth; 

	uint16_t diameter = (uint16_t)pow(2, depth);
	glm::vec3 t_pos(midPos.x - diameter/2, midPos.y - diameter / 2, midPos.z - diameter / 2);
	GenerateBoundaryBox(m_root, diameter, t_pos);
	GenerateTree(m_root, depth, diameter, t_pos);
}

void Octree::GenerateTree(OctNode* child, uint16_t depth, uint16_t diameter, glm::vec3 position) {
	if (depth) {
		child->leaf = false;
		depth--;
		diameter = diameter / 2;
		for (Byte i = 0; i < 8; i++) {
			child->children[i] = new OctNode();
			glm::vec3 t_pos = position;
			CalculatePosition(i, diameter, t_pos);
			GenerateBoundaryBox(child->children[i], diameter, t_pos);
			GenerateTree(child->children[i], depth, diameter, t_pos);
		}
	}
	else
		child->leaf = true;

}

void Octree::CalculatePosition(Byte bit, uint16_t diameter, glm::vec3& position) {
	if (bit % 2 == 1)	// z == true
		position.z += diameter;
	bit = bit >> 1;
	if (bit % 2 == 1)	// y == true
		position.y += diameter;
	bit = bit >> 1;
	if (bit % 2 == 1)	// x == true
		position.x += diameter;
}

void Octree::GenerateBoundaryBox(OctNode* node, uint16_t diameter, glm::vec3 position) {
	node->boundaryBoxMin = position;
	node->boundaryBoxMax = position + glm::vec3(diameter);
	std::cout << node->boundaryBoxMin.x << " " << node->boundaryBoxMax.x << std::endl;
}
