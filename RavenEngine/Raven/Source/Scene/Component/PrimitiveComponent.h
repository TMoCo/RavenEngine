#pragma once


#include "Utilities/Core.h"
#include "ResourceManager/RavenVersion.h"
#include "Component.h"

#include "Math/BoundingBox.h"
#include "Render/RenderObjects/RenderPrimitiveCollector.h"


#include <string>
#include <vector>
#include <memory>





namespace Raven
{
	class Mesh;
	class Material;
	class Scene;




	// PrimitiveComponent:
	//	- Parent class for all componenet that will be rendered like MeshComponent & SkinnedMeshComponent.
	//
	class PrimitiveComponent : public Component
	{
	public:
		// Construct.
		PrimitiveComponent();

		// Destruct.
		virtual ~PrimitiveComponent();

		// Set material at index, the material is going to be used by a mesh on the same index.
		void SetMaterial(uint32_t index, Ptr<Material> mat);

		// Return materail at index, if no material at that index return null.
		Material* GetMaterial(uint32_t index);
		const Material* GetMaterial(uint32_t index) const;

		// Retun the list materials in the model.
		auto& GetMaterials() { return materials; }
		const auto& GetMaterials() const { return materials; }

		// Return the current local bounds of the model.
		virtual const MathUtils::BoundingBox& GetLocalBounds() const { return localBounds; }

		// Called by the render to collect render primitives for drawing.
		virtual void CollectRenderPrimitives(RenderPrimitiveCollector& rcollector);

		// Get/Set Clip Distance.
		inline float GetClipDistance() { return clipDistance; }
		inline void SetClipDistance(float distance) { clipDistance = distance; }

		// Get/Set Cast Shadow.
		inline bool IsCastShadow() { return isCastShadow; }
		inline void SetCastShadow(bool val) { isCastShadow = val; }

	public:
		// serialization load and save
		template<typename Archive>
		void save(Archive& archive) const
		{
			archive(cereal::base_class<Component>(this));
			archive(clipDistance);

			// Saving Reference to materials.
			if (RavenVersionGlobals::SCENE_ARCHIVE_VERSION >= 10001)
			{
				uint32_t matCount = materials.size();
				archive(matCount);

				for (uint32_t i = 0; i < matCount; ++i)
				{
					// Save Resrouce Reference -> Material.
					ResourceRef::Save(archive, materials[i].get());
				}
			}
		}

		template<typename Archive>
		void load(Archive& archive)
		{
			archive(cereal::base_class<Component>(this));
			archive(clipDistance);

			// Load Reference to materials.
			if (RavenVersionGlobals::SCENE_ARCHIVE_VERSION >= 10001)
			{
				uint32_t matCount = 0;
				archive(matCount);
				materials.resize(matCount);

				for (uint32_t i = 0; i < matCount; ++i)
				{
					// Load Resrouce Reference -> Material.
					materials[i] = ResourceRef::Load(archive).FindOrLoad<Material>();
				}
			}
		}


	private:
		// The Model Materials, used by mesh mapped to the same index.
		std::vector< Ptr<Material> > materials;

	protected:
		// The local bounding box of the component.
		MathUtils::BoundingBox localBounds;

		// The distance this model will be clipped at, if -1 then no clipping.
		float clipDistance;

		// if true this primitive will cast shadow.
		bool isCastShadow;
	};

};