#pragma once

#include <iostream>
#include <string>
#include <fstream>

#include <GL/glew.h>

class Shader
{
private:
	GLuint shaderID, uniformProjection, uniformModel, uniformView;
public:
	Shader();
	~Shader();

	void CreateFromString(const char* vertexCode, const char* fragmentCode);
	void CreateFromFile(const char* vertexFile, const char* fragmentFile);
	std::string ReadFile(const char* fileLocation);
	void UseShader();
	void ClearShader();
	void CompileShader(const char* vertexCode, const char* fragmentCode);
	void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);

	inline GLuint GetProjectionLocation() const { return uniformProjection; };
	inline GLuint GetModelLocation() const { return uniformModel; };
	inline GLuint GetViewLocation() const { return uniformView; };
};

