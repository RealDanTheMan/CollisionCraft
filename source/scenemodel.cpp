#include "scenemodel.h"
#include "mesh.h"
#include <algorithm>
#include <memory>

SceneModel::SceneModel(const Mesh &source_mesh)
{
    this->mesh = std::make_unique<Mesh>(source_mesh);
    this->render_mesh = std::make_unique<RenderMesh>(source_mesh);
}

/// Get readonly reference to this model geometry mesh.
const Mesh& SceneModel::getMesh() const
{
    return *this->mesh;
}

/// Get reference to this model render mesh.
RenderMesh& SceneModel::getRenderMesh()
{
    return *this->render_mesh;
}
