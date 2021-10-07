#pragma warning(disable : 26495)
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <iostream>
#include <vector>
#include <queue>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "shader.h"
#include "scene_graph.h"
#include "camera.h"
#include "octree.h"
#include "voxel_object.h"
#include "chunk.h"

#define SCR_WIDTH 1280
#define SCR_HEIGHT 720
#define APPLICATION_NAME "VoxelGameEngine"
#define FRAME_TIME_SIZE 60 * 20

//Timings
float currentFrame = 0;
float deltaTime = 0;
float lastFrame = 0;
float frameTime[FRAME_TIME_SIZE];

//temporal
double mouseYOffset = 0;
bool wireVisible = false;
Material cubeMaterial;
Block block;
Chunk chunk;


struct Light {
	glm::vec3 direction;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
} light = {
	{0.f, -1.f, 0.f},
	{0.2f, 0.2f, 0.2f},
	{0.5f, 0.5f, 0.5f},
	{1.0f, 1.0f, 1.0f}
};


class VoxelGameEngine {
public:
	void run() {
		initWindow();
		initOpenGL();
		mainLoop();
		cleanup();
	}
private:
	GLFWwindow* window;
	uint32_t VAO, VBO, EBO;
	Shader shader;
	Camera* camera;
	SceneNode* root;

	void initWindow() {
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);

