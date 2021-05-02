#pragma once


#include "Utilities/Core.h"
#include "PrimitiveComponent.h"


#include <string>
#include <vector>
#include <memory>






namespace Raven
{
	class SkinnedMesh;
	class SkeletonInstance;




	// SkinnedMeshComponent:
	//	- 
	//
	class SkinnedMeshComponent : public PrimitiveComponent
	{
	public:
		// Construct.
		SkinnedMeshComponent();

		// Destruct.
		~SkinnedMeshComponent();

		// Retun the list mesh in the model.
		inline auto& GetMesh() { return mesh; };
		inline const auto& GetMesh() const { return mesh; };

		// Set a new mesh to the list of meshes in the model.
		void SetMesh(Ptr<SkinnedMesh> newMesh);

		// Called by the render to collect render primitives for drawing.
		virtual void CollectRenderPrimitives(RenderPrimitiveCollector& rcollector) override;

		// Return the skeleton of this skinned mesh.
		inline SkeletonInstance* GetSkeleton() { return skeleton.get(); }

	public:
		// Serialization Save.
		template<typename Archive>
		void save(Archive& archive) const
		{
			RAVEN_ASSERT(0, "Not Implemented");
		}

		// Serialization Load.
		template<typename Archive>
		void load(Archive& archive)
		{
			RAVEN_ASSERT(0, "Not Implemented");
		}


	private:
		// The Model Mesh, each mapped to a materail on the same index.
		Ptr<SkinnedMesh> mesh;

		// The instance of the skeleton used to update the bone transforms.
		Ptr<SkeletonInstance> skeleton;
	};

};