#ifndef SCENE_MODEL_H
#define SCENE_MODEL_H

#include "mesh.h"
#include "rendermesh.h"
#include <memory>

class SceneModel
{
public:
    SceneModel(const Mesh &source_mesh);

    const Mesh& getMesh() const;
    RenderMesh& getRenderMesh();

private:
    std::unique_ptr<Mesh> mesh;
    std::unique_ptr<RenderMesh> render_mesh;
};

#endif
