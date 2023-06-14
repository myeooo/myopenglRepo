#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include "GL/glew.h"
#include <iostream>
#include <fstream>
#include "GLFW/glfw3.h"
#include <CMath>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Windows.h>
#include "Window.h"
#include <vector>
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"

Camera camera;
const GLint WIDTH = 800, HEIGHT = 600;
const float toRadians = 3.14159265f / 180.0f;
std::vector<Mesh*> meshList;
std::vector<Shader*> shaderList;
Window mainWindow;
bool direction = true;
float triOffset = 0.0f;
float triMaxoffset = 0.7f;
float triIncrement = 0.05f;

bool zoomIn = true;
float curSize = 0.4f;
float zoomRate = 0.0001f;
float maxSize = 0.8f;
float minSize = 0.1f;

static const char* vShader = "Shaders/shader.vert";


//fragment
static const char* fShader = "Shaders/shader.frag";
GLfloat vertices[] = {
		-1.0, -1.0f, 0.0f,
		0.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, -1.0f, -1.0f
};

void CreateShader() {
	Shader* shader1 = new Shader();
	shader1->CreateFromFile(vShader, fShader);
	shaderList.push_back(shader1);
}
void createObject(GLfloat vertices[]) {
	unsigned int indices[] = {
		0,3,1,
		1,3,2,
		0,1,2,
		0,4,2,
		0,3,4,
		2,3,4
	};

	Mesh* mesh = new Mesh();
	mesh->CreateMesh(vertices, indices, 15, 18);
	meshList.push_back(mesh);
	std::cout << std::endl;
}


int main()
{
	SYSTEMTIME st;
	GetSystemTime(&st);
	static int FPS = 120;
	unsigned long long milisecPerFrame = 1000 / FPS;
	unsigned long long startTime = st.wSecond * 1000 + st.wMilliseconds;
	GetSystemTime(&st);
	unsigned long long endTime;
	mainWindow = Window(800, 600);
	mainWindow.Initialise();
	createObject(vertices);
	createObject(vertices);
	CreateShader();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f),
		-20.0f, 0.0f,5.0f, 0.5f);

	GLuint uniformModel = 0, uniformProjection = 0, uniformView = 0;

	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / (GLfloat)mainWindow.getBufferHeight(), 0.1f, 100.0f);
	int count = 0;

	while (!mainWindow.getShouldClose())
	{
		count++;
		GetSystemTime(&st);
		endTime = st.wSecond * 1000 + st.wMilliseconds;
		long long loopTime = milisecPerFrame - (endTime - startTime);
		if (loopTime > 0) {
			Sleep(loopTime);
		}

		GetSystemTime(&st);
		startTime = st.wSecond * 1000 + st.wMilliseconds;
		glfwPollEvents();
		camera.keyControll(mainWindow.getsKeys());
		camera.mouseControl(mainWindow.getXchange(), mainWindow.getYchange());
		static float rotation = 0 * toRadians;
		rotation += 6 * toRadians;
		if (abs(triOffset) >= triMaxoffset) {
			direction = !direction;
		}
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		uniformModel = shaderList[0]->GetModelLocation();
		uniformProjection = shaderList[0]->GetProjectionLocation();
		uniformView = shaderList[0]->GetViewLocation();

		shaderList[0]->UseShader();
		glm::mat4 model;

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0, 1.0, -2.5));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		meshList[0]->RenderMesh();	

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0, -1.0f, -2.5));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		meshList[1]->RenderMesh();

		glUseProgram(0);
		mainWindow.swapBuffer();
	}
	return 0;
}