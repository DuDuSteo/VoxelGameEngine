#include "object.hpp"

Object::Object() {
    memset(m_hash_voxels, 0, sizeof(m_hash_voxels));
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

    m_shader.init("files/basic.vert", "files/basic.frag");
    addVoxel(glm::ivec3(0, 0, 0), loadMaterial("files/ruby.mat"));
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

    for (Voxel voxel : m_voxels) {
      objectModel = glm::translate(mvp.model, voxel.pos);
      m_shader.setMat4("model", objectModel);
      m_shader.setVec3("material.ambient", voxel.mat.ambient);
      m_shader.setVec3("material.diffuse", voxel.mat.diffuse);
      m_shader.setVec3("material.specular", voxel.mat.specular);
      m_shader.setFloat("material.shininess", voxel.mat.shininess * 128);
      
      for (int start = 0; start < 31; start += 6) {
        glBindVertexArray(m_VAO);   
        glDrawElements(GL_TRIANGLES, (GLsizei)36 / 6, GL_UNSIGNED_INT,
                   (void *)(start * sizeof(uint32_t)));
      }
    }

    return; }

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

void Object::checkRay(glm::vec3 ray_start, glm::vec3 ray_dir) {
    std::cout << "OBJECT::CHECK_RAY" << std::endl;
    
}

std::vector<Voxel> Object::getListOfVoxels(){
    return m_voxels;
}