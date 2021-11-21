#include <cstdint>
#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <glm/matrix.hpp>
#include <iostream>
#include <queue>
#include <vector>

#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "imgui/imgui.h"

#include "camera/camera.hpp"
#include "file_handler/file_handler.hpp"
#include "material/material.hpp"
#include "shader/shader.hpp"

#define SCR_WIDTH 1280
#define SCR_HEIGHT 720
#define APPLICATION_NAME "VoxelGameEngine"
#define FRAME_TIME_SIZE 60 * 20
#define RAYCAST_LENGTH 10.f

// Timings
float currentFrame = 0;
float deltaTime = 0;
float lastFrame = 0;
float frameTime[FRAME_TIME_SIZE];

// temporal
double mouseYOffset = 0;
bool wireVisible = false;
Material cubeMaterial;

struct Light {
  glm::vec3 direction;
  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;
} light = {{0.f, 0.f, -1.f},
           {0.2f, 0.2f, 0.2f},
           {0.5f, 0.5f, 0.5f},
           {1.0f, 1.0f, 1.0f}};

class VoxelGameEngine {
public:
  void run() {
    initWindow();
    initOpenGL();
    mainLoop();
    cleanup();
  }

private:
  GLFWwindow *window;
  uint32_t VAO, VBO, EBO;
  uint32_t VAO2, VBO2;
  Shader shader;
  Camera *camera;
  std::vector<Vertex> t_vertices;
  std::vector<uint32_t> t_indices;

  void initWindow() {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);

