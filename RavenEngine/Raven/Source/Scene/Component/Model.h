//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ResourceManager/Resources/Mesh.h"

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

	private:

		inline void SetFileName(const std::string& path) { filePath = path; }

		std::vector<std::shared_ptr<Mesh>> meshes;
		std::string filePath;
		PrimitiveType primitiveType;

		friend class ModelLoader; // for setting file name
	};
}