#include "mesh.h"
#include <vector>

Mesh::Mesh(const std::vector<QVector3D> &vertices, const std::vector<int> &indices)
{
    this->vertices = std::make_unique<std::vector<QVector3D>>();
	for (const QVector3D &vert : vertices)
	{
		this->vertices->push_back(vert);
	}
	
    this->indices = std::make_unique<std::vector<int>>();
	for (const int &idx : indices)
	{
		this->indices->push_back(idx);
	}
}

/// Get read-only access to mesh vertices.
const std::vector<QVector3D>* Mesh::getVertices() const
{
	return this->vertices.get();
}

/// Get read-only access to mesh triangle indices.
const std::vector<int>* Mesh::getIndices() const
{
	return this->indices.get();
}
