#include "gridrendermesh.h"
#include <stdexcept>
#include <vector>

GridRenderMesh::GridRenderMesh(unsigned int rows, unsigned int columns, double spacing)
    : vertex_buffer(QOpenGLBuffer::VertexBuffer),
      shaderHandle(nullptr),
      num_vertices(0)
{
    initializeOpenGLFunctions();

    this->vertex_attributes.create();
    this->vertex_attributes.bind();
    this->vertex_buffer.create();
    this->vertex_buffer.bind();

    const int half_rows = rows / 2;
    const int half_columns = columns / 2;
    std::vector<QVector3D> vertices;

    for (int x=-half_columns; x<half_columns; ++x)
    {
        vertices.emplace_back(x * spacing, 0.0, -half_rows * spacing);
        vertices.emplace_back(x * spacing, 0.0, half_rows * spacing);
    }
    
    for (int y=-half_rows; y<half_rows; ++y)
    {
        vertices.emplace_back(-half_columns * spacing, 0.0, y * spacing);
        vertices.emplace_back(half_columns * spacing, 0.0, y * spacing);
    }

    this->num_vertices = vertices.size();
    this->vertex_buffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    this->vertex_buffer.allocate(vertices.data(), vertices.size() * sizeof(QVector3D));
    this->vertex_attributes.release();
    this->vertex_buffer.release();
}

/// Assign shader program to use when rendering this grid mesh.
void GridRenderMesh::bindShader(QOpenGLShaderProgram *shader)
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

/// Get shader program bound to this grid render mesh.
QOpenGLShaderProgram * GridRenderMesh::shader() const
{
    return this->shaderHandle;
}

void GridRenderMesh::updateVertexDataLayout()
{
    this->vertex_attributes.bind();
    this->vertex_buffer.bind();
    this->shaderHandle->bind();

    GLint num_attributes = 0;
    glGetProgramiv(this->shaderHandle->programId(), GL_ACTIVE_ATTRIBUTES, &num_attributes);

    if (num_attributes > 0)
    {
        this->shaderHandle->enableAttributeArray(0);
        this->shaderHandle->setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(QVector3D));
    }

    this->vertex_attributes.release();
    this->vertex_buffer.release();
    this->shaderHandle->release();
}

/// Draw this grid mesh to active render target.
void GridRenderMesh::render()
{
    if (!this->shader() || this->shader()->programId() == 0)
    {
        throw std::runtime_error("Attempting to draw grid render mesh with invalid shader handle");
    }

    this->vertex_attributes.bind();
    this->vertex_buffer.bind();
    this->shader()->bind();

    glDrawArrays(GL_LINES, 0, this->num_vertices);

    this->vertex_attributes.release();
    this->vertex_buffer.release();
    this->shader()->release();
}
