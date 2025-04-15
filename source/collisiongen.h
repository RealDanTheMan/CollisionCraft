#ifndef COLLISION_GEN_H
#define COLLISION_GEN_H

#include "mesh.h"

#include <vector>
#include <memory>

#include <CGAL/Simple_cartesian.h>
#include <CGAL/Surface_mesh.h>

using CGAL_Kernel = CGAL::Simple_cartesian<double>;
typedef CGAL_Kernel::Point_3 CGAL_Point;
typedef CGAL::Surface_mesh<CGAL_Point> CGAL_Surface;


class CollisionGen
{
public:
    CollisionGen();

    void addInputMesh(const Mesh *mesh);
    void clearInputMeshes();
    void generateCollisionHull(std::unique_ptr<Mesh> &out_mesh);

protected:
    std::vector<CGAL_Point> getInputPoints(float padding = 0.0) const;
    static std::unique_ptr<Mesh> meshFromSurface(const CGAL_Surface &surface);

private:
    std::vector<const Mesh*> input_meshes;
};

#endif
