#ifndef RENDER_MESH_H
#define RENDER_MESH_H

#include "mesh.h"

#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <QMatrix4x4>

class RenderMesh : protected QOpenGLFunctions
{
public:
    RenderMesh(const Mesh &mesh);
    void Render(QOpenGLShaderProgram &shader);

    const QVector3D& getBoundingShereCenter() const;
    const double getBoundingShereRadius() const;

    void setTransform(const QMatrix4x4 &transform);
    const QMatrix4x4& getTransform() const;

    void setShader(QOpenGLShaderProgram *shader);
    QOpenGLShaderProgram* getShader() const;


private:
    int index_size = 0;
    QOpenGLBuffer vertex_buffer;
    QOpenGLBuffer index_buffer;
    QOpenGLVertexArrayObject vertex_attributes;

    QMatrix4x4 transform;
    QVector3D bsphere_center;
    double bsphere_radius;

    QOpenGLShaderProgram* shader = nullptr;
};

#endif
