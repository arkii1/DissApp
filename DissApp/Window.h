#pragma once

#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window
{
public:
	Window();

	int Initialise();

	bool GetShouldWindowClose() { return glfwWindowShouldClose(mainWindow); }

	void SwapBuffers() { glfwSwapBuffers(mainWindow); }

	GLfloat GetAspectRatio() { return (GLfloat)bufferWidth / (GLfloat)bufferHeight; };

	bool* GetHasKeyBeenPressed() { return hasKeyBeenPressed; };
	GLfloat GetXChange();
	GLfloat GetYChange();

	~Window();

private:

	GLFWwindow* mainWindow;

	GLint windowWidth, windowHeight;

	GLint bufferWidth, bufferHeight;

	bool hasKeyBeenPressed[1024]; 

	GLfloat lastMousePosX, lastMousePosY, mouseXChange, yChange; 

	bool hasMouseMovedYet;

	void CreateInputCallbacks();

	static void HandleKeyInput(GLFWwindow* window, int key, int code, int action, int mode);
	static void HandleMouseInput(GLFWwindow* window, double xPos, double yPos);
};

