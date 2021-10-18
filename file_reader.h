#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include "voxel.h"

#define PATH "file/"

namespace File {
	std::vector<Voxel> read(const std::string& fileName);
}

