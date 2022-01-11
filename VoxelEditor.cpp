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

class StateHandler
{
public:
  StateHandler()
  {
    m_addMode = true;
    m_removeMode = false;
    m_colorMode = false;
    sceneWindow = false;
    materialWindow = false;
    debugWindow = false;
    objectWindow = false;
    saveAsWindow = false;
    OpenModelWindow = false;
  }
  bool getAddMode()
  {
    return m_addMode;
  }
  bool getRemoveMode()
  {
    return m_removeMode;
  }
  bool getColorMode()
  {
    return m_colorMode;
  }
  void changeAddMode()
  {
    bool t_mode = m_addMode;
    resetModes();
    m_addMode = !t_mode;
  }
  void changeRemoveMode()
  {
    bool t_mode = m_removeMode;
    resetModes();
    m_removeMode = !t_mode;
  }
  void changeColorMode()
  {
    bool t_mode = m_colorMode;
    resetModes();
    m_colorMode = !t_mode;
  }
  void changeMode(int mode)
  {
    switch (mode)
    {
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
  bool sceneWindow;
  bool materialWindow;
  bool debugWindow;
  bool objectWindow;
  bool saveAsWindow;
  bool OpenModelWindow;

private:
  void resetModes()
  {
    m_addMode = false;
    m_removeMode = false;
    m_colorMode = false;
  }
  bool m_addMode;
  bool m_removeMode;
  bool m_colorMode;
};

class VoxelGameEngine
{
public:
  void run()
  {
    initWindow();
    initEngine();
    mainLoop();
    cleanup();
  }

private:
  GLFWwindow *window;
  Camera *camera;
  Object *object;
  MVP mvp;
  StateHandler *stateHandler;
  glm::vec4 backgroundColor = {0.2f, 0.2f, 0.2f, 1.f};
  std::string activeMaterialName;
  std::vector<std::string> materials;

  void initWindow()
  {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);

    window =
        glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, APPLICATION_NAME, NULL, NULL);
    if (window == NULL)
    {
      std::cout << "Failed to create GLFW window" << std::endl;
      glfwTerminate();
    }

    glfwMakeContextCurrent(window);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetWindowUserPointer(window, this);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPos(window, SCR_WIDTH / 2, SCR_HEIGHT / 2);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
      std::cout << "Failed to initialize GLAD" << std::endl;
      glfwTerminate();
    }

