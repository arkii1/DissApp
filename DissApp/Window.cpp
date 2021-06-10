#include "Window.h"

Window::Window()
{
	windowWidth = 1920;
	windowHeight = 1080;

	for (size_t i = 0; i < 1024; i++)
	{
		hasKeyBeenPressed[i] = 0;
	}

	mouseXChange = 0.0f;
	yChange = 0.0f;
	hasMouseMovedYet = false;
}

int Window::Initialise()
{
	if (!glfwInit())
	{
		printf("GLFW initialisation failed!");
		glfwTerminate();
		return 1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	mainWindow = glfwCreateWindow(windowWidth, windowHeight, "Test Window", NULL, NULL);
	if (!mainWindow)
	{
		printf("GLFW window creation failed");
		glfwTerminate();
		return 1;
	}

	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	glfwMakeContextCurrent(mainWindow);

	CreateInputCallbacks();

	glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("GLEW initialisation failed!");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST);

	glViewport(0, 0, bufferWidth, bufferHeight);

	glfwSetWindowUserPointer(mainWindow, this);

	return 0;
}

void Window::CreateInputCallbacks()
{
	glfwSetKeyCallback(mainWindow, HandleKeyInput);
	glfwSetCursorPosCallback(mainWindow, HandleMouseInput);
}

void Window::HandleKeyInput(GLFWwindow* window, int key, int code, int action, int mode)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->hasKeyBeenPressed[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->hasKeyBeenPressed[key] = false;
		}
	}

	if (theWindow->hasKeyBeenPressed[GLFW_KEY_ESCAPE])
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}

void Window::HandleMouseInput(GLFWwindow* window, double xPos, double yPos)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (!theWindow->hasMouseMovedYet)
	{
		theWindow->lastMousePosX = xPos;
		theWindow->lastMousePosY = yPos;
		theWindow->hasMouseMovedYet = true;
	}

	theWindow->mouseXChange = xPos - theWindow->lastMousePosX;
	theWindow->yChange = theWindow->lastMousePosY - yPos;

	theWindow->lastMousePosX = xPos;
	theWindow->lastMousePosY = yPos;
}

GLfloat Window::GetXChange()
{
	GLfloat change = mouseXChange;
	mouseXChange = 0.0f;
	return change;
}

GLfloat Window::GetYChange()
{
	GLfloat change = yChange;
	yChange = 0.0f;
	return change;
}

Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}