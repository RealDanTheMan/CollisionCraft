#ifndef GRID_RENDER_MESH_H
#define GRID_RENDER_MESH_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

class GridRenderMesh : protected QOpenGLFunctions
{

public:
    GridRenderMesh(unsigned int rows, unsigned int columns, double spacing);
    void render();
    void bindShader(QOpenGLShaderProgram *shader);
    QOpenGLShaderProgram* shader() const;

protected:
    void updateVertexDataLayout();

private:
    QOpenGLShaderProgram *shaderHandle;
    QOpenGLBuffer vertex_buffer;
    QOpenGLVertexArrayObject vertex_attributes;
    int num_vertices;
};

#endif
