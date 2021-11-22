cmake_minimum_required(VERSION 3.2)
project(VoxelEditor)
set(SOURCE VoxelEditor.cpp)

#GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(GLFW)

#glm
add_subdirectory(glm)

set(IMGUI_DIR imgui)
include_directories(${IMGUI_DIR} ${IMGUI_DIR}/backends)

add_subdirectory(shader)
add_subdirectory(camera)
add_subdirectory(material)
add_subdirectory(file_handler)
add_subdirectory(object)

set(IMGUI_SOURCES 
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp 
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp 
    ${IMGUI_DIR}/imgui.cpp 
    ${IMGUI_DIR}/imgui_draw.cpp 
    ${IMGUI_DIR}/imgui_demo.cpp 
    ${IMGUI_DIR}/imgui_tables.cpp 
    ${IMGUI_DIR}/imgui_widgets.cpp)

add_executable(${PROJECT_NAME} ${SOURCE} glad.c ${IMGUI_SOURCES})
target_link_libraries(${PROJECT_NAME} glfw glm shader camera material file_handler object ${CMAKE_DL_LIBS})
