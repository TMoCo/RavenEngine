#pragma once



#include "Utilities/Core.h"
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
	class MeshRenderer;
	class SkinnedMeshRenderer;
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

	public:
		// serialization load and save
		template<typename Archive>
		void save(Archive& archive) const
		{
			archive(cereal::base_class<Component>(this));
			archive(clipDistance);
		}

		template<typename Archive>
		void load(Archive& archive)
		{
			archive(cereal::base_class<Component>(this));
			archive(clipDistance);
		}


	private:
		// The Model Materials, used by mesh mapped to the same index.
		std::vector< Ptr<Material> > materials;

	protected:
		// The local bounding box of the component.
		MathUtils::BoundingBox localBounds;

		// The distance this model will be clipped at, if -1 then no clipping.
		float clipDistance;
	};

};