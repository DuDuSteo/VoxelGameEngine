#include "file_reader.h"

namespace File {
	std::vector<Voxel> read(const std::string& fileName)
	{
		std::cout << PATH + fileName + ".txt" << std::endl;
		std::ifstream file(PATH + fileName + ".txt");
		if (file.bad()) {
			std::cout << "VOXEL_READER::READ::FILE_BAD" << std::endl;
			return std::vector<Voxel>();
		}
		Voxel t_voxel;
		std::vector<Voxel> t_voxels;
		while (!file.eof()) {
			file >> t_voxel.pos.x;
			file >> t_voxel.pos.y;
			file >> t_voxel.pos.z;
			t_voxels.push_back(t_voxel);
		}
		file.close();
		return std::vector<Voxel>(t_voxels);
	}
}
