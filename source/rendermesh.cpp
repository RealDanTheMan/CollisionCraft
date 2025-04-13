#include "rendermesh.h"
#include <cstdint>


RenderMesh::RenderMesh(const Mesh &mesh)
    : vertex_buffer(QOpenGLBuffer::VertexBuffer),
      index_buffer(QOpenGLBuffer::IndexBuffer)
{
    initializeOpenGLFunctions();

    this->vertex_attributes.create();
    this->vertex_attributes.bind();

    const int position_size = mesh.numVertices() * sizeof(QVector3D);
    const int normal_size = mesh.numNormals() * sizeof(QVector3D);
    const int buffer_size = position_size + normal_size;

    this->vertex_buffer.create();
    this->vertex_buffer.bind();
    this->vertex_buffer.allocate(buffer_size);

    /// We write vertex buffer data sequentially by data type.
    /// Buffer layout example [[position], [normals], [other]]
    this->vertex_buffer.write(0, mesh.getVertices()->data(), position_size);
    this->vertex_buffer.write(position_size, mesh.getNormals()->data(), normal_size);

    this->index_buffer.create();
    this->index_buffer.bind();
    this->index_buffer.allocate(
        mesh.getIndices()->data(),
        mesh.numIndices() * sizeof(int)
    );

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)(intptr_t)(position_size));

    this->vertex_attributes.release();
    this->vertex_buffer.release();
    this->index_buffer.release();
    this->index_size = mesh.getIndices()->size();

    this->bsphere_center = mesh.getBoundingSphereCenter();
    this->bsphere_radius = mesh.getBoundingSphereRadius();
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

    glDrawElements(GL_TRIANGLES, this->index_size, GL_UNSIGNED_INT, nullptr);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    this->index_buffer.release();
    this->vertex_buffer.release();
    this->vertex_attributes.release();
    shader.release();
}

/// Get center of the bounding sphere of this render mesh.
const QVector3D& RenderMesh::getBoundingShereCenter() const
{
    return this->bsphere_center;
}

/// Get radius of the bounding sphere of this render mesh.
const double RenderMesh::getBoundingShereRadius() const
{
    return this->bsphere_radius;
}
