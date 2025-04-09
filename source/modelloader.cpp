#include "modelloader.h"
#include "logging.h"
#include "mesh.h"

#include <vector>
#include <QString>
#include <QDir>
#include <QFile>
#include <QTemporaryFile>

#include <pxr/usd/usd/stage.h>
#include <pxr/usd/usdGeom/tokens.h>
#include <pxr/usd/usdGeom/metrics.h>
#include <pxr/usd/usdGeom/xform.h>
#include <pxr/usd/usdGeom/xformCommonAPI.h>
#include <pxr/usd/usd/primRange.h>
#include <pxr/usd/usdGeom/mesh.h>
#include <pxr/base/vt/array.h>

/// Load model data from USD file stored in QT resource pack.
/// @param: resource_path Resource pack relative path to the usd model.
/// @param: meshes Container to append loaded meshes to.
void ModelLoader::LoadResourceUSD(const std::string &resource_path, std::vector<Mesh>& meshes)
{
	/// TODO: Loading QT resource model and write to tmp file that
	/// USD SDK can read data from.
	/// Note: At this time USD SDK cannot read from in-memory stream.
	logDebug("Loading resource usd model file -> {}", resource_path);
	QFile file(QString::fromStdString(resource_path));
	if	(!file.open(QIODevice::ReadOnly))
	{
		Logger::active()->error("Failed at fetching resource model file!");
		return;
	}

	QFileInfo info(file);
	QTemporaryFile tmp_file(QDir::tempPath() + "/" + info.baseName() + "_XXXXXX.usdc");
	if(!tmp_file.open())
	{
		logError("Failed to create temporary model file -> {}", tmp_file.fileName().toStdString());
		logError("Error: {}", tmp_file.errorString().toStdString());
	}

	tmp_file.write(file.readAll());
	tmp_file.flush();
	tmp_file.close();

	logDebug("Wrote tmp usd content -> {}", tmp_file.fileName().toStdString());
	ModelLoader::LoadUSD(tmp_file.fileName().toStdString(), meshes);
}

/// Load model data from USD file on disk.
/// @param: filepath File path to USD file on disk to load data from.
/// @param: meshes Container to append loaded mesh data to.
void ModelLoader::LoadUSD(const std::string &filepath, std::vector<Mesh>& meshes)
{
	Logger::active()->info("Loading usd model file");

	pxr::UsdStageRefPtr stage = pxr::UsdStage::Open(filepath);
	if (!stage)
	{
		logWarning("Failed to open USD model file -> {}.", filepath);
		return;
	}


	for (const pxr::UsdPrim &prim : stage->Traverse())
	{
		if (!prim.IsA<pxr::UsdGeomMesh>())
		{
			continue;
		}

		logDebug("Analising USD Mesh -> {}", prim.GetPath().GetString());
		pxr::TfToken up_axis = pxr::UsdGeomGetStageUpAxis(stage);
		pxr::UsdGeomMesh mesh(prim);
	

		/// Vertex data.
		std::vector<QVector3D> mesh_vertices;
		pxr::VtArray<pxr::GfVec3f> vertices;
		mesh.GetPointsAttr().Get(&vertices);
		mesh_vertices.reserve(vertices.size());
		for (const pxr::GfVec3f &vertex : vertices)
		{
			/// Convert to Z up axis as needed.
			if (up_axis == pxr::UsdGeomTokens->z)
			{
				pxr::GfMatrix4d yup {
					1, 0, 0, 0,
					0, 0, -1, 0,
					0, 1, 0, 0,
					0, 0, 0, 1
				};
				
				pxr::GfVec3d pos = yup.Transform(vertex);
				mesh_vertices.emplace_back(pos[0], pos[1], pos[2]);
			}
			else
			{
				mesh_vertices.emplace_back(vertex[0], vertex[1], vertex[2]);
			}
		}

		/// Triangle index data.
		pxr::VtArray<int> indices;
		mesh.GetFaceVertexIndicesAttr().Get(&indices);
		std::vector<int> mesh_indices = std::vector<int>(indices.begin(), indices.end());
	
		/// Build mesh data.
		meshes.emplace_back(mesh_vertices, mesh_indices);
		meshes.back().generateNormals();
		meshes.back().computeBounds();
		logDebug("USD mesh vertex count -> {}", meshes.back().numVertices());
		logDebug("USD mesh index count -> {}", meshes.back().numIndices());
	}
}
