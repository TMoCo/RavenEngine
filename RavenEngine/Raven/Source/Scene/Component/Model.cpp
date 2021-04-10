//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Scene/Component/Model.h"
#include "ResourceManager/ResourceManager.h"

#include "Engine.h"


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

	void Model::AddMesh(Ptr<Mesh> mesh)
	{
		meshes.emplace_back(mesh);
	}

	void Model::AddMeshes(const std::vector< Ptr<Mesh> > & inputMeshes)
	{
		// for multiple meshes
		meshes.insert(meshes.end(), inputMeshes.begin(), inputMeshes.end());
	}

	void Model::SetMaterial(uint32_t index, Ptr<Material> mat)
	{
		if (materials.size() < index + 1)
			materials.resize(index + 1);

		materials[index] = mat;
	}

	Material* Model::GetMaterial(uint32_t index)
	{
		// Invalid Index?
		if (index >= materials.size() || index < 0)
			return nullptr;

		return materials[index].get();
	}

	const Material* Model::GetMaterial(uint32_t index) const
	{
		return const_cast<Model*>(this)->GetMaterial(index);
	}

	void Model::LoadFile()
	{
		auto res = Engine::Get().GetModule<ResourceManager>();
		res->LoadResource<Mesh>(filePath);
		res->GetResource(filePath, meshes);
	}
}