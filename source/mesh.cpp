#include "mesh.h"
#include <cmath>
#include <memory>
#include <vector>

Mesh::Mesh(const std::vector<QVector3D> &vertices, const std::vector<int> &indices) :
        bsphere_center(QVector3D(0.0, 0.0, 0.0)),
        bsphere_radius(0.0)
{
    this->vertices = std::vector<QVector3D>(vertices.begin(), vertices.end());
    this->indices = std::vector<int>(indices.begin(), indices.end());
}

/// Default copy constructor
Mesh::Mesh(const Mesh &from) :
    bsphere_center(from.bsphere_center),
    bsphere_radius(from.bsphere_radius)
{
    
    this->vertices = std::vector<QVector3D>(from.vertices.begin(), from.vertices.end());
    this->indices = std::vector<int>(from.indices.begin(), from.indices.end());
    this->normals = std::vector<QVector3D>(from.normals.begin(), from.normals.end());
}

/// Auto generates normals based on existing triangle data.
/// Clears existing normal data.
void Mesh::generateNormals()
{
    this->normals.clear();

    std::vector<std::vector<QVector3D>> all_normals;
    all_normals.resize(this->vertices.size());

    /// Compute all normals for all triangles each vertex is part of.
    for (int i=0; i+2 < this->indices.size(); i+=3)
    {
        const int idx0 = this->indices.at(i);
        const int idx1 = this->indices.at(i+1);
        const int idx2 = this->indices.at(i+2);
        
        QVector3D a = this->vertices.at(idx0);
        QVector3D b = this->vertices.at(idx1);
        QVector3D c = this->vertices.at(idx2);

        QVector3D ab = (b - a);
        QVector3D ac = (c - a); 
        QVector3D normal = QVector3D::crossProduct(ab, ac);

        all_normals.at(idx0).push_back(normal);
        all_normals.at(idx1).push_back(normal);
        all_normals.at(idx2).push_back(normal);
    }

    /// Update mesh normals with average normals of all triangles given vertex is part of.
    this->normals.reserve(this->vertices.size());
    for (int i=0; i < this->vertices.size(); i++)
    {
        QVector3D normal = QVector3D();
        for (const QVector3D &vnormal : all_normals.at(i))
        {
            normal += vnormal;
        }

        normal /= all_normals.at(i).size();
        this->normals.push_back(normal.normalized());
    }
}

/// Computes mesh bounding sphere.
void Mesh::computeBounds()
{
    if (this->vertices.size() == 0)
    {
        this->bsphere_center = QVector3D(0.0, 0.0, 0.0);
        this->bsphere_radius = 0.0;
        return;
    }

    /// Compute bounding sphere center.
    QVector3D center = QVector3D(0.0, 0.0, 0.0);
    for (const QVector3D &pos : this->vertices)
    {
        center += pos;
    }
    center /= double(this->vertices.size());

    /// Compute bounding sphere radius
    double radius = 0.0;
    for (const QVector3D &pos : this->vertices)
    {
        const double len = (pos - center).lengthSquared();
        if (len > radius)
        {
            radius = len;
        }
    }
    radius = std::sqrt(radius);

    this->bsphere_center = center;
    this->bsphere_radius = radius;
}

/// Get read-only access to mesh vertices.
const std::vector<QVector3D>* Mesh::getVertices() const
{
    return &this->vertices;
}

/// Get read-only access to mesh normals.
const std::vector<QVector3D>* Mesh::getNormals() const
{
    return &this->normals;
}

/// Get read-only access to mesh triangle indices.
const std::vector<int>* Mesh::getIndices() const
{
    return &this->indices;
}

/// Get number of vertices stored in this mesh data.
size_t Mesh::numVertices() const
{
    return this->vertices.size();
}

/// Get number of normals stored in this mesh data.
size_t Mesh::numNormals() const
{
    return this->normals.size();
}

/// Get number of triangle indices stored in this mesh data.
size_t Mesh::numIndices() const
{
    return this->indices.size();
}

/// Get this mesh bounding sphere center.
/// Ensure to call Mesh::computeBounds() if mesh changed since last call.
const QVector3D& Mesh::getBoundingSphereCenter() const
{
    return this->bsphere_center;
}

/// Get this mesh bounding sphere radius.
/// Ensure to call Mesh::computeBounds() if mesh changed since last call.
const double Mesh::getBoundingSphereRadius() const
{
    return this->bsphere_radius;
}
