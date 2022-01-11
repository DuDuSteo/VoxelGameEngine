#include "object.hpp"

Object::Object()
{
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

void Object::draw(MVP mvp, glm::vec3 cameraPosition, Light light)
{
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

    for (Voxel voxel : m_voxels)
    {
        objectModel = glm::translate(mvp.model, voxel.pos);
        m_shader.setMat4("model", objectModel);
        m_shader.setVec3("material.ambient", voxel.mat.ambient);
        m_shader.setVec3("material.diffuse", voxel.mat.diffuse);
        m_shader.setVec3("material.specular", voxel.mat.specular);
        m_shader.setFloat("material.shininess", voxel.mat.shininess * 128);

        glm::ivec3 t_pos = glm::ivec3(VOXEL_COUNT / 2 + voxel.pos.x, VOXEL_COUNT / 2 + voxel.pos.y, VOXEL_COUNT / 2 + voxel.pos.z);
        //right
        if (t_pos.x + 1 <= VOXEL_COUNT)
            if (m_hashVoxels[t_pos.x + 1][t_pos.y][t_pos.z] == false)
                glDrawElements(GL_TRIANGLES, (GLsizei)36 / 6, GL_UNSIGNED_INT, (void *)(0 * sizeof(uint32_t)));
        //left
        if (t_pos.x - 1 >= 0)
            if (m_hashVoxels[t_pos.x - 1][t_pos.y][t_pos.z] == false)
                glDrawElements(GL_TRIANGLES, (GLsizei)36 / 6, GL_UNSIGNED_INT, (void *)(6 * sizeof(uint32_t)));
        //top
        if (t_pos.y + 1 <= VOXEL_COUNT)
            if (m_hashVoxels[t_pos.x][t_pos.y + 1][t_pos.z] == false)
                glDrawElements(GL_TRIANGLES, (GLsizei)36 / 6, GL_UNSIGNED_INT, (void *)(12 * sizeof(uint32_t)));
        //bot
        if (t_pos.y - 1 >= 0)
            if (m_hashVoxels[t_pos.x][t_pos.y - 1][t_pos.z] == false)
                glDrawElements(GL_TRIANGLES, (GLsizei)36 / 6, GL_UNSIGNED_INT, (void *)(18 * sizeof(uint32_t)));
        //front
        if (t_pos.z + 1 <= VOXEL_COUNT)
            if (m_hashVoxels[t_pos.x][t_pos.y][t_pos.z + 1] == false)
                glDrawElements(GL_TRIANGLES, (GLsizei)36 / 6, GL_UNSIGNED_INT, (void *)(24 * sizeof(uint32_t)));
        //back
        if (t_pos.z - 1 >= 0)
            if (m_hashVoxels[t_pos.x][t_pos.y][t_pos.z - 1] == false)
                glDrawElements(GL_TRIANGLES, (GLsizei)36 / 6, GL_UNSIGNED_INT, (void *)(30 * sizeof(uint32_t)));
    }
    return;
}

void Object::addVoxel(glm::ivec3 pos, Material mat)
{
    glm::ivec3 t_pos = glm::ivec3(VOXEL_COUNT / 2 + pos.x, VOXEL_COUNT / 2 + pos.y, VOXEL_COUNT / 2 + pos.z);
    if (t_pos.x < 0 || t_pos.x > VOXEL_COUNT)
    {
        std::cout << "OBJECT::ADD_VOXEL::POS::X Out of bounds " << std::endl;
        return;
    }
    if (t_pos.y < 0 || t_pos.y > VOXEL_COUNT)
    {
        std::cout << "OBJECT::ADD_VOXEL::POS::Y Out of bounds " << std::endl;
        return;
    }
    if (t_pos.z < 0 || t_pos.z > VOXEL_COUNT)
    {
        std::cout << "OBJECT::ADD_VOXEL::POS::Z Out of bounds " << std::endl;
        return;
    }
    if (m_hashVoxels[t_pos.x][t_pos.y][t_pos.z])
    {
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

void Object::changeColor(Voxel *voxel, Material mat)
{
    voxel->mat = mat;
}

void Object::removeVoxel(Voxel *voxel)
{
    glm::ivec3 t_pos = glm::ivec3(VOXEL_COUNT / 2 + voxel->pos.x, VOXEL_COUNT / 2 + voxel->pos.y, VOXEL_COUNT / 2 + voxel->pos.z);
    m_hashVoxels[t_pos.x][t_pos.y][t_pos.z] = false;
    m_voxels.erase(m_voxels.begin() + (voxel - &m_voxels.front()));
}

void Object::removeVoxel(glm::vec3 pos)
{
    std::cout << "OBJECT::REMOVE_VOXEL ";
    glm::ivec3 t_pos = glm::ivec3(VOXEL_COUNT / 2 + pos.x, VOXEL_COUNT / 2 + pos.y, VOXEL_COUNT / 2 + pos.z);
    if (m_hashVoxels[t_pos.x][t_pos.y][t_pos.z] == true)
    {
        m_hashVoxels[t_pos.x][t_pos.y][t_pos.z] = false;
        for (int i = 0; i < m_voxels.size(); i++)
        {
            if (m_voxels[i].pos == pos)
            {
                std::cout << "(" << pos.x << ", " << pos.y << ", " << pos.z << ") ";
                m_voxels.erase(m_voxels.begin() + (&m_voxels[i] - &m_voxels.front()));
                std::cout << "ERASED" << std::endl;
                return;
            }
        }
    }
    else
    {
        std::cout << "VOXEL_NOT_FOUND" << std::endl;
        return;
    }
}
void Object::reset()
{
    std::cout << "OBJECT::RESET " << name << " ";
    name = "new_object";
    memset(m_hashVoxels, 0, sizeof(m_hashVoxels));
    m_voxels.clear();
    std::cout << std::endl;
}

void Object::save()
{
    std::cout << "OBJECT::SAVE " << std::string(FILES_PATH) + name + std::string(VOXEL_FILE_EXTENSION) << " ";
    std::ofstream file(std::string(FILES_PATH) + name + std::string(VOXEL_FILE_EXTENSION));
    if (file.bad() || file.fail())
    {
        std::cout << "FILE_BAD" << std::endl;
        return;
    }
    for (Voxel voxel : m_voxels)
    {
        file << voxel.pos.x << " " << voxel.pos.y << " " << voxel.pos.z << " " << voxel.mat.name << std::endl;
    }
    file.close();
    std::cout << std::endl;
    return;
}

void Object::load(std::string objectPath)
{
    std::cout << "OBJECT::LOAD " << objectPath << " ";
    std::ifstream file(objectPath);
    if (file.bad() || file.fail())
    {
        std::cout << "FILE_BAD" << std::endl;
        return;
    }
    reset();
    glm::ivec3 t_pos;
    std::string t_matName;
    while (!file.eof())
    {
        file >> t_pos.x;
        file >> t_pos.y;
        file >> t_pos.z;
        file >> t_matName;
        addVoxel(t_pos, loadMaterial(t_matName));
    }
    return;
}

Voxel *Object::checkRay(glm::vec3 ray_origin, glm::vec3 ray_dir, glm::vec3 &newBlockLoc)
{
    // return pointer to hitVoxel
    Voxel *ray_hit = nullptr;
    float ray_distance = MAX_RAY_RANGE;
    int ray_axis;

    for (int i = 0; i < m_voxels.size(); i++)
    {
        glm::vec3 max = m_voxels[i].pos + glm::vec3(0.5f);
        glm::vec3 min = m_voxels[i].pos - glm::vec3(0.5f);

        float tmin = (min.x - ray_origin.x) / ray_dir.x;
        float t1 = tmin;
        float tmax = (max.x - ray_origin.x) / ray_dir.x;
        float t2 = tmax;

        if (tmin > tmax)
            std::swap(tmin, tmax);

        float tymin = (min.y - ray_origin.y) / ray_dir.y;
        float t3 = tymin;
        float tymax = (max.y - ray_origin.y) / ray_dir.y;
        float t4 = tymax;

        if (tymin > tymax)
            std::swap(tymin, tymax);

        if ((tmin > tymax) || (tymin > tmax))
            continue;

        if (tymin > tmin)
            tmin = tymin;

        if (tymax < tmax)
            tmax = tymax;

        float tzmin = (min.z - ray_origin.z) / ray_dir.z;
        float t5 = tzmin;
        float tzmax = (max.z - ray_origin.z) / ray_dir.z;
        float t6 = tzmax;

        if (tzmin > tzmax)
            std::swap(tzmin, tzmax);

        if ((tmin > tzmax) || (tzmin > tmax))
            continue;

        if (tzmin > tmin)
            tmin = tzmin;

        if (tzmax < tmax)
            tmax = tzmax;

        float distance = glm::distance(m_voxels[i].pos, ray_origin);

        if (distance < ray_distance)
        {
            float t_tminx = fmin(t1, t2);
            float t_tminy = fmin(t3, t4);
            float t_tmin = fmax(fmax(t_tminx, t_tminy), fmin(t5, t6));
            ray_axis = 2;
            if (t_tmin == t_tminx)
                ray_axis = 0;
            if (t_tmin == t_tminy)
                ray_axis = 1;

            ray_hit = &m_voxels[i];
            ray_distance = distance;
        }
    }
    if (ray_distance == MAX_RAY_RANGE)
    {
        return nullptr;
    }
    else
    {
        if (ray_axis == 2)
        {
            if (ray_origin.z - ray_hit->pos.z > 0)
                newBlockLoc.z = 1.f;
            else
                newBlockLoc.z = -1.f;
        }
        else if (ray_axis == 1)
        {
            if (ray_origin.y - ray_hit->pos.y > 0)
                newBlockLoc.y = 1.f;
            else
                newBlockLoc.y = -1.f;
        }
        else if (ray_axis == 0)
        {
            if (ray_origin.x - ray_hit->pos.x > 0)
                newBlockLoc.x = 1.f;
            else
                newBlockLoc.x = -1.f;
        }
        std::cout << "(" << ray_hit->pos.x << ", " << ray_hit->pos.y << ", " << ray_hit->pos.z << ") at distance: " << ray_distance << std::endl;
        return ray_hit;
    }
}

glm::vec3 Object::getFace(Voxel *voxel, glm::vec3 ray_origin, glm::vec3 ray_dir)
{
    glm::vec3 normalFaces[] = {
        {0.f, 0.f, 1.f},  // front
        {0.f, 0.f, -1.f}, // back
        {1.f, 0.f, 0.f},  // right
        {-1.f, 0.f, 0.f}, // left
        {0.f, 1.f, 0.f},  // top
        {0.f, -1.f, 0.f}  // bot
    };
    // l0-origin l-direction p0-point n-normal p-point
    glm::vec3 closestFace = glm::vec3(0.f);
    float distance = MAX_RAY_RANGE;
    for (glm::vec3 faceNormal : normalFaces)
    {
        float denom = glm::dot((faceNormal * -1.f), ray_dir);
        if (fabs(denom) > 1e-6)
        {
            glm::vec3 t_vec = (voxel->pos + (faceNormal * 0.5f)) - ray_origin;
            float t = glm::dot(t_vec, (faceNormal * -1.f)) / denom;
            if (t >= 0)
            {
                //std::cout << faceNormal.x << " " << faceNormal.y << " " << faceNormal.z << std::endl;
                if (t < distance)
                {
                    closestFace = faceNormal;
                    distance = t;
                }
            }
        }
    }
    return closestFace;
}

std::vector<Voxel> Object::getListOfVoxels()
{
    return m_voxels;
}