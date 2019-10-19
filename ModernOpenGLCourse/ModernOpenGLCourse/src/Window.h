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

	bool keys[1024]; // Range of key characters (key pressed)
	GLfloat lastX, lastY, xChange, yChange; // Mouse movement helpers
	bool mouseFirstMoved;


	static void HandleKeys(GLFWwindow* window, int key, int code, int action, int mode);
	static void HandleMouse(GLFWwindow* window, double xPos, double yPos);
	void CreateCallbacks();
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);
	~Window();

	int Initialize();
	inline void SwapBuffers() const { glfwSwapBuffers(mainWindow); };

	inline GLfloat GetBufferWidth() const { return bufferWidth; };
	inline GLfloat GetBufferHeight() const { return bufferHeight; };
	inline bool GetShouldClose() const { return glfwWindowShouldClose(mainWindow); };
	inline bool* GetKeys() { return keys; };
	GLfloat GetXChange();
	GLfloat GetYChange();
};

