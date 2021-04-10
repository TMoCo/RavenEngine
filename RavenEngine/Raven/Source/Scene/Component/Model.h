//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "Utilities/Core.h"
#include "ResourceManager/Resources/Mesh.h"
#include "ResourceManager/MeshFactory.h"
#include <string>
#include <vector>
#include <memory>

namespace Raven
{
	class Mesh;
	class Material;


	// Model:
	//	- 3D models and their related data such as meshes, and later materials
	//	- Represent a mesh in the scene with material.
	//
	class Model
	{
		// Friend loader for setting file name
		friend class ModelLoader; 

	public:
		Model() {}
		Model(const std::string & fileName);

		~Model();

		void LoadOnGpu();

		// Return a pointer to
		Ptr<Mesh> GetMesh(size_t index);

		// Retun the list meshes in the model.
		inline auto& GetMeshes() { return meshes; };
		inline const auto& GetMeshes() const { return meshes; };

		// Add new mesh to the list of meshes in the model.
		void AddMesh(Ptr<Mesh> mesh);

		// Add new meshes to the list of meshes in the model.
		void AddMeshes(const std::vector< Ptr<Mesh> >& inputMeshes);

		// Set material at index, the material is going to be used by a mesh on the same index.
		void SetMaterial(uint32_t index, Ptr<Material> mat);

		// Return materail at index, if no material at that index return null.
		Material* GetMaterial(uint32_t index);
		const Material* GetMaterial(uint32_t index) const;

		// Retun the list materials in the model.
		auto& GetMaterials() { return materials; }
		const auto& GetMaterials() const { return materials; }

		inline auto GetPrimitiveType() const { return primitiveType; }
		inline auto SetPrimitiveType(PrimitiveType::Id type) { primitiveType = type; }

		inline auto GetFileName() const { return filePath; }

		// serialization load and save
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
				Ptr<Mesh> newMesh(MeshFactory::CreatePrimitive(primitiveType));
				AddMesh(newMesh);
			}
			else
			{
				LoadFile();
			}
		}


	private:

		void LoadFile();

		inline void SetFileName(const std::string& path) { filePath = path; }

	private:
		std::string filePath;

		// The Model Meshes, each mapped to a materail on the same index.
		std::vector< Ptr<Mesh> > meshes;

		// The Model Materials, used by meshes mapped to the same index.
		std::vector< Ptr<Material> > materials;

		PrimitiveType::Id primitiveType;
	};
};