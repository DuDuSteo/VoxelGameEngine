#include "object.hpp"



Object::Object() {
    name = "new_object";
    memset(m_hashVoxels, 0, sizeof(m_hashVoxels));
    loadVertexBuffer(m_vertices);
    loadIndexBuffer(m_indices);

    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO); 
    
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex),
                 &m_vertices.front(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(uint32_t),
                 &m_indices.front(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *)sizeof(glm::vec3));
    glBindVertexArray(0);

    glEnable(GL_DEPTH_TEST);

    m_shader.init("basic", "basic");
    addVoxel(glm::ivec3(0, 0, 0), loadMaterial("ruby"));
}

void Object::draw(MVP mvp, glm::vec3 cameraPosition, Light light) { 
    m_shader.use();

    m_shader.setVec3("viewPos", cameraPosition);

    m_shader.setVec3("light.direction", light.direction);
    m_shader.setVec3("light.ambient", light.ambient);
    m_shader.setVec3("light.diffuse", light.diffuse);
    m_shader.setVec3("light.specular", light.specular);

    m_shader.setMat4("projection", mvp.projection);
    m_shader.setMat4("view", mvp.view);

    glm::mat4 objectModel = mvp.model;

    glBindVertexArray(m_VAO);

    for (Voxel voxel : m_voxels) {
        objectModel = glm::translate(mvp.model, voxel.pos);
        m_shader.setMat4("model", objectModel);
        m_shader.setVec3("material.ambient", voxel.mat.ambient);
        m_shader.setVec3("material.diffuse", voxel.mat.diffuse);
        m_shader.setVec3("material.specular", voxel.mat.specular);
        m_shader.setFloat("material.shininess", voxel.mat.shininess * 128);
      
        glm::ivec3 t_pos = glm::ivec3(VOXEL_COUNT / 2 + voxel.pos.x, VOXEL_COUNT / 2 + voxel.pos.y, VOXEL_COUNT / 2 + voxel.pos.z);
        //right
        if(t_pos.x + 1 <= VOXEL_COUNT)
            if(m_hashVoxels[t_pos.x + 1][t_pos.y][t_pos.z] == false)
                glDrawElements(GL_TRIANGLES, (GLsizei)36 / 6, GL_UNSIGNED_INT, (void *)(0 * sizeof(uint32_t))); 
        //left
        if(t_pos.x - 1 >= 0)
            if(m_hashVoxels[t_pos.x - 1][t_pos.y][t_pos.z] == false)
                glDrawElements(GL_TRIANGLES, (GLsizei)36 / 6, GL_UNSIGNED_INT, (void *)(6 * sizeof(uint32_t))); 
        //top
        if(t_pos.y + 1 <= VOXEL_COUNT)
            if(m_hashVoxels[t_pos.x][t_pos.y + 1][t_pos.z] == false)
                glDrawElements(GL_TRIANGLES, (GLsizei)36 / 6, GL_UNSIGNED_INT, (void *)(12 * sizeof(uint32_t))); 
        //bot
        if(t_pos.y - 1 >= 0)
            if(m_hashVoxels[t_pos.x][t_pos.y - 1][t_pos.z] == false)
                glDrawElements(GL_TRIANGLES, (GLsizei)36 / 6, GL_UNSIGNED_INT, (void *)(18 * sizeof(uint32_t))); 
        //front
        if(t_pos.z + 1 <= VOXEL_COUNT)
            if(m_hashVoxels[t_pos.x][t_pos.y][t_pos.z + 1] == false)
                glDrawElements(GL_TRIANGLES, (GLsizei)36 / 6, GL_UNSIGNED_INT, (void *)(24 * sizeof(uint32_t))); 
        //back
        if(t_pos.z - 1 >= 0)
            if(m_hashVoxels[t_pos.x][t_pos.y][t_pos.z - 1] == false)
                glDrawElements(GL_TRIANGLES, (GLsizei)36 / 6, GL_UNSIGNED_INT, (void *)(30 * sizeof(uint32_t)));  
    }
    return; 
}

void Object::addVoxel(glm::ivec3 pos, Material mat) {
    glm::ivec3 t_pos = glm::ivec3(VOXEL_COUNT / 2 + pos.x, VOXEL_COUNT / 2 + pos.y, VOXEL_COUNT / 2 + pos.z);
    if(t_pos.x < 0 || t_pos.x > VOXEL_COUNT) {
        std::cout << "OBJECT::ADD_VOXEL::POS::X Out of bounds " << std::endl;
        return;
    }
    if(t_pos.y < 0 || t_pos.y > VOXEL_COUNT) {
        std::cout << "OBJECT::ADD_VOXEL::POS::Y Out of bounds " << std::endl;
        return;
    }
    if(t_pos.z < 0 || t_pos.z > VOXEL_COUNT) {
        std::cout << "OBJECT::ADD_VOXEL::POS::Z Out of bounds " << std::endl;
        return;
    }
    if(m_hashVoxels[t_pos.x][t_pos.y][t_pos.z]){
        std::cout << "OBJECT::ADD_VOXEL Voxel already here" << std::endl;
        return;
    }

    Voxel t_voxel;
    t_voxel.pos = pos;
    t_voxel.mat = mat;
    m_voxels.push_back(t_voxel);
    m_hashVoxels[t_pos.x][t_pos.y][t_pos.z] = true;
    std::cout << "OBJECT::ADD_VOXEL (" << t_voxel.pos.x << ", " 
    << t_voxel.pos.y << ", " << t_voxel.pos.z << ") ("
    << t_voxel.mat.name << ")" << std::endl;
}

