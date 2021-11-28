cmake_minimum_required(VERSION 3.2)
project(VoxelEditor)

set(LIB_DIR ${PROJECT_SOURCE_DIR}/libs)

set(SOURCES 
   # ${PROJECT_SOURCE_DIR}/VoxelTester.cpp
    ${PROJECT_SOURCE_DIR}/object/object.cpp 
    ${PROJECT_SOURCE_DIR}/VoxelEditor.cpp
    ${PROJECT_SOURCE_DIR}/shader/shader.cpp
    ${PROJECT_SOURCE_DIR}/camera/camera.cpp 
    ${PROJECT_SOURCE_DIR}/material/material.cpp 
    ${PROJECT_SOURCE_DIR}/file_handler/file_handler.cpp 
    ${PROJECT_SOURCE_DIR}/console_log/console_log.cpp 
)

#imgui
set(IMGUI_DIR ${LIB_DIR}/imgui)
include_directories(${IMGUI_DIR} ${IMGUI_DIR}/backends)

set(IMGUI_SOURCES 
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp 
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp 
    ${IMGUI_DIR}/imgui.cpp 
    ${IMGUI_DIR}/imgui_draw.cpp 
    ${IMGUI_DIR}/imgui_demo.cpp 
    ${IMGUI_DIR}/imgui_tables.cpp 
    ${IMGUI_DIR}/imgui_widgets.cpp)

add_executable(${PROJECT_NAME} ${SOURCES} ${IMGUI_SOURCES})

#glad
set(GLAD_DIR ${LIB_DIR}/glad)
add_library(glad ${GLAD_DIR}/src/glad.c)
target_include_directories(glad PRIVATE ${GLAD_DIR}/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${GLAD_DIR}/include)
target_link_libraries(${PROJECT_NAME} glad ${CMAKE_DL_LIBS})

#GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

set(GLFW_DIR ${LIB_DIR}/GLFW)
add_subdirectory(${GLFW_DIR})

target_link_libraries(${PROJECT_NAME} glfw ${GLFW_LIBRARIES})
target_include_directories(${PROJECT_NAME} PRIVATE ${GLFW_DIR}/include)
target_compile_definitions(${PROJECT_NAME} PRIVATE GLFW_INCLUDE_NONE)

#glm
set(GLM_DIR ${LIB_DIR}/glm)
include_directories(${GLM_DIR})