    window =
        glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, APPLICATION_NAME, NULL, NULL);
    if (window == NULL) {
      std::cout << "Failed to create GLFW window" << std::endl;
      glfwTerminate();
    }

    glfwMakeContextCurrent(window);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetWindowUserPointer(window, this);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetCursorPos(window, SCR_WIDTH / 2, SCR_HEIGHT / 2);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
      std::cout << "Failed to initialize GLAD" << std::endl;
      glfwTerminate();
    }

    // Imgui init

    IMGUI_CHECKVERSION();

    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");
    ImGui::StyleColorsDark();
  }

  void initOpenGL() {

    loadVertexBuffer(t_vertices);
    loadIndexBuffer(t_indices);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glGenVertexArrays(1, &VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, t_vertices.size() * sizeof(Vertex),
                 &t_vertices.front(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, t_indices.size() * sizeof(uint32_t),
                 &t_indices.front(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *)sizeof(glm::vec3));
    glEnableVertexAttribArray(1);

    glEnable(GL_DEPTH_TEST);

    shader.init("shader/basic.vert", "shader/basic.frag");
    // TEMPORAL STUFF HERE
    camera = new Camera();
    camera->Position = glm::vec3(0.f, 0.f, 50.f);
    memset(frameTime, 0, sizeof(frameTime));

    loadMaterial(cubeMaterial, "octree/ruby");
  }

  void mainLoop() {
    while (!glfwWindowShouldClose(window)) {
      glfwPollEvents();
      drawFrame();
      drawGUI();
      glfwSwapBuffers(window);
    }
  }
  void drawFace(Material faceMat, uint32_t start) {
    shader.setVec3("material.ambient", faceMat.ambient);
    shader.setVec3("material.diffuse", faceMat.diffuse);
    shader.setVec3("material.specular", faceMat.specular);
    shader.setFloat("material.shininess", faceMat.shininess * 128);

    glDrawElements(GL_TRIANGLES, (GLsizei)t_indices.size() / 6,
                   GL_UNSIGNED_INT, (void *)(start * sizeof(uint32_t)));
  }

  void drawFrame() {
    currentFrame = (float)glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    for (size_t i = 1; i < FRAME_TIME_SIZE; i++) {
      frameTime[i - 1] = frameTime[i];
    }
    frameTime[FRAME_TIME_SIZE - 1] = deltaTime * 1000;

    glClearColor(1.f, 1.f, 1.f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 projection = glm::perspective(
        glm::radians(45.f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.f);
    glm::mat4 view = camera->GetViewMatrix();
    glm::mat4 model = glm::mat4(1.f);

    processInput();
    glm::vec3 mouse_ray = getRayCast(projection, view, RAYCAST_LENGTH);
    shader.use();

    shader.setVec3("viewPos", camera->Position);

    shader.setVec3("light.direction", light.direction);
    shader.setVec3("light.ambient", light.ambient);
    shader.setVec3("light.diffuse", light.diffuse);
    shader.setVec3("light.specular", light.specular);

    shader.setMat4("projection", projection);
    shader.setMat4("view", view);
    shader.setMat4("model", model);

    if (wireVisible) {
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    for (int i = 0; i < 31; i += 6) {
      drawFace(cubeMaterial, i);
    }
  }

  void drawGUI() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    debugInfoGUI();
    lightPropertiesGUI();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  }

  void cleanup() {
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteBuffers(1, &VAO);
    glfwDestroyWindow(window);
    glfwTerminate();
  }

  void debugInfoGUI() {
    //--------------------------------DEBUG--------------------------------
    ImGui::Begin("Debug Info");
    if (ImGui::Button("WireFrame mode"))
      wireVisible ^= true;
    ImGui::PlotHistogram("", frameTime, IM_ARRAYSIZE(frameTime), 0, NULL, 0.0f,
                         16.f, ImVec2(200, 80));
    ImGui::End();
  }

  void lightPropertiesGUI() {
    ImGui::Begin("Light Properties");
    ImGui::SliderFloat3("Direction", (float *)&light.direction, -1.f, 1.f);
    ImGui::SliderFloat3("Light Ambient", (float *)&light.ambient, 0.f, 1.f);
    ImGui::SliderFloat3("Light Diffuse", (float *)&light.diffuse, 0.f, 1.f);
    ImGui::SliderFloat3("Light Specular", (float *)&light.specular, 0.f, 1.f);
    ImGui::End();
  }
  void processInput() {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(window, GLFW_TRUE);

    static bool blockedCamera = true;
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS &&
        glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
      double xpos, ypos;
      glfwGetCursorPos(window, &xpos, &ypos);
      camera->ProcessKeyboard((float)xpos, (float)ypos, deltaTime,
                              blockedCamera);

      if (blockedCamera)
        blockedCamera = false;
    } else if ((glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) ==
                GLFW_PRESS)) {
      double xpos, ypos;
      glfwGetCursorPos(window, &xpos, &ypos);
      camera->ProcessMouseMovement((float)xpos, (float)ypos, blockedCamera);

      if (blockedCamera)
        blockedCamera = false;
    } else if (!blockedCamera)
      blockedCamera = true;
  }

  glm::vec2 getScreenPos() {
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    xpos = xpos / (SCR_WIDTH / 2) - 1;
    ypos = 1 - ypos / (SCR_HEIGHT / 2);

    return glm::vec2(xpos, ypos);
  }

  glm::vec3 getRayCast(glm::mat4 projection, glm::mat4 view, float ray_length) {
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
      glm::vec4 ray_point = glm::vec4(getScreenPos(), -1.f, 1.f);
      glm::vec4 ray_eye = glm::inverse(projection) * ray_point;
      ray_eye = glm::vec4(glm::vec2(ray_eye), -1.f, 0.f);
      glm::vec3 ray_world = glm::vec3(glm::inverse(view) * ray_eye);
      std::cout << "<<< " << camera->Position.x << " " << camera->Position.y
                << " " << camera->Position.z << std::endl;
      ray_world = camera->Position + ray_world * ray_length;
      std::cout << ">>> " << ray_world.x << " " << ray_world.y << " "
                << ray_world.z << std::endl;
      return ray_world;
    } else
      return glm::vec3(0.f);
  }

  void rayCasting(glm::vec2 screen_pos) {
    glm::vec4 ray = glm::vec4(screen_pos, -1.f, 1.f);
  }

  inline static auto scroll_callback(GLFWwindow *window, double xoffset,
                                     double yoffset) -> void {
    VoxelGameEngine *voxelGame =
        static_cast<VoxelGameEngine *>(glfwGetWindowUserPointer(window));
    voxelGame->camera->ProcessMouseScroll((float)yoffset, deltaTime);
  }
};

int main() {
  VoxelGameEngine app;
  app.run();
}
