#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

class Camera
{
public:
	Camera();
	Camera(glm::vec3 _position, glm::vec3 _worldUp, GLfloat _yaw, GLfloat _pitch, GLfloat _moveSpeed, GLfloat _turnSpeed);
	
	void HandleKeyMovement(bool* keys, GLfloat deltaTime);

	void HandleMouseMovement(GLfloat xChange, GLfloat yChange);

	glm::mat4 CalculateViewMatrix();

	float GetYaw() { return yaw; };
	float GetPitch() { return pitch; };
	glm::vec3 GetPosition() { return position; };

private:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	GLfloat yaw;
	GLfloat pitch;

	GLfloat moveSpeed;
	GLfloat turnSpeed;

	void UpdateLocalVectors();
};

