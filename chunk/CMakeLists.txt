add_library(chunk chunk.hpp chunk.cpp)
add_library(block block.hpp block.cpp)
target_link_libraries(chunk block)
