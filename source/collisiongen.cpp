#include "collisiongen.h"
#include "logging.h"
#include <memory>
#include <unordered_map>
#include <vector>

#include <CGAL/Polygon_mesh_processing/triangulate_faces.h>
#include <CGAL/convex_hull_3.h>


CollisionGen::CollisionGen()
{
}

void CollisionGen::addInputMesh(const Mesh *mesh)
{
    this->input_meshes.push_back(mesh);
}

void CollisionGen::clearInputMeshes()
{
    this->input_meshes.clear();
}

/// Generate simple collision hull mesh which envelops all active input meshes.
/// @param: out_mesh Output mesh handle to allocate generated collision hull.
void CollisionGen::generateCollisionHull(std::unique_ptr<Mesh> &out_mesh)
{
    std::vector<CGAL_Point> points = this->getInputPoints(0.01);
    CGAL_Surface collision_surface;

    CGAL::convex_hull_3(points.begin(), points.end(), collision_surface);
    std::unique_ptr<Mesh> mesh = CollisionGen::meshFromSurface(collision_surface);

    logInfo("Generated collision mesh vertex count -> {}", mesh->numVertices());
    logInfo("Generated collision mesh triangle count -> {}", mesh->numIndices() / 3);
    out_mesh = std::make_unique<Mesh>(*mesh);
}

/// Generate list of all vertex position across all input meshes.
/// @param: padding Normalized padding value relative to bunding sphere diameter of each input mesh.
std::vector<CGAL_Point> CollisionGen::getInputPoints(float padding) const
{
    std::vector<CGAL_Point> points;
    for (const Mesh *mesh : this->input_meshes)
    {
        const QVector3D center = mesh->getBoundingSphereCenter();
        const double diameter = mesh->getBoundingSphereRadius() * 2;

        for (const QVector3D &vertex : *mesh->getVertices())
        {
            if (qAbs(padding) > 0.0)
            {
                QVector3D padding_dir = (vertex - center).normalized();
                QVector3D point = vertex + padding_dir * padding * diameter;
                points.emplace_back(point.x(), point.y(), point.z());
            }
            else
            {
                points.emplace_back(vertex.x(), vertex.y(), vertex.z());
            }
        }
    }

    return points;
}

/// Build triangulated mesh data from CGAL polyhedron.
/// @param: polyhedron Polyhedron to build mesh from.
std::unique_ptr<Mesh> CollisionGen::meshFromSurface(const CGAL_Surface &surface)
{
    CGAL_Surface tri_surface = surface;
    CGAL::Polygon_mesh_processing::triangulate_faces(tri_surface);

    std::vector<QVector3D> vertices;
    std::vector<int> indices;
    vertices.reserve(tri_surface.vertices().size());
    indices.reserve(tri_surface.faces().size() * 3);

    std::unordered_map<CGAL_Surface::Vertex_index, int> vertex_map;
    vertex_map.reserve(tri_surface.vertices().size());

    int idx = 0;
    for (const CGAL_Surface::Vertex_index &vertex : tri_surface.vertices())
    {
        const CGAL_Point &point = tri_surface.point(vertex);
        vertices.emplace_back(point.x(), point.y(), point.z());
        vertex_map[vertex] = idx;
        idx++;
    }

    for (const CGAL_Surface::Face_index &face : tri_surface.faces())
    {
        CGAL_Surface::halfedge_index edge = tri_surface.halfedge(face);
        for ( int i=0; i<3; i++)
        {
            CGAL_Surface::Vertex_index idx = tri_surface.target(edge);
            indices.emplace_back(vertex_map[idx]);
            edge = tri_surface.next(edge);
        }
    }

    auto mesh = std::make_unique<Mesh>(vertices, indices);
    mesh->generateNormals();
    mesh->computeBounds();

    return mesh;
}
