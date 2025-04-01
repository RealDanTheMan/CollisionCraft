#include "rendermesh.h"


RenderMesh::RenderMesh(const Mesh &mesh)
	: vertex_buffer(QOpenGLBuffer::VertexBuffer),
	  index_buffer(QOpenGLBuffer::IndexBuffer)
{
	initializeOpenGLFunctions();

	this->vertex_attributes.create();
	this->vertex_attributes.bind();

	this->vertex_buffer.create();
	this->vertex_buffer.bind();
	this->vertex_buffer.allocate(
		mesh.getVertices()->data(),
		mesh.getVertices()->size() * sizeof(QVector3D)
	);

	this->index_buffer.create();
	this->index_buffer.bind();
	this->index_buffer.allocate(
		mesh.getIndices()->data(),
		mesh.getIndices()->size() * sizeof(int)
	);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(QVector3D), nullptr);

	this->vertex_attributes.release();
	this->vertex_buffer.release();
	this->index_buffer.release();
	this->index_size = mesh.getIndices()->size();
}

/// Draw this mesh to currently bound OpenGL rendering context.
/// @param: shader Shader Program to use during rendering of this mesh.
void RenderMesh::Render(QOpenGLShaderProgram &shader)
{
	this->vertex_attributes.bind();
	this->index_buffer.bind();
	this->vertex_buffer.bind();
	shader.bind();
	this->vertex_attributes.bind();

	glDisable(GL_DEPTH_TEST);
	glDrawElements(GL_TRIANGLES, this->index_size, GL_UNSIGNED_INT, nullptr);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	this->index_buffer.release();
	this->vertex_attributes.release();
	shader.release();
}
