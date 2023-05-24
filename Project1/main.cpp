#include <stdio.h>
#include <string.h>
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <CMath>
#include <glm/mat4x4.hpp>

const GLint WIDTH = 800, HEIGHT = 600;
GLuint VAO, VBO, shader, uniformXMove;

bool direction = true;
float triOffset = 0.0f;
float triMaxoffset = 0.7f;
float triIncrement = 0.0005f;



static const char* vShader = "\n\
#version 330						\n\
									\n\
layout (location = 0) in vec3 pos;	\n\
uniform float xMove;				\n\
void main(){											\n\
	gl_Position = vec4(0.4*pos.x + xMove ,0.4* pos.y, 0.4* pos.z, 1.0);		\n\
}"; 


//fragment
static const char* fShader = "							\n\
#version 330											\n\
														\n\
out vec4 colour;										\n\
void main(){											\n\
	colour = vec4(1.0, 0.0, 0.0, 1.0);					\n\
}";
void createTriangle() {
	GLfloat vertices[] = {
		-1.0,-1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f,1.0f,0.0f
	};
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
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
	uniformXMove = glGetUniformLocation(shader, "xMove");
}


int main()
{
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
	glViewport(0, 0, bufferWidth, bufferHeight);
	createTriangle();
	compilerShader();

	while (!glfwWindowShouldClose(mainWindow)) 
	{
		glfwPollEvents();

		if (direction) {
			triOffset += triIncrement;
		}
		else {
			triOffset -= triIncrement;

		}

		if (abs(triOffset) >= triMaxoffset) {
			direction = !direction;
		}
		glClearColor(0.0f,0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shader);

		glUniform1f(uniformXMove,triOffset);

		glBindVertexArray(VAO);
		
		glDrawArrays(GL_TRIANGLES,0,3);

		glBindVertexArray(0);
		glUseProgram(0);
		glfwSwapBuffers(mainWindow);
	}
	return 0;
}