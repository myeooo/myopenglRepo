#include "Window.h"

Window::Window()
{
	width = 800;
	height = 600;
}

Window::Window(GLint windowWidth, GLint WindowHeight): height(WindowHeight), width(windowWidth)
{
}

int Window::Initialise()
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
	mainWindow = glfwCreateWindow(width, height, "meo meo", NULL, NULL);
	if (!mainWindow) {
		printf("GLFW window creation failed");
		glfwTerminate();
		return 1;
	}

	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);
	glfwMakeContextCurrent(mainWindow);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		printf("GLEW initialisation failed!");
		
		return 1;

	}
	glEnable(GL_DEPTH_TEST);

	glViewport(0, 0, bufferWidth, bufferHeight);
}

Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}
