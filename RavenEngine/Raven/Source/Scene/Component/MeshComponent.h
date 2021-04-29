//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#pragma once


#include "Utilities/Core.h"
#include "PrimitiveComponent.h"



#include <string>
#include <vector>
#include <memory>





namespace Raven
{
	class Mesh;



	// MeshComponent:
	//	- 
	//
	class MeshComponent : public PrimitiveComponent
	{
	public:
		// Construct.
		MeshComponent();

		// Destruct.
		~MeshComponent();

		// Retun the list mesh in the model.
		inline auto& GetMesh() { return mesh; };
		inline const auto& GetMesh() const { return mesh; };

		// Set a new mesh to the list of meshes in the model.
		void SetMesh(Ptr<Mesh> newMesh);

		// Called by the render to collect render primitives for drawing.
		virtual void CollectRenderPrimitives(RenderPrimitiveCollector& rcollector) override;

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
		// The model mesh, each mesh section is mapped to a material.
		Ptr<Mesh> mesh;

	};

};