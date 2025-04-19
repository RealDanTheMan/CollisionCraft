#ifndef MESH_H
#define MESH_H

#include <memory>
#include <vector>
#include <QVector3D>

class Mesh
{
public:
    Mesh(const std::vector<QVector3D> &vertices, const std::vector<int> &indices);
    Mesh(const Mesh &from);

    const std::vector<QVector3D>& getVertices() const;
    const std::vector<QVector3D>& getNormals() const;
    const std::vector<int>& getIndices() const;

    size_t numIndices() const;
    size_t numVertices() const;
    size_t numNormals() const;
    void generateNormals();
    void computeBounds();

    const QVector3D& getBoundingSphereCenter() const;
    const double getBoundingSphereRadius() const;

protected:
    std::vector<QVector3D> vertices;
    std::vector<QVector3D> normals;
    std::vector<int> indices;

    QVector3D bsphere_center;
    double bsphere_radius;
};

#endif
