#include "file_handler.hpp"
#include <cstdint>
#include <fstream>

void saveBuffers(std::vector<Vertex> vert, std::vector<uint32_t> ind) {
  std::ofstream file(std::string(FILES_PATH) + std::string("vert_buffer.txt"));
  if (file.bad()) {
    std::cout << "FILE_HANDLER::SAVE_VERTEX_BUFFER::FILE_BAD" << std::endl;
    return;
  }
  for (int i = 0; i < vert.size(); i++) {
    file << vert[i].pos.x << " " << vert[i].pos.y << " " << vert[i].pos.z
         << " ";
    file << vert[i].normals.x << " " << vert[i].normals.y << " "
         << vert[i].normals.z << std::endl;
  }
  file.close();
  file.open(std::string(FILES_PATH) + std::string("ind_buffer.txt"));
  if (file.bad()) {
    std::cout << "FILE_HANDLER::SAVE_INDEX_BUFFER::FILE_BAD" << std::endl;
    return;
  }
  for (int i = 0; i < ind.size(); i++) {
    file << ind[i] << " ";
  }
  file.close();
}

void loadVertexBuffer(std::vector<Vertex> &vert) {
  std::ifstream file(std::string(FILES_PATH) + std::string("vert_buffer.txt"));
  if (file.bad()) {
    std::cout << "FILE_HANDLER::LOAD_VERTEX_BUFFER::FILE_BAD" << std::endl;
    return;
  }
  Vertex t_vert;
  while (!file.eof()) {
    file >> t_vert.pos.x;
    file >> t_vert.pos.y;
    file >> t_vert.pos.z;
    file >> t_vert.normals.x;
    file >> t_vert.normals.y;
    file >> t_vert.normals.z;
    vert.push_back(t_vert);
  }
  file.close();
}

void loadIndexBuffer(std::vector<uint32_t> &ind) {
  std::ifstream file(std::string(FILES_PATH) + std::string("ind_buffer.txt"));
  if (file.bad()) {
    std::cout << "FILE_HANDLER::LOAD_INDEX_BUFFER::FILE_BAD" << std::endl;
    return;
  }
  uint32_t t_ind;
  while (!file.eof()) {
    file >> t_ind;
    ind.push_back(t_ind);
  }
  file.close();
}