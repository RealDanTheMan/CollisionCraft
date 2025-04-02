#include "mesh.h"
#include "logging.h"
#include <memory>
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

	this->normals = std::make_unique<std::vector<QVector3D>>();
}

/// Auto generates normals based on existing triangle data.
/// Clears existing normal data.
void Mesh::generateNormals()
{
	this->normals->clear();

	std::vector<std::vector<QVector3D>> all_normals;
	all_normals.resize(this->vertices->size());

	/// Compute all normals for all triangles each vertex is part of.
	for (int i=0; i+2 < this->indices->size(); i+=3)
	{
		const int idx0 = this->indices->at(i);
		const int idx1 = this->indices->at(i+1);
		const int idx2 = this->indices->at(i+2);
		
		QVector3D a = this->vertices->at(idx0);
		QVector3D b = this->vertices->at(idx1);
		QVector3D c = this->vertices->at(idx2);

		QVector3D ab = (b - a);
		QVector3D ac = (c - a); 
		QVector3D normal = QVector3D::crossProduct(ab, ac);

		all_normals.at(idx0).push_back(normal);
		all_normals.at(idx1).push_back(normal);
		all_normals.at(idx2).push_back(normal);
	}

	/// Update mesh normals with average normals of all triangles given vertex is part of.
	this->normals->reserve(this->vertices->size());
	for (int i=0; i < this->vertices->size(); i++)
	{
		QVector3D normal = QVector3D();
		for (const QVector3D &vnormal : all_normals.at(i))
		{
			normal += vnormal;
		}

		normal /= all_normals.at(i).size();
		this->normals->push_back(normal.normalized());
	}
}

/// Get read-only access to mesh vertices.
const std::vector<QVector3D>* Mesh::getVertices() const
{
	return this->vertices.get();
}

/// Get read-only access to mesh normals.
const std::vector<QVector3D>* Mesh::getNormals() const
{
	return this->normals.get();
}

/// Get read-only access to mesh triangle indices.
const std::vector<int>* Mesh::getIndices() const
{
	return this->indices.get();
}

/// Get number of vertices stored in this mesh data.
int Mesh::numVertices() const
{
	return this->vertices->size();
}

/// Get number of normals stored in this mesh data.
int Mesh::numNormals() const
{
	return this->normals->size();
}

/// Get number of triangle indices stored in this mesh data.
int Mesh::numIndices() const
{
	return this->indices->size();
}
