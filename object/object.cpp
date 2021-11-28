#include "object.hpp"

Object::Object() {
    memset(m_hash_voxels, 0, sizeof(m_hash_voxels));
}

void Object::draw(glm::mat4 model, Shader shader) { return; }

void Object::addVoxel(glm::ivec3 pos, Material mat) {
    if(m_hash_voxels[VOXEL_COUNT / 2 + pos.x]
                    [VOXEL_COUNT / 2 + pos.y]
                    [VOXEL_COUNT / 2 + pos.z]){
        std::cout << "OBJECT::ADD_VOXEL Voxel already here" << std::endl;
        return;
    }
    Voxel t_voxel;
    t_voxel.pos = pos;
    t_voxel.mat = mat;
    m_voxels.push_back(t_voxel);
    m_hash_voxels[VOXEL_COUNT / 2 + pos.x][VOXEL_COUNT / 2 + pos.y][VOXEL_COUNT / 2 + pos.z] = true;
    std::cout << "OBJECT::ADD_VOXEL (" << t_voxel.pos.x << ", " 
    << t_voxel.pos.y << ", " << t_voxel.pos.z << ") ("
    << t_voxel.mat.name << ")" << std::endl;
}

std::vector<Voxel> Object::getListOfVoxels(){
    return m_voxels;
}