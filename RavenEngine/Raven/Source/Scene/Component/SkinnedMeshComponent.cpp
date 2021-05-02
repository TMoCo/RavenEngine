#include "SkinnedMeshComponent.h"

#include "ResourceManager/Resources/SkinnedMesh.h"
#include "ResourceManager/Resources/Material.h"
#include "Animation/Skeleton.h"

#include "Render/RenderObjects/Primitives/RenderSkinnedMesh.h"


namespace Raven {



SkinnedMeshComponent::SkinnedMeshComponent()
{
	
}


SkinnedMeshComponent::~SkinnedMeshComponent()
{

}


void SkinnedMeshComponent::SetMesh(Ptr<SkinnedMesh> newMesh)
{
	mesh = newMesh;

	// Has Valid Skeleton?
	if (mesh->GetSkeleton())
	{
		skeleton = Ptr<SkeletonInstance>(new SkeletonInstance(mesh->GetSkeleton()));
		skeleton->UpdateBones();
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
