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
	//	-
	//
	class PrimitiveComponent : public Component
	{
	public:
		// Construct.
		PrimitiveComponent();

		// Destruct.
		~PrimitiveComponent();

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

	public:
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


	private:
		// The Model Materials, used by mesh mapped to the same index.
		std::vector< Ptr<Material> > materials;

	protected:
		// The local bounding box of the component.
		MathUtils::BoundingBox localBounds;
	};

};