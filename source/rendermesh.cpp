#include "rendermesh.h"
#include <cstdint>
#include <stdexcept>


RenderMesh::RenderMesh(const Mesh &mesh)
    : vertex_buffer(QOpenGLBuffer::VertexBuffer),
      index_buffer(QOpenGLBuffer::IndexBuffer),
      style(RenderMeshStyle::Shaded),
      material(RenderMeshMaterial::Standard),
      vertex_size(0),
      normal_size(0),
      index_size(0)
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
    this->vertex_buffer.write(0, mesh.getVertices().data(), position_size);
    this->vertex_buffer.write(position_size, mesh.getNormals().data(), normal_size);

    this->index_buffer.create();
    this->index_buffer.bind();
    this->index_buffer.allocate(
        mesh.getIndices().data(),
        mesh.numIndices() * sizeof(int)
    );

    /// Note: Depending on platform (MacOS) OpenGL context can defer buffer writes causing first
    /// draw call to not be able to access vertex data, flushing writes prevents that.
    glFlush();

    this->vertex_attributes.release();
    this->vertex_buffer.release();
    this->index_buffer.release();

    this->index_size = mesh.numIndices();
    this->vertex_size = mesh.numVertices();
    this->normal_size = mesh.numIndices();
    this->bsphere_center = mesh.getBoundingSphereCenter();
    this->bsphere_radius = mesh.getBoundingSphereRadius();
}

/// Get number of vertices stored in this mesh vertex buffer.
size_t RenderMesh::numVertices() const
{
    return this->vertex_size;
}

/// Get number if normal vectors stored in this mesh vertex buffer.
size_t RenderMesh::numNormals() const
{
    return this->normal_size;
}

/// Get number if triangle indices stored in this mesh element buffer.
size_t RenderMesh::numIndices() const
{
    return this->index_size;
}

/// Assign shader program to use when rendering this mesh.
void RenderMesh::bindShader(QOpenGLShaderProgram *shader)
{
    if (shader == this->shaderHandle)
    {
        return;
    }

    this->shaderHandle = shader;
    if (this->shaderHandle)
    {
        this->updateVertexDataLayout();
    }
}

/// Get shader program bound to this render mesh.
QOpenGLShaderProgram* RenderMesh::shader() const
{
    return this->shaderHandle;
}

/// Updated this render mesh vertex attributes to match its shader layout specification.
void RenderMesh::updateVertexDataLayout()
{
    GLint attributes = 0;
    glGetProgramiv(this->shaderHandle->programId(), GL_ACTIVE_ATTRIBUTES, &attributes);

    this->vertex_attributes.bind();
    this->vertex_buffer.bind();
    this->index_buffer.bind();
    this->shaderHandle->bind();

    /// Vertex position attribute layout.
    if (attributes > 0)
    {
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    }

    /// Vertex normal attribute layout.
    if (attributes > 1)
    {
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(
            1,
            3,
            GL_FLOAT,
            GL_FALSE, 0,
            (void*)(intptr_t)(this->vertex_size * sizeof(QVector3D))
        );
    }

    this->vertex_attributes.release();
    this->vertex_buffer.release();
    this->index_buffer.release();
    this->shaderHandle->release();
}

/// Draw this mesh to currently bound OpenGL rendering context.
/// @param: shader Shader Program to use during rendering of this mesh.
void RenderMesh::Render()
{
    if (!this->shaderHandle || this->shaderHandle->programId() == 0)
    {
        throw std::runtime_error("Attempting to draw render mesh with invalid shader handle");
    }

    this->vertex_attributes.bind();
    this->index_buffer.bind();
    this->vertex_buffer.bind();
    this->shaderHandle->bind();

    glDrawElements(GL_TRIANGLES, this->index_size, GL_UNSIGNED_INT, nullptr);

    this->vertex_attributes.release();
    this->index_buffer.release();
    this->vertex_buffer.release();
    this->shaderHandle->release();
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

/// Get model transform matrix.
const QMatrix4x4& RenderMesh::getTransform() const
{
    return this->transform;
}

/// Set model transform matrix.
void RenderMesh::setTransform(const QMatrix4x4 &transform)
{
    this->transform = transform;
}

/// Set mesh render material style.
void RenderMesh::setMaterial(RenderMeshMaterial material)
{
    this->material = material;
}

/// Set mesh render style.
void RenderMesh::setStyle(RenderMeshStyle style)
{
    this->style = style;
}

/// Get render material used by this mesh during rendering.
RenderMeshMaterial RenderMesh::getMaterial() const
{
    return this->material;
}

/// Get render style used by this mesh during rendering.
RenderMeshStyle RenderMesh::getStyle() const
{
    return this->style;
}
