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
	bool* getsKeys(){
		return keys;
	}

	GLfloat getXchange();
	GLfloat getYchange();

	void swapBuffer() { return glfwSwapBuffers(mainWindow); }
	~Window();
private:
	GLFWwindow* mainWindow;
	
	GLint width, height;
	GLint bufferWidth, bufferHeight;

	bool keys[1024];

	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	bool mouseFirstMoved;

	void createCallBack();
	static void handleKeys(GLFWwindow * window, int key, int code, int action , int mode );
	static void handleMouse(GLFWwindow* window, double xPos, double yPos);
};

