#include "material.hpp"

void saveMaterial(Material mat, const std::string &matName, bool edit)
{
	std::cout << "MATERIAL::SAVE_MATERIAL ";
	std::string matPath = std::string(FILES_PATH) + matName + MATERIAL_FILE_EXTENSION;
	std::cout << matPath << " ";
	std::ofstream file(matPath);
	if (file.bad() || file.fail())
	{
		std::cout << "FILE_BAD" << std::endl;
		return;
	}
	file << mat.name << std::endl;
	file << mat.ambient[0] << " " << mat.ambient[1] << " " << mat.ambient[2] << std::endl;
	file << mat.diffuse[0] << " " << mat.diffuse[1] << " " << mat.diffuse[2] << std::endl;
	file << mat.specular[0] << " " << mat.specular[1] << " " << mat.specular[2] << std::endl;
	file << mat.shininess;
	file.close();
	if (edit)
	{
		std::cout << "EDIT_MODE ";
	}
	else
	{
		std::cout << "ADD_MODE ";
		matPath = std::string(FILES_PATH) + MATERIALS_LIST_FILE_NAME + CONFIG_FILE_EXTENSION;
		std::cout << matPath << " ";
		file.open(matPath, std::ios_base::app);
		if (file.bad() || file.fail())
		{
			std::cout << "FILE_BAD" << std::endl;
			return;
		}
		file << std::endl
			 << mat.name;
		file.close();
	}
	std::cout << std::endl;
}

void removeMaterial(const std::string &matName)
{
	std::string matPath = std::string(FILES_PATH) + matName + MATERIAL_FILE_EXTENSION;
	std::cout << "MATERIAL::REMOVE_MATERIAL ";
	std::cout << matPath << " ";
	if (remove(matPath.c_str()))
	{
		std::cout << "REMOVE_FAILED" << std::endl;
		return;
	}
	std::cout << std::endl;
}

Material loadMaterial(const std::string &matName)
{
	std::cout << "MATERIAL::LOAD_MATERIAL ";
	std::cout << std::string(FILES_PATH) + matName + MATERIAL_FILE_EXTENSION << " ";
	std::ifstream file(std::string(FILES_PATH) + matName + MATERIAL_FILE_EXTENSION);
	if (file.bad() || file.fail())
	{
		std::cout << "FILE_BAD" << std::endl;
	}
	Material t_mat;
	file >> t_mat.name;
	for (int i = 0; i < 3; i++)
		file >> t_mat.ambient[i];
	for (int i = 0; i < 3; i++)
		file >> t_mat.diffuse[i];
	for (int i = 0; i < 3; i++)
		file >> t_mat.specular[i];
	file >> t_mat.shininess;
	file.close();
	std::cout << std::endl;
	return t_mat;
}

std::vector<std::string> loadMaterialNames()
{
	std::cout << "MATERIAL::LOAD_MATERIAL_NAMES ";
	std::vector<std::string> t_materialNames;
	std::cout << std::string(FILES_PATH) + MATERIALS_LIST_FILE_NAME + CONFIG_FILE_EXTENSION << " ";
	std::ifstream file(std::string(FILES_PATH) + MATERIALS_LIST_FILE_NAME + CONFIG_FILE_EXTENSION);
	if (file.bad() || file.fail())
	{
		std::cout << "FILE_BAD" << std::endl;
		return t_materialNames;
	}

	std::string t_materialName;

	while (!file.eof())
	{
		file >> t_materialName;
		std::cout << t_materialName << " ";
		t_materialNames.push_back(t_materialName);
	}
	file.close();
	std::cout << std::endl;
	return t_materialNames;
}

std::vector<Material> loadMaterialsfromFile()
{
	std::cout << "MATERIAL::LOAD_MATERIALS_FROM_FILE ";
	std::vector<Material> t_materials;
	std::cout << std::string(FILES_PATH) + MATERIALS_LIST_FILE_NAME + CONFIG_FILE_EXTENSION << " ";
	std::ifstream file(std::string(FILES_PATH) + MATERIALS_LIST_FILE_NAME + CONFIG_FILE_EXTENSION);
	if (file.bad() || file.fail())
	{
		std::cout << "FILE_BAD" << std::endl;
		return t_materials;
	}

	std::string t_materialName;

	while (!file.eof())
	{
		file >> t_materialName;
		t_materials.push_back(loadMaterial(t_materialName));
	}
	file.close();
	std::cout << std::endl;
	return t_materials;
}
