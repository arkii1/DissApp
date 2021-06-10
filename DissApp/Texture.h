#pragma once

//Libraries
#include <GL/glew.h>
#include "stb_image.h"

class Texture
{
public:
	// Constructors
	Texture();
	Texture(char* fileLoc);

	// Sets up the texture
	void LoadTexture();
	// Tells GPU to use texture
	void UseTexture();
	// Clears texture when not needed
	void ClearTexture();

	//Destructor
	~Texture();

private:
	//ID so we can access it 
	GLuint textureID;
	//Proerties of the texture image
	int width, height, bitDepth;
	//File location of the texture image 
	char* fileLocation;
};

