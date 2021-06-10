#include "Material.h"

// Constructor which assigns properties to default values
Material::Material()
{
	specularIntensity = 0.0f;
	shininess = 0.0f;
}

// Constructor which assignss properties to passed values
Material::Material(GLfloat sIntensity, GLfloat shine)
{
	specularIntensity = sIntensity;
	shininess = shine;
}

// Assigns uniform variables of our shader to the properties of this material
void Material::UseMaterial(GLuint specularIntensityLocation, GLuint shininessLocation)
{
	glUniform1f(specularIntensityLocation, specularIntensity);
	glUniform1f(shininessLocation, shininess);
}