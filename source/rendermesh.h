#ifndef RENDER_MESH_H
#define RENDER_MESH_H

#include "mesh.h"

#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <QMatrix4x4>

enum class RenderMeshStyle
{
    Shaded = 0,
    ShadedWireframe = 1,
    WireframeOnly = 2
};


enum class RenderMeshMaterial
{
    Standard = 0,
    Collision = 1
};


class RenderMesh : protected QOpenGLFunctions
{
public:
    RenderMesh(const Mesh &mesh);
    void Render(QOpenGLShaderProgram &shader);

    const QVector3D& getBoundingShereCenter() const;
    void setTransform(const QMatrix4x4 &transform);
    void setMaterial(RenderMeshMaterial material);
    void setStyle(RenderMeshStyle style);

    const double getBoundingShereRadius() const;
    const QMatrix4x4& getTransform() const;
    RenderMeshMaterial getMaterial() const;
    RenderMeshStyle getStyle() const;


private:
    int index_size = 0;
    QOpenGLBuffer vertex_buffer;
    QOpenGLBuffer index_buffer;
    QOpenGLVertexArrayObject vertex_attributes;

    QMatrix4x4 transform;
    QVector3D bsphere_center;
    double bsphere_radius;

    RenderMeshStyle style;
    RenderMeshMaterial material;
};

#endif
