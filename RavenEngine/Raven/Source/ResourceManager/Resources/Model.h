#pragma once

#include "ResourceManager/Resources/IResource.h"
#include "ResourceManager/Resources/Mesh.h"

namespace Raven
{
	class Model : public IResource
	{
	public:
		Model() : IResource(EResourceType::RT_Model, true) {}

		inline virtual ~Model()
		{
			meshes.clear(); // calls the deleters for mesh objects
		}

		inline static EResourceType Type() noexcept { return EResourceType::RT_Model; } // return the resource type

		// return shared pointer to a mesh resource
		inline std::shared_ptr<Mesh> GetMesh(size_t index)
		{
			if (index > meshes.size())
			{
				return nullptr;
			}
			else
			{
				return meshes[index];
			}
		}
		// pointer to the vector of meshes
		inline auto* GetMeshes()
		{
			return &meshes;
		}

		inline void AddMesh(Mesh* mesh)
		{
			meshes.push_back(std::shared_ptr<Mesh>(mesh));
		}

	private:
		std::vector<std::shared_ptr<Mesh>> meshes;

		NOCOPYABLE(Model);
	};
}