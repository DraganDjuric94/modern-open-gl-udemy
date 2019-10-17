#include <iostream>
#include <string>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader.h"

const float PI_F = 3.14159265358979f;
const float toRadians = PI_F / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader*> shaderList;

// Vertex shader
static const char* vShader = "src/shaders/shader.vert";
// Fragment shader
static const char* fShader = "src/shaders/shader.frag";

void CreateObjects()
{
	GLfloat vertices[] = {
		-1.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.5f
	};

	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	Mesh* meshOne = new Mesh();
	meshOne->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(meshOne);
	
	Mesh* meshTwo = new Mesh();
	meshTwo->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(meshTwo);
}

void CreateShaders()
{
	Shader* firstShader = new Shader();
	firstShader->CreateFromFile(vShader, fShader);
	shaderList.push_back(firstShader);
}

int main()
{
	mainWindow = Window(800, 600);
	mainWindow.Initialize();

	CreateObjects();
	CreateShaders();

	GLuint uniformProjection = 0, uniformModel = 0;
	// Create perspective projection matrix
	glm::mat4 projection = glm::perspective(45.0f, mainWindow.GetBufferWidth() / mainWindow.GetBufferHeight(), 0.1f, 100.0f);

	// Loop until window is closed
	while (!mainWindow.GetShouldClose())
	{
		// Get and handle user input events
		glfwPollEvents();

		// Clear window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		// Clear color buffer bit and clear depth buffer bit
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderList[0]->UseShader();
		uniformModel = shaderList[0]->GetModelLocation();
		uniformProjection = shaderList[0]->GetProjectionLocation();

		// Create identity matrix
		glm::mat4 model(1.0f);
		// Translate only x coordinate value
		model = glm::translate(model, glm::vec3(0.0f, -0.5f, -2.5f));
		// Rotate by curr angle degrees around z axis
		// model = glm::rotate(model, currentAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		// Scale x and y values to curr size percent
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));

		// Set our model matrix as uniform "model" value
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		// Set our projection matrix as uniform "projection" value
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		meshList[0]->RenderMesh();

		// Create identity matrix
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, -2.5f));
		// Rotate by curr angle degrees around z axis
		// model = glm::rotate(model, currentAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		// Scale x and y values to curr size percent
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));

		// Set our model matrix as uniform "model" value
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		// Set our projection matrix as uniform "projection" value
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		meshList[1]->RenderMesh();

		glUseProgram(0);

		// Swap to buffer that we have drawn to
		mainWindow.SwapBuffers();
	}

	return 0;
}