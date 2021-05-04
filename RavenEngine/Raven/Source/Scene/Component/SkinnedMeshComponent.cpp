#include "SkinnedMeshComponent.h"

#include "ResourceManager/Resources/SkinnedMesh.h"
#include "ResourceManager/Resources/Material.h"
#include "Animation/Skeleton.h"
#include "Scene/Entity/Entity.h"

#include "Render/RenderObjects/Primitives/RenderSkinnedMesh.h"


namespace Raven {



SkinnedMeshComponent::SkinnedMeshComponent()
	: isLoading(false)
{
	
}


SkinnedMeshComponent::~SkinnedMeshComponent()
{

}


void SkinnedMeshComponent::SetMesh(Ptr<SkinnedMesh> newMesh)
{
	Entity entity = GetEntity();

	// Cleanup...
	if (skeleton)
	{
		skeleton->DestroyTransformHierarchy();
	}

	// Set new...
	mesh = newMesh;

	// Has Valid Skeleton?
	if (mesh->GetSkeleton())
	{
		skeleton = Ptr<SkeletonInstance>(new SkeletonInstance(this, mesh->GetSkeleton()));

		if (!isLoading)
		{
			skeleton->BuildTransformHierarchy();
			skeleton->UpdateBones();
		}
	}

	localBounds = mesh->GetBounds();
}


void SkinnedMeshComponent::CollectRenderPrimitives(RenderPrimitiveCollector& rcollector)
{
	for (uint32_t i = 0; i < mesh->GetNumSections(); ++i)
	{
		SkinnedMeshSection* meshSection = mesh->GetMeshSection(i);

		// Invalid?
		if (!meshSection)
		{
			continue;
		}



		// Add mesh section to be render.
		RenderSkinnedMesh* rmesh = rcollector.NewSkinnedMesh();
		rmesh->SetMesh(meshSection->renderRscMesh.get());
		RAVEN_ASSERT(skeleton->GetBones()->size() <= RENDER_SKINNED_MAX_BONES, "");
		rmesh->SetBones(skeleton->GetBones());

		// Material to use while rendering the mesh section.
		Material* mat = GetMaterial(i);

		// Has Material?
		if (mat)
		{
			// Update Material Paramters if Dirty.
			if (mat->IsDirty())
			{
				mat->UpdateRenderResource();
			}

			rmesh->SetMaterial(mat->GetRenderRsc());
		}
	}
}


} // End of namespace Raven 
