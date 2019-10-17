#include "Mesh.h"

Mesh::Mesh()
	: VAO(0), VBO(0), IBO(0), indexCount(0)
{
}

Mesh::~Mesh()
{
	ClearMesh();
}

void Mesh::CreateMesh(GLfloat* vertices, unsigned int* indices, unsigned int numberOfVertices, unsigned int numberOfIndices)
{
	indexCount = numberOfIndices;

	// Create vertex array in Graphics RAM and store its ID in VAO
	glGenVertexArrays(1, &VAO);
	// Bind (use) vertex array with ID stored in VAO
	glBindVertexArray(VAO);

	// Generate index buffer with ID stored in IBO
	glGenBuffers(1, &IBO);
	// Bind it as index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	// Bind data from indices to index buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * numberOfIndices, indices, GL_STATIC_DRAW);

	// Generate vertex buffer and store its ID in VBO 
	glGenBuffers(1, &VBO);
	// Bind that buffer as array buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Bind data from vertices to vertex array
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numberOfVertices, vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	// Unbind vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	// Unbind vertex array
	glBindVertexArray(0);
}

void Mesh::RenderMesh()
{
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Mesh::ClearMesh()
{
	if (IBO != 0)
	{
		glDeleteBuffers(1, &IBO);
		IBO = 0;
	}
	
	if (VBO != 0)
	{
		glDeleteBuffers(1, &VBO);
		VBO = 0;
	}
	
	if (VAO != 0)
	{
		glDeleteVertexArrays(1, &VAO);
		VAO = 0;
	}

	indexCount = 0;
}
