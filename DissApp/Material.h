#pragma once

#include <GL\glew.h>

class Material
{
public:
	// Constructors 
	Material();
	Material(GLfloat sIntensity, GLfloat shine);

	// Tells GPU to use material
	void UseMaterial(GLuint specularIntensityLocation, GLuint shininessLocation);

private:
	// Properties of the material
	GLfloat specularIntensity;
	GLfloat shininess;
};
