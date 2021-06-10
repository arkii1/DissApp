#include "Window.h"

// Constructor for window - sets resolution to 800x600
Window::Window()
{
	width = 1920;
	height = 1080;

	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}

	xChange = 0.0f;
	yChange = 0.0f;
	mouseFirstMoved = true;
}

int Window::Initialise()
{
	// Initlaise GLFW. Returns with error if fails
	if (!glfwInit())
	{
		printf("GLFW initialisation failed!");
		glfwTerminate();
		return 1;
	}

	// Set OpenGL version to 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // 3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // .3
	// Core profile = No OpenGL backwards compatability
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Allows OpenGL forward compatability
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Create window. Returns with error if fails
	mainWindow = glfwCreateWindow(width, height, "Test Window", NULL, NULL);
	if (!mainWindow)
	{
		printf("GLFW window creation failed");
		glfwTerminate();
		return 1;
	}


	// Get Frame Buffer Size information. A frame buffer is a portion of RAM containing a bitmap used for video display
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	// Let GLFW know the window OpenGL will draw on
	glfwMakeContextCurrent(mainWindow);

	// Create input callbacks
	CreateCallbacks();
	// Disables cursor
	glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	// Allow modern library extension features
	glewExperimental = GL_TRUE;

	//Initialise GLEW. Returns with an error if fails
	if (glewInit() != GLEW_OK)
	{
		printf("GLEW initialisation failed!");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	// Enabling depth test
	glEnable(GL_DEPTH_TEST);

	// Setup viewport size. "Sets up the size of the part we are drawing to on our window"
	glViewport(0, 0, bufferWidth, bufferHeight);

	// For camera controller - sets window for our input
	glfwSetWindowUserPointer(mainWindow, this);

	// Note that we often return 0 when no errors occur, and 1 when errors do. main::main() works this way as well
	return 0;
}

// When a key is pressed in mainWindow, HandleKeys is called. Same logic with mouse
void Window::CreateCallbacks()
{
	glfwSetKeyCallback(mainWindow, HandleKeys);
	glfwSetCursorPosCallback(mainWindow, HandleMouse);
}

// Uses key and whether it's been pressed or not to set keys array index to true/false
void Window::HandleKeys(GLFWwindow* window, int key, int code, int action, int mode)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
		}
	}

	if (theWindow->keys[GLFW_KEY_ESCAPE])
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}

// Calculates change in mouse position
void Window::HandleMouse(GLFWwindow* window, double xPos, double yPos)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (theWindow->mouseFirstMoved)
	{
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMoved = false;
	}

	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos;

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;
}

GLfloat Window::GetXChange()
{
	GLfloat change = xChange;
	xChange = 0.0f;
	return change;
}

GLfloat Window::GetYChange()
{
	GLfloat change = yChange;
	yChange = 0.0f;
	return change;
}

// Destroys window
Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}