    IMGUI_CHECKVERSION();

    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");
    ImGui::StyleColorsDark();
  }

  void initEngine()
  {
    object = new Object();
    camera = new Camera();
    stateHandler = new StateHandler();
    materials = loadMaterialNames();
    activeMaterialName = materials[0];

    camera->Position = glm::vec3(0.f, 0.f, 20.f);
    memset(frameTime, 0, sizeof(frameTime));
  }

  void mainLoop()
  {
    while (!glfwWindowShouldClose(window))
    {
      glfwPollEvents();
      drawFrame();
      drawGUI();
      glfwSwapBuffers(window);
    }
  }

  void drawFrame()
  {
    currentFrame = (float)glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    for (size_t i = 1; i < FRAME_TIME_SIZE; i++)
    {
      frameTime[i - 1] = frameTime[i];
    }
    frameTime[FRAME_TIME_SIZE - 1] = deltaTime * 1000;
    glClearColor(backgroundColor.x, backgroundColor.y, backgroundColor.z, backgroundColor.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (wireVisible)
    {
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    mvp.projection = glm::perspective(
        glm::radians(45.f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.f);
    mvp.view = camera->GetViewMatrix();
    mvp.model = glm::mat4(1.f);

    processInput();

    object->draw(mvp, camera->Position, light);
  }

  void drawGUI()
  {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    menuGUI();
    if (stateHandler->debugWindow)
      debugGUI();
    if (stateHandler->sceneWindow)
      sceneGUI();
    if (stateHandler->objectWindow)
      objectGUI();
    editOptionsGUI();
    if (stateHandler->materialWindow)
      materialGUI();
    if (stateHandler->saveAsWindow)
      saveAsGUI();
    if (stateHandler->OpenModelWindow)
      openModelGUI();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  }

  void cleanup()
  {
    glfwDestroyWindow(window);
    glfwTerminate();
  }

  void debugGUI()
  {
    ImGui::Begin("Debug", &stateHandler->debugWindow);
    if (ImGui::Button("WireFrame mode"))
      wireVisible ^= true;
    ImGui::PlotHistogram("", frameTime, IM_ARRAYSIZE(frameTime), 0, NULL, 0.0f,
                         16.f, ImVec2(200, 80));
    ImGui::End();
  }

  void sceneGUI()
  {
    ImGui::Begin("Scene", &stateHandler->sceneWindow);
    ImGui::Text("Background");
    ImGui::ColorEdit4("Color", (float *)&backgroundColor);
    ImGui::Text("Light");
    ImGui::SliderFloat3("Direction", (float *)&light.direction, -1.f, 1.f);
    ImGui::SliderFloat3("Ambient", (float *)&light.ambient, 0.f, 1.f);
    ImGui::SliderFloat3("Diffuse", (float *)&light.diffuse, 0.f, 1.f);
    ImGui::SliderFloat3("Specular", (float *)&light.specular, 0.f, 1.f);
    ImGui::End();
  }

  void menuGUI()
  {
    if (ImGui::BeginMainMenuBar())
    {
      if (ImGui::BeginMenu("File"))
      {
        if (ImGui::MenuItem("New Model"))
        {
          object->reset();
          object->addVoxel(glm::ivec3(0, 0, 0), loadMaterial("ruby"));
        }
        ImGui::MenuItem("Open Model", "", &stateHandler->OpenModelWindow);
        ImGui::MenuItem("Save As", "", &stateHandler->saveAsWindow);
        ImGui::EndMenu();
      }

      if (ImGui::BeginMenu("Window"))
      {
        ImGui::MenuItem("Scene", "", &stateHandler->sceneWindow);
        ImGui::MenuItem("Material", "", &stateHandler->materialWindow);
        ImGui::MenuItem("Debug", "", &stateHandler->debugWindow);
        ImGui::MenuItem("Object", "", &stateHandler->objectWindow);
        ImGui::EndMenu();
      }
      ImGui::EndMainMenuBar();
    }
  }

  void saveAsGUI()
  {
    ImGui::Begin("Save As", &stateHandler->saveAsWindow);
    ImGui::InputText("Name", &object->name);
    if (ImGui::Button("Save"))
    {
      stateHandler->saveAsWindow = false;
      object->save();
    }
    ImGui::SameLine();
    if (ImGui::Button("Cancel"))
    {
      stateHandler->saveAsWindow = false;
    }
    ImGui::End();
  }

  void openModelGUI()
  {
    ImGui::Begin("Open Model", &stateHandler->OpenModelWindow);
    static std::string loadName = std::string(FILES_PATH) + object->name + std::string(VOXEL_FILE_EXTENSION);
    ImGui::InputText("Path", &loadName);
    if (ImGui::Button("Open"))
    {
      stateHandler->OpenModelWindow = false;
      object->load(loadName);
    }
    ImGui::SameLine();
    if (ImGui::Button("Cancel"))
    {
      stateHandler->OpenModelWindow = false;
    }
    ImGui::End();
  }

  void objectGUI()
  {
    static glm::ivec3 pos = glm::ivec3(0, 0, 0);
    ImGui::Begin("Voxel Handler", &stateHandler->objectWindow);
    ImGui::InputInt3("Voxel Position", (int *)&pos);
    if (ImGui::Button("Add Voxel"))
      object->addVoxel(pos, loadMaterial(activeMaterialName));
    ImGui::SameLine();
    if (ImGui::Button("Remove Voxel"))
      object->removeVoxel(pos);
    ImGui::End();
  }

  void editOptionsGUI()
  {
    static int e = 0;

    ImGui::Begin("Edit Modes");
    if (ImGui::RadioButton("Add", &e, 0))
      stateHandler->changeAddMode();
    ImGui::SameLine();
    if (ImGui::RadioButton("Remove", &e, 1))
      stateHandler->changeRemoveMode();
    ImGui::SameLine();
    if (ImGui::RadioButton("Color", &e, 2))
      stateHandler->changeColorMode();
    ImGui::End();
  }

  void materialGUI()
  {
    ImGui::Begin("Material", &stateHandler->materialWindow);
    static Material newMaterial;
    ImGui::Text("Saved Materials");
    if (ImGui::BeginCombo("List", activeMaterialName.c_str()))
    {
      for (int n = 0; n < materials.size(); n++)
      {
        bool is_selected = (activeMaterialName == materials[n]);
        if (ImGui::Selectable(materials[n].c_str(), is_selected))
        {
          activeMaterialName = materials[n];
          if (newMaterial.name != activeMaterialName)
          {
          }
          newMaterial = loadMaterial(activeMaterialName);
        }
        if (is_selected)
          ImGui::SetItemDefaultFocus();
      }
      ImGui::EndCombo();
    }
    ImGui::Text("Create Material");
    ImGui::InputText("Name", &newMaterial.name);
    ImGui::ColorEdit3("Ambient", (float *)&newMaterial.ambient);
    ImGui::ColorEdit3("Diffuse", (float *)&newMaterial.diffuse);
    ImGui::ColorEdit3("Specular", (float *)&newMaterial.specular);
    ImGui::SliderFloat("Shininess", &newMaterial.shininess, 0.f, 1.f);
    if (ImGui::Button("Save"))
    {
      bool t_edit = false;
      for (std::string materialName : materials)
      {
        if (materialName == newMaterial.name)
        {
          t_edit = true;
        }
      }
      saveMaterial(newMaterial, newMaterial.name, t_edit);
      materials = loadMaterialNames();
    }
    ImGui::SameLine();
    if (ImGui::Button("Refresh"))
    {
      materials = loadMaterialNames();
    }

    ImGui::End();
  }

  void processInput()
  {
    //ESC to leave
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(window, GLFW_TRUE);

    //Camera Handler
    static bool blockedCamera = true;
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS &&
        glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
      double xpos, ypos;
      glfwGetCursorPos(window, &xpos, &ypos);
      camera->ProcessKeyboard((float)xpos, (float)ypos, deltaTime,
                              blockedCamera);

      if (blockedCamera)
        blockedCamera = false;
    }
    else if ((glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) ==
              GLFW_PRESS))
    {
      double xpos, ypos;
      glfwGetCursorPos(window, &xpos, &ypos);
      camera->ProcessMouseMovement((float)xpos, (float)ypos, blockedCamera);

      if (blockedCamera)
        blockedCamera = false;
    }
    else if (!blockedCamera)
      blockedCamera = true;
  }

  glm::vec2 getScreenPos()
  {
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    xpos = xpos / (SCR_WIDTH / 2) - 1;
    ypos = 1 - ypos / (SCR_HEIGHT / 2);

    return glm::vec2(xpos, ypos);
  }

  glm::vec3 getRayCast(glm::mat4 projection, glm::mat4 view)
  {
    glm::vec4 ray_point = glm::vec4(getScreenPos(), -1.f, 1.f);
    glm::vec4 ray_eye = glm::inverse(projection) * ray_point;
    ray_eye = glm::vec4(glm::vec2(ray_eye), -1.f, 0.f);
    glm::vec3 ray_world = glm::vec3(glm::inverse(view) * ray_eye);
    return ray_world;
  }

  inline static auto scroll_callback(GLFWwindow *window, double xoffset,
                                     double yoffset) -> void
  {
    VoxelGameEngine *voxelGame =
        static_cast<VoxelGameEngine *>(glfwGetWindowUserPointer(window));
    voxelGame->camera->ProcessMouseScroll((float)yoffset, deltaTime);
  }

  inline static auto mouse_button_callback(GLFWwindow *window, int button, int action, int mods) -> void
  {
    VoxelGameEngine *voxelGame =
        static_cast<VoxelGameEngine *>(glfwGetWindowUserPointer(window));
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
      glm::vec3 ray_origin = voxelGame->camera->Position;
      glm::vec3 ray_dir = voxelGame->getRayCast(voxelGame->mvp.projection, voxelGame->mvp.view);
      glm::vec3 newBlockLoc = glm::vec3(0.f);
      Voxel *t_voxel = voxelGame->object->checkRay(ray_origin, ray_dir, newBlockLoc);
      if (t_voxel)
      {
        if (voxelGame->stateHandler->getColorMode())
          voxelGame->object->changeColor(t_voxel, loadMaterial(voxelGame->activeMaterialName));
        if (voxelGame->stateHandler->getRemoveMode())
          voxelGame->object->removeVoxel(t_voxel);
        if (voxelGame->stateHandler->getAddMode())
        {
          voxelGame->object->addVoxel(t_voxel->pos + newBlockLoc, loadMaterial(voxelGame->activeMaterialName));
        }
      }
    }
  }
};

int main()
{
  VoxelGameEngine app;
  app.run();
}
