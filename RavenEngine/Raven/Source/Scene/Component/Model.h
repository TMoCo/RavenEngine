//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "ResourceManager/MeshFactory.h"
#include <string>
#include <vector>
#include <memory>
#include "Component.h"
#include "ResourceManager/Resources/Fbx.h"
#include "Animation/Bone.h"
//
// A class for 3D models and their related data such as meshes, and later materials
//

namespace Raven
{
	class Mesh;

	class Model : public Component
	{
	public:
		Model() {}
		Model(const std::string & fileName);

		~Model();

		void LoadOnGpu();

		// return shared pointer to a mesh resource
		std::shared_ptr<Mesh> GetMesh(size_t index);
		// pointer to the vector of meshes
		inline auto& GetMeshes() { return meshes; };

		std::shared_ptr<Mesh> AddMesh(Mesh* mesh);
		void AddMesh(const std::shared_ptr<Mesh> & mesh);
		void AddMeshes(const std::vector<std::shared_ptr<Mesh>>& inputMeshes);

		inline auto GetPrimitiveType() const { return primitiveType; }
		inline auto SetPrimitiveType(PrimitiveType::Id type) { primitiveType = type; }

		inline auto GetFileName() const { return filePath; }

		// serialization load and save
		template<typename Archive>
		void save(Archive& archive) const
		{
			if (meshes.size() > 0)
			{
				archive(
					cereal::make_nvp("PrimitiveType", primitiveType), 
					cereal::make_nvp("FilePath", filePath),
					cereal::make_nvp("Id", entity)
					);
			}
		}
		template<typename Archive>
		void load(Archive& archive)
		{

			archive(
				cereal::make_nvp("PrimitiveType", primitiveType), 
				cereal::make_nvp("FilePath", filePath),
				cereal::make_nvp("Id", entity));

			meshes.clear();

			if (primitiveType != PrimitiveType::File)
			{
				AddMesh(MeshFactory::CreatePrimitive(primitiveType));
			}
			else
			{
				LoadFile(true);
			}
		}

		//if fromLoad is true, indicating engine should not generate the bones and the entities from fbx
//
		void LoadFile(bool fromLoad = false);
	private:

		void BindMeshComponent();
		void BindMeshComponentForFBX();
		inline void SetFileName(const std::string& path) { filePath = path; }

		std::string filePath;

		std::vector<std::shared_ptr<Mesh>> meshes;

		PrimitiveType::Id primitiveType;

		friend class ModelLoader; // for setting file name
	};
};