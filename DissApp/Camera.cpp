#include "Camera.h"

Camera::Camera()
{
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	up = worldUp;
	yaw = 0.0f;
	pitch = 0.0f;
	front = glm::vec3(0.0f, 0.0f, -1.0f);

	moveSpeed = 5.0f;
	turnSpeed = 1.0f;

	UpdateLocalVectors();
}

Camera::Camera(glm::vec3 _position, glm::vec3 _up, GLfloat _yaw, GLfloat _pitch, GLfloat _moveSpeed, GLfloat _turnSpeed)
{
	position = _position;
	worldUp = _up;
	up = worldUp;
	yaw = _yaw;
	pitch = _pitch;
	front = glm::vec3(0.0f, 0.0f, -1.0f);

	moveSpeed = _moveSpeed;
	turnSpeed = _turnSpeed;

	UpdateLocalVectors();
}

void Camera::HandleKeyMovement(bool* keys, GLfloat deltaTime) 
{
	if (keys[GLFW_KEY_W])
	{
		position += front * moveSpeed * deltaTime;
	}
	
	if (keys[GLFW_KEY_S])
	{
		position -= front * moveSpeed * deltaTime;
	}

	if (keys[GLFW_KEY_A])
	{
		position -= right * moveSpeed * deltaTime;
	}

	if (keys[GLFW_KEY_D])
	{
		position += right * moveSpeed * deltaTime;
	}
}

void Camera::HandleMouseMovement(GLfloat xChange, GLfloat yChange)
{
	xChange *= turnSpeed;
	yChange *= turnSpeed;

	yaw += xChange;
	pitch += yChange;

	if (pitch > 89.0f)
	{
		pitch = 89.0f;
	}

	if (pitch < -89.0f)
	{
		pitch = -89.0f;
	}

	UpdateLocalVectors();
}

glm::mat4 Camera::CalculateViewMatrix()
{
	return glm::lookAt(position, position + front, up);
}

void Camera::UpdateLocalVectors()
{
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	glm::normalize(front);

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}