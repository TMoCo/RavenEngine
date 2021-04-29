//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "Utilities/Core.h"
#include "Component.h"
#include "Animation/Bone.h"
#include "ResourceManager/Resources/Mesh.h"
#include "ResourceManager/MeshFactory.h"
#include "ResourceManager/Resources/Fbx.h"


#include <string>
#include <vector>
#include <memory>





namespace Raven
{
	class Entity;
	class Mesh;
	class Material;
	class MeshRenderer;
	class SkinnedMeshRenderer;
	class Scene;


	// Data of MeshRenderer as viewed by the model
	struct ModelMeshRendererData
	{
		MeshRenderer* mesh;
		SkinnedMeshRenderer* skinned;

		ModelMeshRendererData()
			: mesh(nullptr)
			, skinned(nullptr)
		{

		}
	};



	// Model:
	//	- 3D models and their related data such as meshes, and materials
	//	- Represent a mesh in the scene with material.
	//
	class Model : public Component
	{
		// Friend loader for setting file name
		friend class ModelLoader; 

	public:
		// Construct.
		Model();

		// Destruct.
		~Model();

		// Retun the list mesh in the model.
		inline auto& GetMesh() { return mesh; };
		inline const auto& GetMesh() const { return mesh; };

		// Set a new mesh to the list of meshes in the model.
		inline void SetMesh(Ptr<Mesh> newMesh) { mesh = newMesh; }

		// Set material at index, the material is going to be used by a mesh on the same index.
		void SetMaterial(uint32_t index, Ptr<Material> mat);

		// Return materail at index, if no material at that index return null.
		Material* GetMaterial(uint32_t index);
		const Material* GetMaterial(uint32_t index) const;

		// Retun the list materials in the model.
		auto& GetMaterials() { return materials; }
		const auto& GetMaterials() const { return materials; }

		// Return the current local bounds of the model.
		inline const auto& GetLocalBounds() const { return mesh->GetBounds(); }

		// serialization load and save
		template<typename Archive>
		void save(Archive& archive) const
		{
			RAVEN_ASSERT(0, "Not Implemented");
		}

		template<typename Archive>
		void load(Archive& archive)
		{
			RAVEN_ASSERT(0, "Not Implemented");
		}

		// Return mesh renderers for at their mesh indices.
		void GetMeshRenderers(std::vector<ModelMeshRendererData>& outMesRenderers, Scene* scene);

	private:
		// Recursive Implemenation fo GetMeshRenderer
		void GetMeshRenderersImp(std::vector<ModelMeshRendererData>& outMesRenderers, Entity& ent);

	private:
		void BindMeshComponent();
		void BindMeshComponentForFBX();

	private:
		// The Model Mesh, each mapped to a materail on the same index.
		Ptr<Mesh> mesh;

		// The Model Materials, used by mesh mapped to the same index.
		std::vector< Ptr<Material> > materials;
	};

};