#pragma once

#include "octree.h"

class VoxelObject {
public:
	VoxelObject();
	void Draw();
private:
	Octree* root;
};

// voxel object is a octree that is being organized by scene_graph structure