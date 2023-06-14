#pragma once
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include "GL/glew.h"
class Shader
{
public: 
	Shader();
	void CreateFromString(const char* vertexCode , const char* fragmentCode);
	void CreateFromFile(const char* vertexLocation, const char* fragmentFragment);
	std::string ReadFile(const char* fileLocation);
	GLuint GetProjectionLocation();
	GLuint GetModelLocation();
	GLuint GetViewLocation();

	void UseShader();
	void ClearShader();
	~Shader();
private:
	GLuint shaderID, uniformProjection,uniformModel, uniformView;
	void CompileShader(const char* vertexCode, const char* fragmentCode);
	void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);

};

