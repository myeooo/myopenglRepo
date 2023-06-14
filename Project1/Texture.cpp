#include "Texture.h"

Texture::Texture(char* fileLoc)
{
	textureId = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = fileLoc;
}

Texture::~Texture()
{
}

Texture::Texture()
{
	textureId = 0;
	width = 0;
	height = 0; 
	bitDepth = 0;
}

void Texture::LoadTexture()
{
	unsigned char* textData = stbi_load(fileLocation, &width, &height, &bitDepth, 0);
	if (!textData) {
		printf("failed to load file: %s", fileLocation);
		return;
	}
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);


}
