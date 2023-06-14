#pragma once

#include <GL/glew.h>
#include "stb_image.h"

class Texture
{
public:
	Texture(char * fileLoc);
	~Texture();
	Texture();
	void LoadTexture();

private:
	GLuint textureId;
	int width, height, bitDepth;
	char* fileLocation;
};

