#ifndef COLLISION_GEN_H
#define COLLISION_GEN_H

#include "mesh.h"

#include <vector>
#include <memory>

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/Nef_polyhedron_3.h>

using CGAL_Kernel = CGAL::Exact_predicates_exact_constructions_kernel;
using CGAL_Point = CGAL_Kernel::Point_3;
using CGAL_Surface = CGAL::Surface_mesh<CGAL_Point>;
using CGAL_Polyhedron = CGAL::Polyhedron_3<CGAL_Kernel>;
using CGAL_NefPolyhedron = CGAL::Nef_polyhedron_3<CGAL_Kernel>;


enum CollisionTechnique
{
    SimpleHull = 0,
    ExactDecomposition = 1
};


class CollisionGen
{
public:
    CollisionGen();

    void addInputMesh(const Mesh *mesh);
    void clearInputMeshes();
    void generateCollisionHull(std::unique_ptr<Mesh> &out_mesh);
    void generateCollisionHulls(std::vector<std::unique_ptr<Mesh>> &out_meshes);

    static void meshFromSurface(const CGAL_Surface &surface, Mesh &out_mesh);
    static bool surfaceFromMesh(const Mesh &mesh, CGAL_Surface &out_surface);
    static bool polyhedronFromMesh(const Mesh &mesh, CGAL_Polyhedron &out_poly);
    static void capSurface(CGAL_Surface &surface_mesh);

protected:
    std::vector<CGAL_Point> getInputPoints(float padding = 0.0) const;

private:
    std::vector<const Mesh*> input_meshes;
};

#endif
