#pragma once

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window
{
private:
	GLFWwindow* mainWindow;
	GLint width, height;
	GLint bufferWidth, bufferHeight;
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);
	~Window();

	int Initialize();
	inline void SwapBuffers() const { glfwSwapBuffers(mainWindow); };

	inline GLfloat GetBufferWidth() const { return bufferWidth; };
	inline GLfloat GetBufferHeight() const { return bufferHeight; };
	inline bool GetShouldClose() const { return glfwWindowShouldClose(mainWindow); };
};

