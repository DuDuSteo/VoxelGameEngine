add_library(octree octree.hpp octree.cpp)
add_library(material material.hpp material.cpp)
target_link_libraries(octree material)
target_link_libraries(octree block)