#include "Light.h"

Light::Light()
{
	// Set our starting values to some defaults
	colour = glm::vec3(1.0f, 1.0f, 1.0f);
	ambientIntensity = 1.0f;

	direction = glm::vec3(1.0f, 0.0f, 0.0f);
	diffuseIntensity = 0.0f;
}

Light::Light(GLfloat red, GLfloat blue, GLfloat green, GLfloat aIntensity,
	GLfloat xDir, GLfloat yDir, GLfloat zDir, GLfloat dIntensity) //change to GLuint or unsigned int if bothered
{
	// Set private variables to given parameters
	colour = glm::vec3(red, green, blue);
	ambientIntensity = aIntensity;

	direction = glm::vec3(xDir, yDir, zDir);
	diffuseIntensity = dIntensity;
}

void Light::UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColourLocation,
	GLfloat diffuseIntensityLocation, GLfloat directionLocation)
{
	// Bind colour to uniform variable
	glUniform3f(ambientColourLocation, colour.x, colour.y, colour.z);
	// Bind ambient intesntiy to uniform variable
	glUniform1f(ambientIntensityLocation, ambientIntensity);
	// Bind direction to uniform variable
	glUniform3f(directionLocation, direction.x, direction.y, direction.z);
	// Bind diffuse intensity to uniform variable
	glUniform1f(diffuseIntensityLocation, diffuseIntensity);
}
