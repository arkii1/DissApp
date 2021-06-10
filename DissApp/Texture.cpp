#include "Texture.h"

Texture::Texture()
{
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = (char*)("");
}
Texture::Texture(char* fileLoc)
{
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = fileLoc;
}

void Texture::LoadTexture()
{
	// Loads our texture and sets to texData. texData only here to check if it actually loaded. Also sets width, height & bitDepth by using the texture image
	unsigned char *texData = stbi_load(fileLocation, &width, &height, &bitDepth, 0);
	if (!texData) //Null check
	{
		printf("Failed to find: %s\n", fileLocation);
		return;
	}

	// Similarly to buffer objects, we generate a texture and bind it
	glGenTextures(1, &textureID);
	// Then bind, just like VBO and VAO
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Set wrapping parameters. S and T are essentially 2 different axis of a texture.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Set magnification and minification filters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Set up image data from texData to our bound texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
	// Generate Mipmap
	glGenerateMipmap(GL_TEXTURE_2D);

	// Unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);

	// Freeing up memory for stbi so the program can be used again
	stbi_image_free(texData);
}

// Applies texture
void Texture::UseTexture()
{
	// Similar to binding. A GPU should have at least 16 texture units, where each unit contains a different texture. We are telling our GPU we want to use it, similarly to binding a VAO or VBO
	glActiveTexture(GL_TEXTURE0);
	// Binds our texture to the currently bound texture unit, which we chose above
	glBindTexture(GL_TEXTURE_2D, textureID);
}

// Clears texture from gpu and resets all values
void Texture::ClearTexture()
{
	glDeleteTextures(1, &textureID);
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = 0;
}

// Destructor
Texture::~Texture()
{
	ClearTexture();
}

