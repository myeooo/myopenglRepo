#pragma once
#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint WindowHeight);
	int Initialise();
	GLfloat getBufferWidth() {
		return bufferWidth;
	}
	GLfloat getBufferHeight() {
		return bufferHeight;
	}
	bool getShouldClose() { return glfwWindowShouldClose(mainWindow); }
	void swapBuffer() { return glfwSwapBuffers(mainWindow); }
	~Window();
private:
	GLFWwindow* mainWindow;
	GLint bufferWidth, bufferHeight;
	GLint width, height;
};

