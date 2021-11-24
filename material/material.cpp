#include "material.hpp"

void saveMaterial(Material mat, const std::string& matName) {
	std::ofstream file(matName + ".mat");
	if (file.bad()) {
		std::cout << "MATERIAL::SAVE_MATERIAL::FILE_BAD" << std::endl;
		return;
	}
	file << mat.name << std::endl;
	file << mat.ambient[0] << " " << mat.ambient[1] << " " << mat.ambient[2] << std::endl;
	file << mat.diffuse[0] << " " << mat.diffuse[1] << " " << mat.diffuse[2] << std::endl;
	file << mat.specular[0] << " " << mat.specular[1] << " " << mat.specular[2] << std::endl;
	file << mat.shininess;
	file.close();
}

void loadMaterial(Material& mat, const std::string& matName) {
	std::cout << matName << std::endl;
	std::ifstream file(matName);
	if (file.bad()) {
		std::cout << "MATERIAL::LOAD_MATERIAL::FILE_BAD" << std::endl;
		return;
	}
	file >> mat.name;
	for (int i = 0; i < 3; i++)
		file >> mat.ambient[i];
	for (int i = 0; i < 3; i++)
		file >> mat.diffuse[i];
	for (int i = 0; i < 3; i++)
		file >> mat.specular[i];
	file >> mat.shininess;
	file.close();
}
