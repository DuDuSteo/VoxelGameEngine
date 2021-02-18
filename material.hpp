#pragma once

#include <glm/glm.hpp>
#include <fstream>

#define PATH "material/"

namespace material {
	struct Material {
		std::string name;
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
		float shininess;
	};

	Material EMERALD = {
		"emerald",
		{0.0215, 0.1745, 0.0215},
		{0.07568, 0.61424, 0.07568},
		{0.633,	0.727811, 0.633},
		0.6
	};

	Material JADE = {
		"jade",
		{0.135, 0.2225, 0.1575},
		{0.54, 0.89, 0.63},
		{0.316228, 0.316228, 0.316228},
		0.1
	};

	Material OBSIDIAN = {
		"obsidian",
		{0.05375, 0.05, 0.06625},
		{0.18275, 0.17, 0.22525},
		{0.332741, 0.328634, 0.346435},
		0.3
	};

	Material PEARL = {
		"pearl",
		{0.25, 0.20725, 0.20725},
		{1, 0.829, 0.829},
		{0.296648, 0.296648, 0.296648},
		0.088
	};

	Material RUBY = {
		"ruby",
		{0.1745, 0.01175, 0.01175},
		{0.61424, 0.04136, 0.04136},
		{0.727811, 0.626959, 0.626959},
		0.6
	};

	Material TURQUOISE = {
		"turquoise",
		{0.1, 0.18725, 0.1745},
		{0.396, 0.74151, 0.69102},
		{0.297254, 0.30829, 0.306678},
		0.1
	};

	Material BRASS = {
		"brass",
		{0.329412, 0.223529, 0.027451},
		{0.780392, 0.568627, 0.113725},
		{0.992157, 0.941176, 0.807843},
		0.21794872
	};

	Material BRONZE = {
		"bronze",
		{0.2125, 0.1275, 0.054},
		{0.714, 0.4284, 0.18144},
		{0.393548, 0.271906, 0.166721},
		0.2
	};

	//Material MATERIAL_ = {
	//	"",
	//	{},
	//	{},
	//	{},
	//	
	//};

	void SaveMaterial(Material mat, const std::string& materialName) {
		std::ofstream file(PATH + materialName + ".mat");
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

	void LoadMaterial(Material& mat, const std::string& materialName) {
		std::cout << PATH + materialName + ".mat" << std::endl;
		std::ifstream file(PATH + materialName + ".mat");
		if (file.bad()){
			std::cout << "MATERIAL::LOAD_MATERIAL::FILE_BAD" << std::endl;
			return;
		}
		file >> mat.name;
		for (size_t i = 0; i < 3; i++)
			file >> mat.ambient[i];
		for (size_t i = 0; i < 3; i++)
			file >> mat.diffuse[i];
		for (size_t i = 0; i < 3; i++)
			file >> mat.specular[i];
		file >> mat.shininess;
		file.close();
	}
}
