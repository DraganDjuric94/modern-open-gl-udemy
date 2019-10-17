#include <iostream>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const float PI_F = 3.14159265358979f;

// Window dimensions
const GLint WIDTH = 800;
const GLint HEIGHT = 600;
const float toRadians = PI_F / 180.0f;

GLuint VAO, VBO, shader, uniformModel;

bool direction = true; // left = false, right = true
float triOffset = 0.0f;
float triMaxOffset = 0.7f;
float triIncrement = 0.0005f;

float currentAngle = 0.0f;

bool sizeDirection = true;
float currentSize = 0.4f;
float maxSize = 0.8f;
float minSize = 0.01f;

// Vertex shader
static const char* vShader = "											\n\
#version 330															\n\
																		\n\
layout (location = 0) in vec3 pos;										\n\
																		\n\
out vec4 vColor;														\n\
																		\n\
uniform mat4 model;														\n\
																		\n\
void main()																\n\
{																		\n\
	gl_Position = model * vec4(pos, 1.0);								\n\
	vColor = vec4(clamp(pos, 0.0, 1.0), 1.0);							\n\
}";

// Fragment shader
static const char* fShader = "											\n\
#version 330															\n\
																		\n\
in vec4 vColor;															\n\
																		\n\
out vec4 color;															\n\
																		\n\
void main()																\n\
{																		\n\
	color = vColor;														\n\
}";

void CreateTriangle()
{
	GLfloat vertices[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};

	// Create vertex array in Graphics RAM and store its ID in VAO
	glGenVertexArrays(1, &VAO);
	// Bind (use) vertex array with ID stored in VAO
	glBindVertexArray(VAO);

	// Generate vertex buffer and store its ID in VBO 
	glGenBuffers(1, &VBO);
	// Bind that buffer as array buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Bind data from vertices to vertex array
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	// Unbind vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind vertex array
	glBindVertexArray(0);
}

void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
	GLuint theShader = glCreateShader(shaderType);

	const GLchar* theCode[1];
	theCode[0] = shaderCode;

	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);

	glShaderSource(theShader, 1, theCode, codeLength);
	glCompileShader(theShader);

	int result;
	GLchar eLog[1024] = { 0 };
	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);

	if (!result)
	{
		glGetProgramInfoLog(theShader, sizeof(eLog), NULL, eLog);
		std::cout << "Error compiling the "<< shaderType << " shader: " << eLog << std::endl;
		return;
	}

	glAttachShader(theProgram, theShader);
}

void CompileShaders()
{
	shader = glCreateProgram();

	if (!shader)
	{
		std::cout << "Error creating shader program!" << std::endl;
		return;
	}

	AddShader(shader, vShader, GL_VERTEX_SHADER);
	AddShader(shader, fShader, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glLinkProgram(shader);
	glGetProgramiv(shader, GL_LINK_STATUS, &result);

	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		std::cout << "Error linking program: " << eLog << std::endl;
		return;
	}

	glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);

	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		std::cout << "Error validating program: " << eLog << std::endl;
		return;
	}

	uniformModel = glGetUniformLocation(shader, "model");
}

int main()
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
	GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Test Window", NULL, NULL);
	if (!mainWindow)
	{
		std::cout << "GLFW Window creation failed!" << std::endl;
		glfwTerminate();
		return 1;
	}

	// Get buffer (OpenGL viewport) size information
	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	// Set context for GLEW to use
	glfwMakeContextCurrent(mainWindow);

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

	// Setup viewport size
	glViewport(0, 0, bufferWidth, bufferHeight);

	CreateTriangle();
	CompileShaders();

	// Loop until window is closed
	while (!glfwWindowShouldClose(mainWindow))
	{
		// Get and handle user input events
		glfwPollEvents();

		if (direction)
		{
			triOffset += triIncrement;
		}
		else
		{
			triOffset -= triIncrement;
		}

		if ((abs(triOffset) >= triMaxOffset))
		{
			direction = !direction;
		}

		currentAngle += 0.005f;
		if (currentAngle >= 360)
		{
			currentAngle -= 360;
		}

		if (sizeDirection)
		{
			currentSize += 0.0001f;
		}
		else
		{
			currentSize -= 0.0001f;
		}

		if (currentSize >= maxSize || currentSize <= minSize)
		{
			sizeDirection = !sizeDirection;
		}

		// Clear window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shader);

		// Create identity matrix
		glm::mat4 model(1.0f);
		// Translate only x coordinate value
		//model = glm::translate(model, glm::vec3(triOffset, 0.0f, 0.0f));
		// Rotate by curr angle degrees around z axis
		//model = glm::rotate(model, currentAngle * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		// Scale x and y values to curr size percent
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));

		// Set our model matrix as uniform value
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		glBindVertexArray(VAO);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		glBindVertexArray(0);
		glUseProgram(0);

		// Swap to buffer that we have drawn to
		glfwSwapBuffers(mainWindow);
	}

	return 0;
}