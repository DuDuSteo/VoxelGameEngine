#include "voxel_object.h"

VoxelObject::VoxelObject() {
	root = new Octree(2, glm::vec3(0.f, 0.f, 0.f));
}

void VoxelObject::Draw() {

}
