#include "Window.h"

void Window::HandleKeys(GLFWwindow* window, int key, int code, int action, int mode)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
	
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
		return;
	}

	if (key >= 0 && key <= 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
			return;
		}

		if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
			return;
		}
	}
}

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
	theWindow->yChange = theWindow->lastY - yPos; // Remove inverted camera movement

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;
}

void Window::CreateCallbacks()
{
	glfwSetKeyCallback(mainWindow, HandleKeys);
	glfwSetCursorPosCallback(mainWindow, HandleMouse);
}

Window::Window()
	: width(800), height(600), mainWindow(nullptr), bufferHeight(0), bufferWidth(0), keys{ false }
{
}

Window::Window(GLint windowWidth, GLint windowHeight)
	: width(windowWidth), height(windowHeight), mainWindow(nullptr), bufferHeight(0), bufferWidth(0), keys{ false }
{
}

Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}

int Window::Initialize()
{
	// Initialize GLFW
	if (!glfwInit())
	{
		std::cout << "GLFW initialization failed!" << std::endl;
		glfwTerminate();
		return 1;
	}

	// Setup GLFW window properties
	// OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Core profile = no backwards compatibility
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Allow forward compatibility
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Create window
	mainWindow = glfwCreateWindow(width, height, "Test Window", NULL, NULL);
	if (!mainWindow)
	{
		std::cout << "GLFW Window creation failed!" << std::endl;
		glfwTerminate();
		return 1;
	}

	// Get buffer (OpenGL viewport) size information
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	// Set context for GLEW to use
	glfwMakeContextCurrent(mainWindow);

	// Handle key and mouse input
	CreateCallbacks();

	// Lock and hide mouse pointer to opened window
	glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Allow modern extension features
	glewExperimental = GL_TRUE;	

	// Initialize GLEW
	if (glewInit() != GLEW_OK)
	{
		std::cout << "GLFW initialization failed!" << std::endl;
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	// Enable depth testing to see what's in front
	glEnable(GL_DEPTH_TEST);

	// Setup viewport size
	glViewport(0, 0, bufferWidth, bufferHeight);

	// Mark this pointer as user of mainWindow
	glfwSetWindowUserPointer(mainWindow, this);
}

GLfloat Window::GetXChange()
{
	GLfloat change = xChange;
	xChange = 0;

	return change;
}

GLfloat Window::GetYChange()
{
	GLfloat change = yChange;
	yChange = 0;

	return change;
}
