#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>

#include <glm/matrix.hpp>
#include <iostream>
#include <queue>
#include <vector>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <misc/cpp/imgui_stdlib.h>


#include "material/material.hpp"
#include "camera/camera.hpp"
#include "file_handler/file_handler.hpp"
#include "material/material.hpp"
#include "object/object.hpp"
#include "shader/shader.hpp"

// Timings
float currentFrame = 0;
float deltaTime = 0;
float lastFrame = 0;
float frameTime[FRAME_TIME_SIZE];
bool wireVisible = false;
bool colorMode = true;

Light light = {{0.f, 0.f, -1.f},
           {0.2f, 0.2f, 0.2f},
           {0.5f, 0.5f, 0.5f},
           {1.0f, 1.0f, 1.0f}};

class EditModes {
  public:
    EditModes() {
      m_addMode = true;
      m_removeMode = false;
      m_colorMode = false;
    }
    bool getAddMode() {
      return m_addMode;
    }
    bool getRemoveMode() {
      return m_removeMode;
    }
    bool getColorMode() {
      return m_colorMode;
    }
    void changeAddMode() {
      bool t_mode = m_addMode;
      resetModes();
      m_addMode = !t_mode;
    }
    void changeRemoveMode() {
      bool t_mode = m_removeMode;
      resetModes();
      m_removeMode = !t_mode;
    }
    void changeColorMode() {
      bool t_mode = m_colorMode;
      resetModes();
      m_colorMode = !t_mode;
    }
    void changeMode(int mode) {
      switch(mode){
        case 0:
          changeAddMode();
          break;
        case 1:
          changeRemoveMode();
          break;
        case 2:
          changeColorMode();
          break;
      }   
    } 
  private:
    void resetModes() {
      m_addMode = false;
      m_removeMode = false;
      m_colorMode = false;    
    }
    bool m_addMode;
    bool m_removeMode;
    bool m_colorMode;
};

class DebugDraw {
  public:
    void init() {
      m_shader.init(std::string(FILES_PATH) + std::string("debug.vert"), std::string(FILES_PATH) + std::string("debug.frag"));
    }
    void drawLine(glm::vec3 start, glm::vec3 end, glm::mat4 projection, glm::mat4 view) {
      glm::mat4 model = glm::mat4(1.f);

      glGenVertexArrays(1, &m_VAO);
      glBindVertexArray(m_VAO); 

      glGenBuffers(1, &m_VBO);
      std::vector<glm::vec3> t_vertices;
      t_vertices.push_back(end);
      t_vertices.push_back(end + glm::vec3(1.f, 0.f, 0.f));
      

      glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
      glBufferData(GL_ARRAY_BUFFER, sizeof(t_vertices), &t_vertices.front(), GL_STATIC_DRAW);

      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
      glEnableVertexAttribArray(0);

      m_shader.use();
      m_shader.setVec3("color", glm::vec3(1.f, 0.f, 0.f));
      m_shader.setMat4("projection", projection);
      m_shader.setMat4("view", view);
      m_shader.setMat4("model", model);

      glDrawArrays(GL_LINE_STRIP, 0, t_vertices.size()); 

      glBindVertexArray(0); 
    }
  private:
    uint32_t m_VBO, m_VAO;
    Shader m_shader;
};

class VoxelGameEngine {
public:
  void run() {
    initWindow();
    initEngine();
    mainLoop();
    cleanup();
  }

private:
  GLFWwindow *window;
  Camera *camera;
  Object *object;
  EditModes *editModes;
  glm::vec4 backgroundColor = {1.f, 1.f, 1.f, 1.f};
  std::vector<std::string> materials;
  Material activeMaterial;

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

  void initEngine() {  
    object = new Object();
    camera = new Camera();
    editModes = new EditModes();
    materials = loadMaterialNames();
    activeMaterial = loadMaterial(materials[0]);

    camera->Position = glm::vec3(0.f, 0.f, 20.f);
    memset(frameTime, 0, sizeof(frameTime));
  }

  void mainLoop() {
    while (!glfwWindowShouldClose(window)) {
      glfwPollEvents();
      drawFrame();
      drawGUI();
      glfwSwapBuffers(window);
    }
  }

