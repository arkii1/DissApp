#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <GL/glew.h>

class Shader
{
public:

	Shader();

	void CreateFromFiles(const char* vertexLocation, const char* fragmentLocation);

	std::string ReadFile(const char* fileLocation);

	GLuint GetUniformModelTransformLocation() { return uniformModelTransform; };
	GLuint GetUniformProjection() { return uniformProjection; };
	GLuint GetUniformAmbientIntesityLocation() { return uniformAmbientIntensity; };
	GLuint GetUniformAmbientColourLocation() { return uniformAmbientColour; };
	GLuint GetUniformDiffuseIntensityLocation() { return uniformDiffuseIntensity; };
	GLuint GetUniformDirectionLocation() { return uniformDirection; };
	GLuint GetUniformSpecularIntensityLocation() { return uniformSpecularIntensity; };
	GLuint GetUniformShininessLocation() { return uniformShininess; };
	GLuint GetUniformViewLocation() { return uniformView; };
	GLuint GetUniformEyePositionLocation() { return uniformEyePosition; };

	void UseShader();

	void ClearShader();

	~Shader();
private:

	GLuint shaderID, uniformModelTransform, uniformProjection,
		uniformAmbientColour, uniformAmbientIntensity, uniformDirection, uniformDiffuseIntensity,
		uniformSpecularIntensity, uniformShininess,
		uniformView, uniformEyePosition;

	void CompileShader(const char* vertexCode, const char* fragmentCode);

	void AddShaderToProgram(GLuint theProgram, const char* shaderCode, GLenum shaderType);
};

