//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Scene/Component/Model.h"

#include "ResourceManager/Resources/Mesh.h"

namespace Raven
{
	Model::~Model()
	{
		meshes.clear(); // calls the deleters for mesh objects
	}

	void Model::LoadOnGpu()
	{
		// loads the meshes onto the GPU
		for (const auto& mesh : meshes)
		{
			mesh->LoadOnGpu();
		}

	}

	// return shared pointer to a mesh resource
	std::shared_ptr<Mesh> Model::GetMesh(size_t index)
	{
		if (index > meshes.size())
		{
			return nullptr;
		}
		else
		{
			// can use this pointer to load a single mesh onto the GPU
			return meshes[index];
		}
	}

	// pointer to the vector of meshes
	std::vector<std::shared_ptr<Mesh>>& Model::GetMeshes()
	{
		return meshes;
	}

	void Model::AddMesh(Mesh* mesh)
	{
		// add a single mesh to the model
		meshes.emplace_back(std::shared_ptr<Mesh>(mesh));
	}

	void Model::AddMeshes(std::vector<std::shared_ptr<Mesh>> inputMeshes)
	{
		// for multiple meshes
		meshes.insert(meshes.end(), inputMeshes.begin(), inputMeshes.end());
	}
}