#pragma once

//Libraries
#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window
{
public:
	// Constructor for instantiating a window
	Window();

	// An initialiser for our window
	int Initialise();

	// A bool which checks if window needs to be closed
	bool GetShouldWindowClose() { return glfwWindowShouldClose(mainWindow); }

	// For swapping our viewable frame with our drawing frame
	void SwapBuffers() { glfwSwapBuffers(mainWindow); }

	// Accessor method for buffer resolution
	GLfloat GetAspectRatio() { return (GLfloat)bufferWidth / (GLfloat)bufferHeight; };

	// Accessors for inputs
	bool* GetKeys() { return keys; };
	GLfloat GetXChange();
	GLfloat GetYChange();

	// A destructor for when we want to destroy our window
	~Window();

private:

	// Pointer to our main window
	GLFWwindow* mainWindow;

	// Width and height of our window
	GLint width, height;

	// Width and height of our buffer
	GLint bufferWidth, bufferHeight;

	// Create an array of bools. Will store whether each key on keyboard has been pressed or not
	bool keys[1024]; 

	GLfloat lastX, lastY, xChange, yChange; 

	bool mouseFirstMoved;

	void CreateCallbacks();
	static void HandleKeys(GLFWwindow* window, int key, int code, int action, int mode);
	static void HandleMouse(GLFWwindow* window, double xPos, double yPos);
};

