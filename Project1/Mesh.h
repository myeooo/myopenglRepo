#pragma once
#include "GL/glew.h"
class Mesh
{
public:
	Mesh();
	virtual ~Mesh();
	void ClearMesh();

	void CreateMesh(GLfloat *vertices, unsigned int * indices, unsigned int numOfVertices,unsigned int numberOfindices);
	void RenderMesh();
private:
	GLuint VAO, VBO, IBO;
	GLsizei indexCount;

};