		window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, APPLICATION_NAME, NULL, NULL);
		if (window == NULL) {
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
		}

		glfwMakeContextCurrent(window);

		//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		glfwSetWindowUserPointer(window, this);
		glfwSetScrollCallback(window, scroll_callback);
		glfwSetCursorPos(window, SCR_WIDTH / 2, SCR_HEIGHT / 2);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			std::cout << "Failed to initialize GLAD" << std::endl;
			glfwTerminate();
		}

		//Imgui init

		IMGUI_CHECKVERSION();

		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();

		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 330 core");
		ImGui::StyleColorsDark();
	}

	void initOpenGL() {
		chunk.fill();

		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glGenVertexArrays(1, &VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, chunk.m_vertices.size() * sizeof(Vertex), &chunk.m_vertices.front(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, chunk.m_indices.size() * sizeof(uint32_t), &chunk.m_indices.front(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(glm::vec3));
		glEnableVertexAttribArray(1);

		glEnable(GL_DEPTH_TEST);
		
		shader.init("basic.vert", "basic.frag");
		// TEMPORAL STUFF HERE
		camera = new Camera();
		camera->Position = glm::vec3(0.f, 0.f, 50.f);
		memset(frameTime, 0, sizeof(frameTime));

		loadMaterial(cubeMaterial, "ruby");

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
		
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		

		glm::mat4 projection = glm::perspective(glm::radians(45.f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.f);
		glm::mat4 view = camera->GetViewMatrix();
		glm::mat4 model = glm::mat4(1.f);

		processInput();

		shader.use();

		

		shader.setVec3("viewPos", camera->Position);

		shader.setVec3("light.direction", light.direction);
		shader.setVec3("light.ambient", light.ambient);
		shader.setVec3("light.diffuse", light.diffuse);
		shader.setVec3("light.specular", light.specular);

		shader.setMat4("projection", projection);
		shader.setMat4("view", view);
		shader.setMat4("model", model);

		shader.setVec3("material.ambient", cubeMaterial.ambient);
		shader.setVec3("material.diffuse", cubeMaterial.diffuse);
		shader.setVec3("material.specular", cubeMaterial.specular);
		shader.setFloat("material.shininess", cubeMaterial.shininess * 128);
		
		if (wireVisible)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		
		glDrawElements(GL_TRIANGLES, (GLsizei)chunk.m_indices.size(), GL_UNSIGNED_INT, 0);
		

		/*std::vector<SceneNode*> t_children = root->getChildren();
		for (size_t i = 0; i < t_children.size(); i++) {

			shader.setVec3("material.ambient", t_children[i]->m_material.ambient);
			shader.setVec3("material.diffuse", t_children[i]->m_material.diffuse);
			shader.setVec3("material.specular", t_children[i]->m_material.specular);
			shader.setFloat("material.shininess", (t_children[i]->m_material.shininess * 128));

			shader.setMat4("model", t_children[i]->getMatrix());
			glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
		}	*/
	}

	void drawGUI() {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		//materialEditorGUI();
		debugInfoGUI();
		//sceneEditorGUI();
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

	void materialEditorGUI() {
		//--------------------------------CUBE EDITOR--------------------------------
		static char stringBuffer[15] = { 0 };

		ImGui::Begin("Material Editor");
		
		ImGui::Text("Material");
		ImGui::InputText("Name", stringBuffer, IM_ARRAYSIZE(stringBuffer));
		
		if (ImGui::Button("Save"))
			saveMaterial(cubeMaterial, stringBuffer);
		ImGui::SameLine();
		if(ImGui::Button("Load"))
			loadMaterial(cubeMaterial, stringBuffer);
		ImGui::SliderFloat3("Ambient", (float*)&cubeMaterial.ambient, 0.f, 1.f);
		ImGui::SliderFloat3("Diffuse", (float*)&cubeMaterial.diffuse, 0.f, 1.f);
		ImGui::SliderFloat3("Specular", (float*)&cubeMaterial.specular, 0.f, 1.f);
		ImGui::SliderFloat("Shininess", &cubeMaterial.shininess, 0.f, 1.f);
		ImGui::End();
	}

	void debugInfoGUI() {
		//--------------------------------DEBUG--------------------------------
		ImGui::Begin("Debug Info");
		if (ImGui::Button("WireFrame mode"))
			wireVisible ^= true;
		ImGui::PlotHistogram("", frameTime, IM_ARRAYSIZE(frameTime), 0, NULL, 0.0f, 16.f, ImVec2(200, 80));
		ImGui::End();
	}

	void sceneEditorGUI() {
		ImGui::Begin("Scene Editor");
			
		static int selected = 0;
		static float translation[3] = { 0.f, 0.f, 0.f };
		static float scale[3] = { 1.f, 1.f, 1.f };
		static float rotatation[4] = { 0.f, 0.f, 0.f, 1.f };
		static char objectName[16] = { 0 };
		static char materialName[16] = { 0 };

		{
			ImGui::BeginGroup();
			if (ImGui::BeginChild("Scene", ImVec2(150, 200), true)) {
				std::vector<SceneNode*> t_children = root->getChildren();

				for (size_t i = 0; i < t_children.size(); i++) {
					if (ImGui::Selectable(t_children[i]->getName().c_str(), selected == i)) {
						selected = (int)i;
						memset(objectName, 0, 16);
						memset(materialName, 0, 16);
						for (size_t j = 0; j < t_children[i]->getName().size(); j++)
							objectName[j] = t_children[i]->getName()[j];
						for (size_t j = 0; j < t_children[i]->m_material.name.size(); j++)
							materialName[j] = t_children[i]->m_material.name[j];
						for (size_t j = 0; j < 3; j++)
						{
							translation[j] = t_children[i]->getTranslationf()[j];
							scale[j] = t_children[i]->getScalef()[j];
							rotatation[j] = t_children[i]->getRotationf()[j];
						}
						rotatation[3] = t_children[i]->getRotationf()[3];
					}
						
				}
				std::cout << selected << std::endl;

				
				ImGui::EndChild();	
			}
			if (ImGui::Button("Deselect"))
				selected = -1;
			ImGui::EndGroup();
		}
		
		
		ImGui::SameLine();
		{
			ImGui::BeginGroup();
			if(ImGui::BeginChild("Edit", ImVec2(0, 0), false)) {
				
				
				ImGui::InputText("Child Name", objectName, IM_ARRAYSIZE(objectName));
				ImGui::Text("Orientatation");
				ImGui::InputFloat3("translation", translation);
				ImGui::InputFloat3("scale", scale);
				ImGui::InputFloat3("rotatation", rotatation);
				
				ImGui::Text("Material");
				ImGui::InputText("Material Name", materialName, IM_ARRAYSIZE(materialName));
				
				if (ImGui::Button("Reset")) {
					memset(objectName, 0, 16);
					memset(materialName, 0, 16);
					for (int i = 0; i < 3; i++) {
						translation[i] = 0.f;
						scale[i] = 1.f;
						rotatation[i] = 0.f;
					}
					rotatation[3] = 1.f;
				}
				ImGui::SameLine();
				if (selected == -1) {
					if (ImGui::Button("Add"))
						root->addChild(new SceneNode(objectName, translation, scale, rotatation, materialName));
				}
				else {
					if (ImGui::Button("Edit"))
						root->getChildren()[selected]->setThis(new SceneNode(objectName, translation, scale, rotatation, materialName));
					ImGui::SameLine();
					if (ImGui::Button("Delete")) {
						root->destroyChild(selected);
						selected = -1;
					}
				}
				
						
				ImGui::EndChild();
			}
			ImGui::EndGroup();
		}
			
			

			
		ImGui::End();	
	}

	void lightPropertiesGUI() {
		ImGui::Begin("Light Properties");
		ImGui::SliderFloat3("Direction", (float*)&light.direction, -1.f, 1.f);
		ImGui::SliderFloat3("Light Ambient", (float*)&light.ambient, 0.f, 1.f);
		ImGui::SliderFloat3("Light Diffuse", (float*)&light.diffuse, 0.f, 1.f);
		ImGui::SliderFloat3("Light Specular", (float*)&light.specular, 0.f, 1.f);
		ImGui::End();
	}

	void processInput() {
		//--------------------------------ESC TO QUIT--------------------------------
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GLFW_TRUE);

		//--------------------------------CAMERA MOVEMENT--------------------------------
		static bool blockedCamera = true;
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
			double xpos, ypos;
			glfwGetCursorPos(window, &xpos, &ypos);
			camera->ProcessKeyboard((float)xpos, (float)ypos, deltaTime, blockedCamera);

			if (blockedCamera)
				blockedCamera = false;
		}
		else if ((glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS)) {
			double xpos, ypos;
			glfwGetCursorPos(window, &xpos, &ypos);
			camera->ProcessMouseMovement((float)xpos, (float)ypos, blockedCamera);

			if (blockedCamera)
				blockedCamera = false;
		}
		else
			if (!blockedCamera)
				blockedCamera = true;

	}

	inline static auto scroll_callback(GLFWwindow* window, double xoffset, double yoffset) -> void {
		VoxelGameEngine* voxelGame = static_cast<VoxelGameEngine*>(glfwGetWindowUserPointer(window));
		voxelGame->camera->ProcessMouseScroll((float)yoffset, deltaTime);
	}
};

int main() {
	VoxelGameEngine app;
	app.run();

}