void Object::changeColor(Voxel* voxel, Material mat) {
    voxel->mat = mat;
}

void Object::removeVoxel(Voxel* voxel) {
    glm::ivec3 t_pos = glm::ivec3(VOXEL_COUNT / 2 + voxel->pos.x, VOXEL_COUNT / 2 + voxel->pos.y, VOXEL_COUNT / 2 + voxel->pos.z);
    m_hashVoxels[t_pos.x][t_pos.y][t_pos.z] = false;
    m_voxels.erase(m_voxels.begin() + (voxel - &m_voxels.front()));
}

void Object::removeVoxel(glm::vec3 pos) {
    std::cout << "OBJECT::REMOVE_VOXEL ";
    glm::ivec3 t_pos = glm::ivec3(VOXEL_COUNT / 2 + pos.x, VOXEL_COUNT / 2 + pos.y, VOXEL_COUNT / 2 + pos.z);
    if(m_hashVoxels[t_pos.x][t_pos.y][t_pos.z] == true) {
        m_hashVoxels[t_pos.x][t_pos.y][t_pos.z] = false;
        for(int i = 0; i < m_voxels.size(); i++) {
            if(m_voxels[i].pos == pos) {
                std::cout << "(" << pos.x << ", " << pos.y << ", " << pos.z << ") ";
                m_voxels.erase(m_voxels.begin() + (&m_voxels[i] - &m_voxels.front()));
                std::cout << "ERASED" << std::endl;
                return;
            }
        }
    } else {
        std::cout << "VOXEL_NOT_FOUND" << std::endl;
        return;
    }   
}
void Object::reset() {
    std::cout << "OBJECT::RESET " << name << " ";
    name = "new_object";
    memset(m_hashVoxels, 0, sizeof(m_hashVoxels));
    m_voxels.clear();
    std::cout << std::endl;
}

void Object::save() {
    std::cout << "OBJECT::SAVE " << std::string(FILES_PATH) + name + std::string(VOXEL_FILE_EXTENSION) << " ";
    std::ofstream file(std::string(FILES_PATH) + name + std::string(VOXEL_FILE_EXTENSION));
    if (file.bad() || file.fail()) {
        std::cout << "FILE_BAD" << std::endl;
        return;
    }
    for (Voxel voxel : m_voxels) {      
        file << voxel.pos.x << " " << voxel.pos.y << " " << voxel.pos.z << " " << voxel.mat.name << std::endl;
    }
    file.close();
    std::cout << std::endl;
    return; 
}

void Object::load(std::string objectPath) {
    std::cout << "OBJECT::LOAD " << objectPath << " ";
    std::ifstream file(objectPath);
    if (file.bad() || file.fail()) {
        std::cout << "FILE_BAD" << std::endl;
        return;
    }
    reset();
    glm::ivec3 t_pos;
    std::string t_matName;
    while(!file.eof()){
        file >> t_pos.x;
        file >> t_pos.y;
        file >> t_pos.z;
        file >> t_matName;
        addVoxel(t_pos, loadMaterial(t_matName));
    }
    return;
}

Voxel* Object::checkRay(glm::vec3 ray_origin, glm::vec3 ray_dir) {
    // return pointer to hitVoxel
    Voxel* ray_hit = nullptr;
    float ray_distance = MAX_RAY_RANGE;

    for (int i = 0; i < m_voxels.size(); i++) {
        glm::vec3 max = m_voxels[i].pos + glm::vec3(0.5f);
        glm::vec3 min = m_voxels[i].pos - glm::vec3(0.5f);

        float tmin = (min.x - ray_origin.x) / ray_dir.x; 
        float tmax = (max.x - ray_origin.x) / ray_dir.x; 
    
        if (tmin > tmax) std::swap(tmin, tmax); 
    
        float tymin = (min.y - ray_origin.y) / ray_dir.y; 
        float tymax = (max.y - ray_origin.y) / ray_dir.y; 
    
        if (tymin > tymax) std::swap(tymin, tymax); 

        if (tymin > tmin) 
            tmin = tymin; 
    
        if (tymax < tmax) 
            tmax = tymax;      
    
        float tzmin = (min.z - ray_origin.z) / ray_dir.z; 
        float tzmax = (max.z - ray_origin.z) / ray_dir.z; 
    
        if (tzmin > tzmax) std::swap(tzmin, tzmax); 
    
        if (tzmin > tmin) 
            tmin = tzmin; 
    
        if (tzmax < tmax) 
            tmax = tzmax;     
        if(!( ((tmin > tymax) || (tymin > tmax)) || ((tmin > tzmax) || (tzmin > tmax)) )) {
            float distance = sqrt(pow(m_voxels[i].pos.x - ray_origin.x, 2) +
                            pow(m_voxels[i].pos.y - ray_origin.y, 2) +
                            pow(m_voxels[i].pos.z - ray_origin.z, 2));
            if(distance < ray_distance) {
                ray_hit = &m_voxels[i];
                ray_distance = distance;
            }   
        }       
    }
    if(ray_distance == MAX_RAY_RANGE) {
        //std::cout << "missed" << std::endl;
        return nullptr;
    } else {
        std::cout << "(" << ray_hit->pos.x << ", " << ray_hit->pos.y << ", " << ray_hit->pos.z << ") at distance: " << ray_distance << std::endl;
        return ray_hit;
    }
}

std::vector<Voxel> Object::getListOfVoxels(){
    return m_voxels;
}