  void drawFrame() {
    currentFrame = (float)glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    for (size_t i = 1; i < FRAME_TIME_SIZE; i++) {
      frameTime[i - 1] = frameTime[i];
    }
    frameTime[FRAME_TIME_SIZE - 1] = deltaTime * 1000;
    glClearColor(backgroundColor.x, backgroundColor.y, backgroundColor.z, backgroundColor.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (wireVisible) {
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    MVP mvp;
    mvp.projection = glm::perspective(
        glm::radians(45.f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.f);
    mvp.view = camera->GetViewMatrix();
    mvp.model = glm::mat4(1.f);

    processInput(mvp);

    object->draw(mvp, camera->Position, light);
  }

  void drawGUI() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    debugInfoGUI();
    scenePropertiesGUI();
    objectHandlingGUI();
    rayCastingInfoGUI();
    editOptionsGUI();
    materialGUI();
    ImGui::ShowDemoWindow();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  }

  void cleanup() {
    glfwDestroyWindow(window);
    glfwTerminate();
  }

  void debugInfoGUI() {
    ImGui::Begin("Debug Info");
    if (ImGui::Button("WireFrame mode"))
      wireVisible ^= true;
    ImGui::PlotHistogram("", frameTime, IM_ARRAYSIZE(frameTime), 0, NULL, 0.0f,
                         16.f, ImVec2(200, 80));
    ImGui::End();
  }

  void scenePropertiesGUI() {
    ImGui::Begin("Scene Properties");
    ImGui::Text("Background");
    ImGui::ColorEdit4("Color", (float*)&backgroundColor);
    ImGui::Text("Light");
    ImGui::SliderFloat3("Direction", (float *)&light.direction, -1.f, 1.f);
    ImGui::SliderFloat3("Ambient", (float *)&light.ambient, 0.f, 1.f);
    ImGui::SliderFloat3("Diffuse", (float *)&light.diffuse, 0.f, 1.f);
    ImGui::SliderFloat3("Specular", (float *)&light.specular, 0.f, 1.f);
    ImGui::End();
  }

  void objectHandlingGUI() {
    static glm::ivec3 pos = glm::ivec3(0, 0, 0);
    ImGui::Begin("Voxel Handler");
    ImGui::InputInt3("Voxel Position", (int *)&pos);
    if(ImGui::Button("Add Voxel"))
      object->addVoxel(pos, activeMaterial);
    ImGui::SameLine();
    if(ImGui::Button("Remove Voxel"))
      object->removeVoxel(pos);
    ImGui::End();
  }

  void rayCastingInfoGUI() {
    // ImGui::Begin("Ray Casting Info");
    // ImGui::InputFloat3("Ray Position", (float *)&mouse_ray);
    // ImGui::InputFloat3("Camera Position", (float *)&camera->Position);
    // ImGui::InputFloat("Ray Length", &raycastLength);
    // ImGui::End();
  }

  void editOptionsGUI() {
    static int e = 0;

    ImGui::Begin("Edit Modes");
    if(ImGui::RadioButton("Add", &e, 0))
      editModes->changeAddMode();
    ImGui::SameLine();
    if(ImGui::RadioButton("Remove", &e, 1))
      editModes->changeRemoveMode();
    ImGui::SameLine();
    if(ImGui::RadioButton("Color", &e, 2))
      editModes->changeColorMode();    
    ImGui::End();
  }

  void materialGUI(){

    ImGui::Begin("Material");
    static std::string current_item = materials[0];

    ImGui::Text("Saved Materials");
    if (ImGui::BeginCombo("List", current_item.c_str())) {
      for (int n = 0; n < materials.size(); n++) {
        bool is_selected = (current_item == materials[n]);
        if (ImGui::Selectable(materials[n].c_str(), is_selected)) {     
          current_item = materials[n];
          if(activeMaterial.name != current_item){}
            activeMaterial = loadMaterial(current_item);
        }     
        if (is_selected) 
          ImGui::SetItemDefaultFocus(); 
          
      }
    ImGui::EndCombo();
    }
    // if(ImGui::Button("Remove")) {
    //   removeMaterial(activeMaterial.name);
    // }
    ImGui::Text("Active Material Properties");
    ImGui::InputText("Name", &activeMaterial.name);
    ImGui::ColorEdit3("Ambient" ,(float*)&activeMaterial.ambient);
    ImGui::ColorEdit3("Diffuse", (float*)&activeMaterial.diffuse);
		ImGui::ColorEdit3("Specular", (float*)&activeMaterial.specular);
    ImGui::SliderFloat("Shininess", &activeMaterial.shininess, 0.f, 1.f);
    if (ImGui::Button("Save")) {
      bool t_edit = false;
      for(std::string materialName : materials) {
        if(materialName == activeMaterial.name) {
          t_edit = true;
        }
      }
      saveMaterial(activeMaterial, activeMaterial.name, t_edit);
      materials = loadMaterialNames();
    }
    ImGui::SameLine();
    if (ImGui::Button("Refresh")) {
      materials = loadMaterialNames();
    }

    ImGui::End();
  }

  void processInput(MVP mvp) {
    //ESC to leave
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(window, GLFW_TRUE);

    //Camera Handler
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

    //LMB handle the click
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
      Voxel* t_voxel = object->checkRay(camera->Position, getRayCast(mvp.projection, mvp.view));
      if(t_voxel) {
        if(editModes->getColorMode())
          object->changeColor(t_voxel, activeMaterial);
        if(editModes->getRemoveMode())
          object->removeVoxel(t_voxel);
        if(editModes->getAddMode())
          std::cout << "ADD_VOXEL" << std::endl;
      }
      
    }
  }

  glm::vec2 getScreenPos() {
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    xpos = xpos / (SCR_WIDTH / 2) - 1;
    ypos = 1 - ypos / (SCR_HEIGHT / 2);

    return glm::vec2(xpos, ypos);
  }

  glm::vec3 getRayCast(glm::mat4 projection, glm::mat4 view) {
    glm::vec4 ray_point = glm::vec4(getScreenPos(), -1.f, 1.f);
    glm::vec4 ray_eye = glm::inverse(projection) * ray_point;
    ray_eye = glm::vec4(glm::vec2(ray_eye), -1.f, 0.f);
    glm::vec3 ray_world = glm::vec3(glm::inverse(view) * ray_eye);
    return ray_world;
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
