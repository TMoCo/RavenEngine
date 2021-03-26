//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ResourceManager/Resources/IResource.h"
#include "ResourceManager/Resources/Mesh.h"

//
// A class for 3D models and their related data such as meshes, and later materials
//

namespace Raven
{
	enum class PrimitiveType;
	class Model : public IResource
	{
	public:
		Model() : IResource(EResourceType::RT_Model, true) {}
		Model(const std::string & file) :IResource(EResourceType::RT_Model, true),filePath(file){
			LOGW("mesh did not load when model initilize");
		}

		~Model()
		{
			meshes.clear(); // calls the deleters for mesh objects
		}

		inline static EResourceType Type() noexcept { return EResourceType::RT_Model; } // return the resource type

		inline void LoadOnGpu()
		{
			if (!onGPU)
			{
				// loads the meshes onto the GPU
				for (const auto& mesh : meshes)
				{
					mesh->LoadOnGpu();
				}
				onGPU = true;
			}
		}

		// return shared pointer to a mesh resource
		inline std::shared_ptr<Mesh> GetMesh(size_t index)
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
		inline auto& GetMeshes()
		{
			return meshes;
		}

		inline void AddMesh(Mesh* mesh)
		{
			meshes.push_back(std::shared_ptr<Mesh>(mesh));
		}

		inline auto GetPrimitiveType() const { return primitiveType; }

		inline auto SetPrimitiveType(PrimitiveType type) { primitiveType = type; }

		inline auto GetFileName() const { return filePath; }

	private:
		std::vector<std::shared_ptr<Mesh>> meshes;
		std::string filePath;
		PrimitiveType primitiveType;
	};
}