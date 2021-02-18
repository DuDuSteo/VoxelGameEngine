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
#include "camera.h"
#include "material.hpp"

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
bool wireVisible = false;
glm::vec3 cubePosition = glm::vec3(0.f, 0.f, 0.f);
material::Material cubeMaterial;

struct Vertex {
	glm::vec3 pos;
	glm::vec3 normals;
};

struct Light {
	glm::vec3 direction;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
} light = {
	{0.f, -1.f, -1.f},
	{1.f, 1.f, 1.f},
	{1.f, 1.f, 1.f},
	{1.f, 1.f, 1.f}
};

const std::vector<Vertex> vertices = {
	{{1.00000, 1.00000, -1.000000},{0.000, 1.000, 0.0000}},		//0
	{{1.00000, -1.00000, -1.000000},{0.000, -1.000, 0.0000}},		//1
	{{1.00000, 1.00000, 1.000000},{0.000, 1.000, 0.0000}},		//2
	{{1.00000, -1.00000, 1.000000},{0.000, 0.000, 1.0000}},		//3
	{{-1.00000, 1.00000, -1.000000},{0.000, 1.000, 0.0000}},		//4
	{{-1.00000, -1.00000, -1.000000},{-1.000, 0.000, 0.0000}},		//5
	{{-1.00000, 1.00000, 1.000000},{-1.000, 0.000, 0.0000}},		//6
	{{-1.00000, -1.00000, 1.000000},{0.000, 0.000, 1.0000}},		//7

	{{1.00000, 1.00000, -1.000000},{1.000, 0.000, 0.0000}},		//0 + 8
	{{1.00000, -1.00000, -1.000000},{1.000, 0.000, 0.0000}},		//1 + 8
	{{1.00000, 1.00000, 1.000000},{0.000, 0.000, 1.0000}},		//2 + 8
	{{1.00000, -1.00000, 1.000000},{1.000, 0.000, 0.0000}},		//3 + 8
	{{-1.00000, 1.00000, -1.000000},{0.000, 0.000, -1.0000}},		//4 + 8
	{{-1.00000, -1.00000, -1.000000},{0.000, -1.000, 0.0000}},		//5 + 8
	{{-1.00000, 1.00000, 1.000000},{0.000, 1.000, 0.0000}},		//6 + 8
	{{-1.00000, -1.00000, 1.000000},{-1.000, 0.000, 0.0000}},		//7 + 8

	{{1.00000, 1.00000, -1.000000},{0.000, 0.000, -1.0000}},		//0 + 16
	{{1.00000, -1.00000, -1.000000},{0.000, 0.000, -1.0000}},		//1 + 16
	{{1.00000, 1.00000, 1.000000},{1.000, 0.000, 0.0000}},		//2 + 16
	{{1.00000, -1.00000, 1.000000},{0.000, -1.000, 0.0000}},		//3 + 16
	{{-1.00000, 1.00000, -1.000000},{-1.000, 0.000, 0.0000}},		//4 + 16
	{{-1.00000, -1.00000, -1.000000},{0.000, 0.000, -1.0000}},		//5 + 16
	{{-1.00000, 1.00000, 1.000000},{0.000, 0.000, 1.0000}},		//6 + 16
	{{-1.00000, -1.00000, 1.000000},{0.000, -1.000, 0.0000}}		//7 + 16
};

const std::vector<uint32_t> indices = {
	4, 2, 0,
	10, 7, 3,
	6, 5, 15,
	1, 23, 13,
	8, 11, 9,
	12, 17, 21,

	4, 14, 2,
	10, 22, 7,
	6, 20, 5,
	1, 19, 23,
	8, 18, 11,
	12, 16, 17

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
	Camera camera;

	void initWindow() {
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);

		window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, APPLICATION_NAME, NULL, NULL);
		if (window == NULL) {
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
		}

		glfwMakeContextCurrent(window);

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
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glGenVertexArrays(1, &VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices.front(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), &indices.front(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(glm::vec3));
		glEnableVertexAttribArray(1);

		glEnable(GL_DEPTH_TEST);
		
		shader.init("basic.vert", "basic.frag");
		// TEMPORAL STUFF HERE
		camera.Position = glm::vec3(0.f, 0.f, 5.f);
		memset(frameTime, 0, sizeof(frameTime));
		cubeMaterial = material::BRONZE;

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
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 model = glm::mat4(1.f);

		processInput();
		model = glm::translate(model, cubePosition);

		shader.use();
		shader.setMat4("projection", projection);
		shader.setMat4("view", view);
		shader.setMat4("model", model);

		shader.setVec3("material.ambient", cubeMaterial.ambient);
		shader.setVec3("material.diffuse", cubeMaterial.diffuse);
		shader.setVec3("material.specular", cubeMaterial.specular);
		shader.setFloat("material.shininess", (cubeMaterial.shininess*128));

		shader.setVec3("viewPos", camera.Position);

		shader.setVec3("light.direction", light.direction);
		shader.setVec3("light.ambient", light.ambient);
		shader.setVec3("light.diffuse", light.diffuse);
		shader.setVec3("light.specular", light.specular);

		if (wireVisible)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		

	}

	void drawGUI() {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		materialEditorGUI();
		debugInfoGUI();

		ImGui::Begin("Rest");
		ImGui::Text("Cube");
		ImGui::SliderFloat3("cubePosition", (float*)&cubePosition, -2.f, 2.f);
		
		

		ImGui::Text("Light");
		ImGui::SliderFloat3("Direction", (float*)&light.direction, -1.f, 1.f);
		ImGui::SliderFloat3("Light Ambient", (float*)&light.ambient, 0.f, 1.f);
		ImGui::SliderFloat3("Light Diffuse", (float*)&light.diffuse, 0.f, 1.f);
		ImGui::SliderFloat3("Light Specular", (float*)&light.specular, 0.f, 1.f);
		ImGui::End();

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

	void processInput() {
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			cubePosition.z -= 1.f * deltaTime;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			cubePosition.z += 1.f * deltaTime;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			cubePosition.x -= 1.f * deltaTime;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			cubePosition.x += 1.f * deltaTime;
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
			cubePosition.y += 1.f * deltaTime;
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
			cubePosition.y -= 1.f * deltaTime;	
	}

	void materialEditorGUI() {
		//--------------------------------CUBE EDITOR--------------------------------
		static char stringBuffer[15] = { 0 };

		ImGui::Begin("Material Editor");
		
		ImGui::Text("Material");
		ImGui::InputText("Name", stringBuffer, IM_ARRAYSIZE(stringBuffer));
		
		if (ImGui::Button("Save"))
			material::SaveMaterial(cubeMaterial, stringBuffer);
		ImGui::SameLine();
		if(ImGui::Button("Load"))
			material::LoadMaterial(cubeMaterial, stringBuffer);
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
};

int main() {
	VoxelGameEngine app;
	app.run();
}
