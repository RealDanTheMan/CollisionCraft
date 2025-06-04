#include "collisiongen.h"
#include "VHACD.h"
#include "logging.h"
#include <memory>
#include <unordered_map>
#include <vector>

#include <CGAL/Polygon_mesh_processing/triangulate_faces.h>
#include <CGAL/Polygon_mesh_processing/triangulate_hole.h>
#include <CGAL/Polygon_mesh_processing/stitch_borders.h>
#include <CGAL/Polygon_mesh_processing/orient_polygon_soup.h>
#include <CGAL/Polygon_mesh_processing/polygon_soup_to_polygon_mesh.h>
#include <CGAL/convex_hull_3.h>
#include <CGAL/convex_decomposition_3.h>


CollisionGen::CollisionGen()
{
}

/// Adds new mesh to use as input for collision generation process.
void CollisionGen::addInputMesh(const Mesh *mesh)
{
    this->input_meshes.push_back(mesh);
}

/// Remove all input meshes.
void CollisionGen::clearInputMeshes()
{
    this->input_meshes.clear();
}

/// Generate collection of hull meshes which envelop all active input meshes using 
/// approximate convex decomposition technique via VHACD.
/// @param: out_meshes List to add newly generated collision hulls to.
void CollisionGen::generateVHACD(
    const CollisionGenSettings &settings,
    std::vector<std::unique_ptr<Mesh>> &out_meshes
)
{
    for (const Mesh *in_mesh : this->input_meshes)
    {
        logDebug("Processing approximate collision for mesh of {} vertices", in_mesh->numVertices());

        Mesh mesh(*in_mesh);
        CollisionGen::cleanupMesh(mesh);
        if (!Mesh::isValid(mesh))
        {
            logError("Encountered degenerate input mesh data, skipping mesh");
            continue;
        }

        std::vector<float> points;
        points.reserve(mesh.numVertices() * 3);
        for (const QVector3D &vertex : mesh.getVertices())
        {
            points.push_back(vertex.x());
            points.push_back(vertex.y());
            points.push_back(vertex.z());
        }

        std::vector<unsigned int> indices;
        indices.reserve(mesh.numIndices());
        for (const int &idx : mesh.getIndices())
        {
            indices.push_back(idx);
        }

        VHACD::IVHACD::Parameters params;
        params.m_resolution = settings.resolution;
        params.m_mode = settings.mode;
        params.m_concavity = settings.concavity;
        params.m_maxConvexHulls = settings.max_hulls;
        params.m_maxNumVerticesPerCH = settings.max_hull_vertices;
        params.m_minVolumePerCH = settings.min_hull_volume;
        params.m_convexhullDownsampling = settings.downsample;
        params.m_logger = &this->vhacd_logger;
        
        /// New version of MacOS have poor support of OpenCL a best so we disable acceleration
        /// to avoid crashes during decomposition process.
#if defined(__APPLE__)
        params.m_oclAcceleration = false;
#endif

        auto vhacd = VHACD::CreateVHACD();
        bool success = vhacd->Compute(
            points.data(),
            mesh.numVertices(),
            indices.data(),
            mesh.numIndices() / 3,
            params
        );

        if (success)
        {
            unsigned int num_hulls = vhacd->GetNConvexHulls();
            logDebug("VHACD Convex Decomposition succeeded generating {} hulls", num_hulls);

            VHACD::IVHACD::ConvexHull hull;
            for (unsigned int i=0; i < num_hulls; i++)
            {
                vhacd->GetConvexHull(i, hull);
                std::vector<QVector3D> vertices;
                std::vector<int> indices;

                for (int i=0; i+2<hull.m_nPoints*3; i+=3)
                {
                    float x = hull.m_points[i];
                    float y = hull.m_points[i+1];
                    float z = hull.m_points[i+2];
                    vertices.emplace_back(x, y, z);
                }

                for (int i=0; i<hull.m_nTriangles*3; i++)
                {
                    indices.push_back(hull.m_triangles[i]);
                }

                out_meshes.push_back(std::make_unique<Mesh>(vertices, indices));
                out_meshes.back()->generateNormals();
                out_meshes.back()->computeBounds();
            }
        }

        vhacd->Clean();
        vhacd->Release();
    }
}

