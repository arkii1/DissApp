#pragma once

#include <GL\glew.h>

class Material
{
public:
	Material();
	Material(GLfloat sIntensity, GLfloat shine);

	void UseMaterial(GLuint specularIntensityLocation, GLuint shininessLocation);

private:

	GLfloat specularIntensity, shininess;
};
