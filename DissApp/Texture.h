#pragma once

//Libraries
#include <GL/glew.h>
#include "stb_image.h"

class Texture
{
public:
	Texture();
	Texture(char* fileLoc);

	void LoadTexture();

	void UseTexture();

	~Texture();

private:
	GLuint textureID;

	int textureWidth, textureHeight, bitDepth;

	char* fileLocation;
};

