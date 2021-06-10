#pragma once
// Libraries
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <GL/glew.h>

class Shader
{
public:

	// Constructor method
	Shader();

	// Create the shader from shader code
	void CreateFromFiles(const char* vertexLocation, const char* fragmentLocation);

	// Read the shader code
	std::string ReadFile(const char* fileLocation);

	// Accessor method for uniform variables
	GLuint GetUniformModel() { return uniformModel; };
	GLuint GetUniformProjection() { return uniformProjection; };
	GLuint GetUniformAmbientIntesityLocation() { return uniformAmbientIntensity; };
	GLuint GetUniformAmbientColourLocation() { return uniformAmbientColour; };
	GLuint GetUniformDiffuseIntensityLocation() { return uniformDiffuseIntensity; };
	GLuint GetUniformDirectionLocation() { return uniformDirection; };
	GLuint GetUniformSpecularIntensityLocation() { return uniformSpecularIntensity; };
	GLuint GetUniformShininessLocation() { return uniformShininess; };
	GLuint GetUniformViewLocation() { return uniformView; };
	GLuint GetUniformEyePositionLocation() { return uniformEyePosition; };


	// Called to use shader
	void UseShader();

	// Called the clear shader
	void ClearShader();

	// Destructor for the shader
	~Shader();
private:
	// Uniform variables
	GLuint shaderID, uniformModel, uniformProjection,
		uniformAmbientColour, uniformAmbientIntensity, uniformDirection, uniformDiffuseIntensity,
		uniformSpecularIntensity, uniformShininess,
		uniformView, uniformEyePosition;

	// Compiles the shader using shader code
	void CompileShader(const char* vertexCode, const char* fragmentCode);

	// Adds shader to the program
	void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
};

