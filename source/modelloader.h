#ifndef MODEL_LOADER_H
#define MODEL_LOADER_H

#include "mesh.h"

#include <string>
#include <vector>

class ModelLoader
{
public:
	static void 
	LoadResourceUSD(const std::string &resource_path, std::vector<Mesh> &meshes);
	
	static void 
	LoadUSD(const std::string &filepath, std::vector<Mesh> &meshes);
};

#endif
