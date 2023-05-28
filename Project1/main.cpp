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

#include <vector>
#include "Mesh.h"
#include "Shader.h"
const GLint WIDTH = 800, HEIGHT = 600;
const float toRadians = 3.14159265f / 180.0f;
std::vector<Mesh*> meshList;
std::vector<Shader*> shaderList;

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
		-1.0,-1.0f, 0.0f,
		0.0f,-1.0f,1.0f,
		1.0f, -1.0f, 0.0f,
		0.0f,1.0f,0.0f
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
		2,3,0,
		0,1,2
	};
	
	Mesh* mesh = new Mesh();
	mesh->CreateMesh(vertices,indices , 12,12);
	meshList.push_back(mesh);
	std::cout <<std::endl;
	std::cout << "test mesh size";
	std::cout << meshList.size()<< std::endl;
}


int main()
{
	SYSTEMTIME st;
	GetSystemTime(&st);
	static int frameRate = 60;
	unsigned long long milisecPerFrame = 1000 / frameRate;
	unsigned long long startTime= st.wSecond * 1000 + st.wMilliseconds;
	GetSystemTime(&st);
	unsigned long long endTime ;
	if (!glfwInit()) {
		printf("GLFW install failed");
		glfwTerminate();
		return 1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "meo meo", NULL, NULL);
	if (!mainWindow) {
		printf("GLFW window creation failed");
		return 1;
	}
	
	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);
	glfwMakeContextCurrent(mainWindow);

	glewExperimental = GL_TRUE;
	if (glewInit()!= GLEW_OK) {
		printf("GLEW initialisation failed!");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;

	}
	glEnable(GL_DEPTH_TEST);

	glViewport(0, 0, bufferWidth, bufferHeight);
	createObject(vertices);
	createObject(vertices);
	CreateShader();
	GLuint uniformModel = 0, uniformProjection = 0;

	glm::mat4 projection = glm::perspective(45.0f,(GLfloat)bufferWidth/ (GLfloat)bufferHeight,0.1f,100.0f);
	int count = 0;
	
	while (!glfwWindowShouldClose(mainWindow)) 
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

		if (direction) {
			triOffset += triIncrement;
		}
		else {
			triOffset -= triIncrement;

		}

		if (zoomIn) {
			curSize += zoomRate;
		}
		else {
			curSize -= zoomRate;

		}

		if (curSize <= minSize||curSize >= maxSize) {
			zoomIn = !zoomIn;
		}
		

		static float rotation = 0 * toRadians;
		rotation +=2 * toRadians;
		if (abs(triOffset) >= triMaxoffset) {
			direction = !direction;
		}
		glClearColor(0.0f,0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		uniformModel = shaderList[0]->GetModelLocation();
		uniformProjection = shaderList[0]->GetProjectionLocation();
		shaderList[0]->UseShader();
		glm::mat4 model ;
		float test = 0.0f;
		float offset = 0.5f;
		for (Mesh* mesh : meshList) {
			
			offset = -offset;
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(0, offset, -3.0));
			model = glm::scale(model, glm::vec3(0.4f, 0.4f,1.0f));
			model = glm::rotate(model, rotation, glm::vec3(0.0f,1,0));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		
			mesh->RenderMesh();
			test += 1.0f;
		}
		
		glUseProgram(0);
		glfwSwapBuffers(mainWindow);
	}
	return 0;
}