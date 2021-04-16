//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Scene/Component/Model.h"
#include "ResourceManager/Resources/Mesh.h"
#include "Engine.h"
#include "ResourceManager/ResourceManager.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Scene/Entity/EntityManager.h"
#include "ResourceManager/FbxLoader.h"
#include "Utilities/StringUtils.h"
#include "Animation/SkeletonCache.h"
#include "MeshRenderer.h"


namespace Raven
{
	Model::Model(const std::string & fileName)
		: primitiveType(PrimitiveType::File), filePath(fileName)
	{
		
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
		if (index >= meshes.size())
		{
			return nullptr;
		}
		else
		{
			// can use this pointer to load a single mesh onto the GPU
			return meshes[index];
		}
	}

	std::shared_ptr<Mesh> Model::AddMesh(Mesh* mesh)
	{
		auto ret = std::shared_ptr<Mesh>(mesh);
		meshes.emplace_back(ret);
		return ret;
	}

	void Model::AddMesh(const std::shared_ptr<Mesh>& mesh)
	{
		meshes.emplace_back(mesh);
	}

	void Model::AddMeshes(const std::vector<std::shared_ptr<Mesh>> & inputMeshes)
	{
		// for multiple meshes
		meshes.insert(meshes.end(), inputMeshes.begin(), inputMeshes.end());
	}

	void Model::LoadFile(bool fromLoad)
	{
		auto res = Engine::Get().GetModule<ResourceManager>();
		auto loader = res->GetLoader<MeshLoader>();

		std::string extension = StringUtils::GetExtension(filePath);
		// file extension calls appropriate loading function
		if (extension == "obj")
		{
			loader->LoadOBJ(filePath);
			res->LoadResource<Mesh>(filePath);
			res->GetResource(filePath, meshes);
			BindMeshComponent();
		}
		else if (extension == "fbx")
		{
			loader->LoadFBX(filePath, fromLoad ? nullptr : this);
			if(meshes.empty())
				res->GetResource(filePath, meshes);
			BindMeshComponentForFBX();
		}
	}

	void Model::BindMeshComponentForFBX()
	{
		auto currentScene = Engine::Get().GetModule<SceneManager>()->GetCurrentScene();
		Entity ent(entity, currentScene);
		if (ent.GetChildren().empty())
		{
			FbxLoader loader;
			loader.LoadHierarchy(filePath, this);

			for (auto i = 0; i < meshes.size(); i++)
			{
				auto entity = currentScene->CreateEntity(meshes[i]->name);
				if (!meshes[i]->blendIndices.empty() ||
					!meshes[i]->blendWeights.empty())
				{
					auto& render = entity.AddComponent<SkinnedMeshRenderer>();
					render.mesh = meshes[i];
					render.meshIndex = i;
					render.skeleton = SkeletonCache::Get().Get(filePath);
				}
				else
				{
					auto& render = entity.AddComponent<MeshRenderer>();
					render.mesh = meshes[i];
					render.meshIndex = i;
				}
				entity.SetParent(ent);
			}
		}
	}

	void Model::BindMeshComponent()
	{
		auto currentScene = Engine::Get().GetModule<SceneManager>()->GetCurrentScene();
		auto& enManager = currentScene->GetEntityManager();

		Entity ent(entity, currentScene);

		if (ent.GetChildren().empty())
		{
			if (meshes.size() == 1)
			{
				auto& render = ent.AddComponent<MeshRenderer>();
				render.mesh = meshes[0];
				render.meshIndex = 0;
			}
			else
			{
				for (auto i = 0; i < meshes.size(); i++)
				{
					auto entity = currentScene->CreateEntity(meshes[i]->name);
					auto& render = entity.AddComponent<MeshRenderer>();
					render.mesh = meshes[i];
					render.meshIndex = i;
					entity.SetParent(ent);
				}
			}
		}
		
	}
}