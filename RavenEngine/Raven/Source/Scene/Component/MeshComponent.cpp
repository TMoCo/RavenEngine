//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#pragma once



#include "MeshComponent.h"
#include "ResourceManager/Resources/Mesh.h"
#include "ResourceManager/Resources/Material.h"
#include "Render/RenderObjects/Primitives/RenderMesh.h"



namespace Raven {



MeshComponent::MeshComponent()
{
	
}


MeshComponent::~MeshComponent()
{

}


void MeshComponent::SetMesh(Ptr<Mesh> newMesh)
{
	mesh = newMesh;
	localBounds = mesh->GetBounds();
}


void MeshComponent::CollectRenderPrimitives(RenderPrimitiveCollector& rcollector)
{
	for (uint32_t i = 0; i < mesh->GetNumSections(); ++i)
	{
		MeshSection* meshSection = mesh->GetMeshSection(i);

		// Invalid?
		if (!meshSection)
		{
			continue;
		}

		// Add mesh section to be render.
		RenderMesh* rmesh = rcollector.NewMesh();
		rmesh->SetMesh( meshSection->renderRscMesh.get() );

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

			rmesh->SetMaterial( mat->GetRenderRsc() );
		}
	}
}


} // End of namespace Raven 
