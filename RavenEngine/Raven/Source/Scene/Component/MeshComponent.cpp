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
	MeshLOD* meshLOD;

	// Has LODs?
	uint32_t numLODs = mesh->GetNumLODs();

	if (numLODs > 1)
	{
		uint32_t level;

		for (level = 1; level < numLODs; ++level)
		{
			if (rcollector.GetViewDistance() < mesh->GetMeshLOD(level).distance)
			{
				level -= 1;
				break;
			}
		}

		meshLOD = &mesh->GetMeshLOD(level);
	}
	else
	{
		meshLOD = &mesh->GetMeshLOD(0);
	}


	for (uint32_t i = 0; i < meshLOD->sections.size(); ++i)
	{
		MeshSection* meshSection = meshLOD->sections[i].get();

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
		else
		{
			auto& defaultMaterial = meshSection->defaultMaterial;

			// Has Default Material?
			if (defaultMaterial.IsValid())
			{
				mat = defaultMaterial.GetWeak<Material>();
			}
		}
	}
}


} // End of namespace Raven 
