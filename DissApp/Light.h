#pragma once
//Libraries
#include <GL\glew.h>
#include <glm\glm.hpp>

class Light
{
public:
	// Constructors
	Light();
	Light(GLfloat red, GLfloat blue, GLfloat green, GLfloat aIntensity,
		GLfloat xDir, GLfloat yDir, GLfloat zDir, GLfloat dIntensity);

	// Tells GPU to use our light
	void UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColourLocation,
		GLfloat diffuseIntensityLocation, GLfloat directionLocation);

private:
	// Colour & direction of light
	glm::vec3 colour, direction;

	//Ambient & diffuse intensity of light
	GLfloat ambientIntensity, diffuseIntensity;
};