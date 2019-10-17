#pragma once

#include <GL/glew.h>

class Mesh
{
private:
	GLuint VAO, VBO, IBO;
	GLsizei indexCount;
public:
	Mesh();
	~Mesh();

	void CreateMesh(GLfloat* vertices, unsigned int* indices, unsigned int numberOfVertices, unsigned int numberOfIndices);
	void RenderMesh();
	void ClearMesh();
};

