#ifndef MESH_H
#define MESH_H

#include <memory>
#include <vector>
#include <QVector3D>

class Mesh
{
public:
	Mesh(const std::vector<QVector3D> &vertices, const std::vector<int> &indices);
	const std::vector<QVector3D>* getVertices() const;
	const std::vector<int>* getIndices() const;

protected:
	std::unique_ptr<std::vector<QVector3D>> vertices;
	std::unique_ptr<std::vector<int>> indices;
};

#endif
