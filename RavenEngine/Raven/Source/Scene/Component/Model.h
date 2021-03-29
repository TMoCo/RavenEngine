//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ResourceManager/Resources/Mesh.h"
#include "ResourceManager/MeshFactory.h"

//
// A class for 3D models and their related data such as meshes, and later materials
//

namespace Raven
{
	enum class PrimitiveType;
	class Model
	{
	public:
		Model() {} 

		~Model();

		void LoadOnGpu();

		// return shared pointer to a mesh resource
		std::shared_ptr<Mesh> GetMesh(size_t index);
		// pointer to the vector of meshes
		std::vector<std::shared_ptr<Mesh>>& GetMeshes();

		void AddMesh(Mesh* mesh);
		void AddMeshes(std::vector<std::shared_ptr<Mesh>> inputMeshes);

		inline auto GetPrimitiveType() const { return primitiveType; }
		inline auto SetPrimitiveType(PrimitiveType type) { primitiveType = type; }

		inline auto GetFileName() const { return filePath; }

		template<typename Archive>
		void save(Archive& archive) const
		{
			if (meshes.size() > 0)
			{
				archive(cereal::make_nvp("PrimitiveType", primitiveType), cereal::make_nvp("FilePath", filePath));
			}
		}

		template<typename Archive>
		void load(Archive& archive)
		{

			archive(cereal::make_nvp("PrimitiveType", primitiveType), cereal::make_nvp("FilePath", filePath));

			meshes.clear();

			if (primitiveType != PrimitiveType::File)
			{
				AddMesh(MeshFactory::CreatePrimative(primitiveType));
			}
			else
			{
				//TODO
				LOGE("load from file did not implementation");
			}
		}


	private:

		inline void SetFileName(const std::string& path) { filePath = path; }

		std::vector<std::shared_ptr<Mesh>> meshes;
		std::string filePath;
		PrimitiveType primitiveType;

		friend class ModelLoader; // for setting file name
	};
}