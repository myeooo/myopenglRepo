#include <stdio.h>
#include <string.h>
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <CMath>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Windows.h>
#include <iostream>
#include <vector>
#include "Mesh.h"
const GLint WIDTH = 800, HEIGHT = 600;
const float toRadians = 3.14159265f / 180.0f;
std::vector<Mesh*> meshList;
GLuint shader, uniformModel, uniformProjection;

bool direction = true;
float triOffset = 0.0f;
float triMaxoffset = 0.7f;
float triIncrement = 0.05f;

bool zoomIn = true;
float curSize = 0.4f;
float zoomRate = 0.0001f;
float maxSize = 0.8f;
float minSize = 0.1f;

static const char* vShader = "\n\
#version 330						\n\
layout (location = 0) in vec3 pos;	\n\
									\n\
out vec4 vCol;								\n\
									\n\
uniform mat4 model;					\n\
// how camera move									\n\
uniform mat4 projection;				\n\
									\n\
void main(){											\n\
	gl_Position = projection*model* vec4(pos, 1.0f);		\n\
	vCol = vec4(clamp(pos, 0.0f, 1.0f),1.0f);		\n\
}"; 


//fragment
static const char* fShader = "							\n\
#version 330											\n\
														\n\
in vec4 vCol;												\n\
														\n\
out vec4 colour;										\n\
void main(){											\n\
	colour = vCol;							\n\
}";
GLfloat vertices[] = {
		-1.0,-1.0f, 0.0f,
		0.0f,-1.0f,1.0f,
		1.0f, -1.0f, 0.0f,
		0.0f,1.0f,0.0f
};
GLfloat vertices2[] = {
	-1.0,1, 0.0f,
	0.0f,1,1.0f,
	1.0f, 1, 0.0f,
	0.0f,2,0.0f
};
void createTriangle(GLfloat vertices[]) {
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
void addShader(GLuint theProgram, const char* shaderCode, GLenum shaderType) 
{
	GLuint theShader = glCreateShader(shaderType);

	const GLchar* theCode[1];
	theCode[0] = shaderCode;

	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);

	glShaderSource(theShader, 1, theCode, codeLength);
	glCompileShader(theShader);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(theShader, 1024, NULL, eLog);
		fprintf(stderr, "Error compiling the %d shader: '%s'\n", shaderType, eLog);
		return;
	}

	glAttachShader(theProgram, theShader);
}
 
void compilerShader() {

	shader = glCreateProgram();

	if (!shader) {
		printf("error creating shader program!");
		return;
	}
	addShader(shader, vShader, GL_VERTEX_SHADER);
	addShader(shader, fShader, GL_FRAGMENT_SHADER);
	GLint result = 0;
	GLchar elog[1024] = {0};
	glLinkProgram(shader);
	glGetProgramiv(shader, GL_LINK_STATUS,&result);
	if (!result) {
		glGetProgramInfoLog(shader, sizeof(elog), NULL, elog);
		printf("error linking program: '%s'\n",elog);
		return;
	}

	glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(shader, sizeof(elog), NULL, elog);
		printf("error validate program: '%s'\n", elog);
		return;
	}

	uniformModel = glGetUniformLocation(shader, "model");
	uniformProjection = glGetUniformLocation(shader, "projection"); 

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
	createTriangle(vertices);
	createTriangle(vertices2);
	compilerShader();

	glm::mat4 projection = glm::perspective(45.0f,(GLfloat)bufferWidth/ (GLfloat)bufferHeight,0.1f,100.0f);

	while (!glfwWindowShouldClose(mainWindow)) 
	{
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
		glUseProgram(shader);

		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(triOffset, 0, -2.5f));
//		model = glm::rotate(model, rotation, glm::vec3(0,0,0));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f,1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		for (Mesh* mesh : meshList) {
			mesh->RenderMesh();
		}
		
		glUseProgram(0);
		glfwSwapBuffers(mainWindow);
	}
	return 0;
}