/// Generate list of all vertex position across all input meshes.
/// @param: padding Normalized padding value relative to bounding sphere diameter of each input mesh.
std::vector<CGAL_Point> CollisionGen::getInputPoints(float padding) const
{
    std::vector<CGAL_Point> points;
    for (const Mesh *mesh : this->input_meshes)
    {
        const QVector3D center = mesh->getBoundingSphereCenter();
        const double diameter = mesh->getBoundingSphereRadius() * 2;

        for (const QVector3D &vertex : mesh->getVertices())
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

/// Clean up given mesh to ensure consistent winding order and water tightness.
bool CollisionGen::cleanupMesh(Mesh &mesh)
{
    CGAL_Surface surface;
    CollisionGen::surfaceFromMesh(mesh, surface);
    CollisionGen::capSurface(surface);

    CGAL_Polyhedron polyhedron;
    CGAL::copy_face_graph(surface, polyhedron);

    if (!polyhedron.is_valid())
    {
        logError("Input mesh polyhedron is not valid");
        return false;
    }

    if (!polyhedron.is_closed())
    {
        logError("Input mesh polyhedron is not closed");
        return false;
    }

    CGAL_NefPolyhedron volume_poly(polyhedron);
    if (!volume_poly.is_valid())
    {
        logError("Failed to generate polyhedron does not represent valid volume");
        return false;
    }
    
    volume_poly.convert_to_Polyhedron(polyhedron);
    CGAL::copy_face_graph(polyhedron, surface);
    CollisionGen::meshFromSurface(surface, mesh);

    return true;
}

/// Build triangulated mesh data from CGAL polyhedron.
/// @param: polyhedron Polyhedron to build mesh from.
/// @param: out_mesh Reference to newly built mesh object.
void CollisionGen::meshFromSurface(const CGAL_Surface &surface, Mesh &out_mesh)
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
        vertices.emplace_back(
            CGAL::to_double(point.x()),
            CGAL::to_double(point.y()),
            CGAL::to_double(point.z())
        );

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

    out_mesh = Mesh(vertices, indices);
    out_mesh.generateNormals();
    out_mesh.computeBounds();

    return;
}

/// Builds CGAL surface mesh from standard mesh data.
/// Resulting mesh will have enforced triangulation and consistent winding order.
/// @param: mesh Standard mesh to build surface from.
/// @param: out_surface Reference to newly built surface.
bool CollisionGen::surfaceFromMesh(const Mesh &mesh, CGAL_Surface &out_surface)
{
    std::vector<CGAL_Point> points;
    std::vector<std::vector<int>> faces;

    for (const QVector3D &vertex : mesh.getVertices())
    {
        CGAL_Point vpos(
            CGAL_Kernel::FT(vertex.x()),
            CGAL_Kernel::FT(vertex.y()),
            CGAL_Kernel::FT(vertex.z())
        );

        points.push_back(vpos);
    }

    for (int i=0; i+2 < mesh.numIndices(); i+=3)
    {
        const int idx0 = mesh.getIndices().at(i);
        const int idx1 = mesh.getIndices().at(i+1);
        const int idx2 = mesh.getIndices().at(i+2);
        
         faces.push_back({
           idx0,
           idx1,
           idx2
        });
    }

    CGAL_Surface surface;
    CGAL::Polygon_mesh_processing::orient_polygon_soup(points, faces);
    CGAL::Polygon_mesh_processing::polygon_soup_to_polygon_mesh(points, faces, surface);

    if (surface.is_valid() && CGAL::is_valid_polygon_mesh(surface))
    {
        out_surface = surface;
        return true;
    }

    return false;
}

/// Builds CGAL polyhedron from standard mesh data.
/// @param: mesh Input mesh to generate polyhedron from.
/// @param: out_poly Reference to newly built polyhedron object.
bool CollisionGen::polyhedronFromMesh(const Mesh &mesh, CGAL_Polyhedron &out_poly)
{

    CGAL_Surface surface;
    if (!CollisionGen::surfaceFromMesh(mesh, surface))
    {
        return false;
    }

    CGAL_Polyhedron polyhedron;
    CGAL::copy_face_graph(surface, polyhedron);

    if (polyhedron.is_valid())
    {
        out_poly = polyhedron;
        return true;
    }

    return false;
}

/// Fill holes inside given CGAL surface mesh using simple fan triangle strips.
void CollisionGen::capSurface(CGAL_Surface &surface)
{
    CGAL::Polygon_mesh_processing::stitch_borders(surface);

    for (auto edge : CGAL::halfedges(surface))
    {
        if (CGAL::is_border(edge, surface))
        {
            CGAL::Polygon_mesh_processing::triangulate_hole(surface, edge);
        }
    }
}
