cmake_minimum_required(VERSION 3.2)
project(VoxelEditor)

find_package(glfw3 3.3 REQUIRED)
find_package(glm REQUIRED)

set(IMGUI_DIR imgui)
include_directories(${IMGUI_DIR} ${IMGUI_DIR}/backends)

add_subdirectory(shader)
add_subdirectory(camera)
add_subdirectory(octree)
add_subdirectory(chunk)

set(IMGUI_SOURCES 
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp 
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp 
    ${IMGUI_DIR}/imgui.cpp 
    ${IMGUI_DIR}/imgui_draw.cpp 
    ${IMGUI_DIR}/imgui_demo.cpp 
    ${IMGUI_DIR}/imgui_tables.cpp 
    ${IMGUI_DIR}/imgui_widgets.cpp)

add_executable(${PROJECT_NAME} VoxelEditor.cpp glad.c ${IMGUI_SOURCES})
target_link_libraries(${PROJECT_NAME} glfw glm shader camera octree chunk ${CMAKE_DL_LIBS})