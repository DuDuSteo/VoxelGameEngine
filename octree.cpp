#include "octree.h"


Octree::Octree(uint16_t depth, glm::vec3 startPos) {
	m_root = new OctNode();
	uint16_t diameter = (uint16_t)pow(2, depth);
	GenerateBoundaryBox(m_root, 0, diameter, startPos);
	GenerateTree(m_root, depth, diameter, startPos);
}

void Octree::GenerateTree(OctNode* child, uint16_t depth, uint16_t diameter, glm::vec3 position) {
	if (depth) {
		child->leaf = false;
		depth--;
		diameter = diameter / 2;
		for (Byte i = 0; i < 8; i++) {
			child->children[i] = new OctNode();
			
			GenerateBoundaryBox(child->children[i], i, diameter, position);
			GenerateTree(child->children[i], depth, diameter, position);
		}
	}
	else
		child->leaf = true;

}

void Octree::GenerateBoundaryBox(OctNode* node, Byte bit, uint16_t diameter, glm::vec3 position) {
	std::cout << (int)bit;
	if (bit % 2 == 1) {	// z == true
		node->boundaryBoxMin.z = position.z; 
		node->boundaryBoxMax.z = position.z + diameter;
	}
	else {	// z == false
		node->boundaryBoxMin.z = position.z - diameter; 
		node->boundaryBoxMax.z = position.z; 
	}		
	bit = bit >> 1;
	if (bit % 2 == 1) {	// y == true
		node->boundaryBoxMin.y = position.y;
		node->boundaryBoxMax.y = position.y + diameter;
	}
	else {	// y == false
		node->boundaryBoxMin.y = position.y - diameter;
		node->boundaryBoxMax.y = position.y;
	}
	bit = bit >> 1;
	if (bit % 2 == 1) {	// x == true
		node->boundaryBoxMin.x = position.x;
		node->boundaryBoxMax.x = position.x + diameter;
	}
	else {	// z == false
		node->boundaryBoxMin.x = position.x - diameter;
		node->boundaryBoxMax.x = position.x;
	}
	std::cout << " Max x: " << node->boundaryBoxMax.x <<
		" Max y: " << node->boundaryBoxMax.y <<
		" Max z: " << node->boundaryBoxMax.z << std::endl;
	std::cout << "-" << " Min x: " << node->boundaryBoxMin.x <<
		" Min y: " << node->boundaryBoxMin.y <<
		" Min z: " << node->boundaryBoxMin.z << std::endl << std::endl;
}
