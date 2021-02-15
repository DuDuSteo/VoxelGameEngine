#pragma warning(disable : 26495)
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <iostream>
#include <vector>

#include "shader.h"
#include "camera.h"

#define SCR_WIDTH 1280
#define SCR_HEIGHT 720
#define APPLICATION_NAME "VoxelGameEngine"

//Timings
float currentFrame = 0;
float deltaTime = 0;
float lastFrame = 0;


//temporal
bool wireVisible = false;
glm::vec3 cubePosition = glm::vec3(0.f, 0.f, -5.f);

struct Vertex {
	glm::vec3 pos;
	//glm::vec3 normals;
	//glm::vec2 texcoord;
};

const std::vector<Vertex> vertices = {
	{{-0.2f, -0.2f, -0.2f}},			//0
	{{-0.2f, -0.2f,	 0.2f}},			//1
	{{-0.2f,  0.2f, -0.2f}},			//2
	{{-0.2f,  0.2f,  0.2f}},			//3
	{{ 0.2f, -0.2f, -0.2f}},			//4
	{{ 0.2f, -0.2f,  0.2f}},			//5
	{{ 0.2f,  0.2f, -0.2f}},			//6
	{{ 0.2f,  0.2f,  0.2f}}				//7
};

const std::vector<uint32_t> indices = {
	0, 1, 2,
	1, 3, 2,
	1, 5, 7,
	1, 7, 3,
	1, 5, 4,
	1, 4, 7,
	5, 4, 6,
	5, 6, 7,
	4, 0, 6,
	0, 2, 6,
	3, 7, 6,
	3, 6, 2
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

		glEnable(GL_DEPTH_TEST);
		
		shader.init("basic.vert", "basic.frag");
	}
	void mainLoop() {
		while (!glfwWindowShouldClose(window)) {
			glfwPollEvents();
			drawFrame();
			glfwSwapBuffers(window);
		}
	}
	void drawFrame() {
		currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 projection = glm::perspective(glm::radians(45.f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.f);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 model = glm::mat4(1.f);

		processInput();
		//std::cout << "x: " << cubePosition.x << " y: " << cubePosition.z << std::endl;
		std::cout << deltaTime << std::endl;
		model = glm::translate(model, cubePosition);

		shader.use();
		shader.setMat4("projection", projection);
		shader.setMat4("view", view);
		shader.setMat4("model", model);
		if (wireVisible)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

	}
	void cleanup() {
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
		glDeleteBuffers(1, &VAO);
		glfwDestroyWindow(window);
		glfwTerminate();
	}
	void processInput()
	{
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

		if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
				wireVisible = !wireVisible;		
	}

};

int main() {
	VoxelGameEngine app;
	app.run();
}
