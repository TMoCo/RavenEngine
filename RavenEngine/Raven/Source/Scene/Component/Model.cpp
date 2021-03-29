//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Scene/Component/Model.h"
#include "ResourceManager/Resources/Mesh.h"
#include "Engine.h"
#include "ResourceManager/ResourceManager.h"


namespace Raven
{
	Model::Model(const std::string & fileName)
		: primitiveType(PrimitiveType::File), filePath(fileName)
	{
		LoadFile();
	}

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



	void Model::AddMesh(Mesh* mesh)
	{
		meshes.emplace_back(std::shared_ptr<Mesh>(mesh));
	}

	void Model::AddMeshes(const std::vector<std::shared_ptr<Mesh>> & inputMeshes)
	{
		// for multiple meshes
		meshes.insert(meshes.end(), inputMeshes.begin(), inputMeshes.end());
	}

	void Model::LoadFile()
	{
		auto res = Engine::Get().GetModule<ResourceManager>();
		res->LoadResource<Mesh>(filePath);
		res->GetResource(filePath, meshes);
	}

}