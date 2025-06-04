#ifndef COLLISION_GEN_H
#define COLLISION_GEN_H

#include "logging.h"
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

#define VHACD_USE_OPENCL 0
#include <VHACD.h>


enum CollisionTechnique
{
    SimpleHull = 0,
    ExactDecomposition = 1,
    ApproximateDecomposition = 2
};


class VHACDDebugLogger : public VHACD::IVHACD::IUserLogger
{
public:
    void Log(const char *const msg) override
    {
        logDebug("VHACD Debug message -> {}", msg);
    }
};


struct CollisionGenSettings
{
    double  scale;
    double  resolution;
    int     max_hulls;
    int     max_hull_vertices;
    double  min_hull_volume;
    int     downsample;
    double  concavity;
    int     depth_planes;
    int     mode;
};


class CollisionGen
{
public:
    CollisionGen();

    void addInputMesh(const Mesh *mesh);
    void clearInputMeshes();

    void generateCollisionHull(
        const CollisionGenSettings &settings,
        std::unique_ptr<Mesh> &out_mesh
    );

    void generateCollisionHulls(
        const CollisionGenSettings &settings,
        std::vector<std::unique_ptr<Mesh>> &out_meshes
    );

    void generateVHACD(
        const CollisionGenSettings &settings,
        std::vector<std::unique_ptr<Mesh>> &out_meshes
    );

    static void meshFromSurface(const CGAL_Surface &surface, Mesh &out_mesh);
    static bool surfaceFromMesh(const Mesh &mesh, CGAL_Surface &out_surface);
    static bool polyhedronFromMesh(const Mesh &mesh, CGAL_Polyhedron &out_poly);
    static void capSurface(CGAL_Surface &surface_mesh);

protected:
    std::vector<CGAL_Point> getInputPoints(float padding = 0.0) const;
    bool cleanupMesh(Mesh &mesh);

private:
    std::vector<const Mesh*> input_meshes;
    VHACDDebugLogger vhacd_logger;
};

#endif
