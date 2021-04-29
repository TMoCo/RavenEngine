//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "PrimitiveComponent.h"
#include "MeshRenderer.h"

#include "ResourceManager/ResourceManager.h"

#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Scene/Entity/EntityManager.h"
#include "Utilities/StringUtils.h"
#include "Animation/SkeletonCache.h"
#include "MeshRenderer.h"

#include "Engine.h"


namespace Raven {



PrimitiveComponent::PrimitiveComponent()
{
	
}


PrimitiveComponent::~PrimitiveComponent()
{

}


void PrimitiveComponent::SetMaterial(uint32_t index, Ptr<Material> mat)
{
	if (materials.size() < index + 1)
		materials.resize(index + 1);

	materials[index] = mat;
}


Material* PrimitiveComponent::GetMaterial(uint32_t index)
{
	// Invalid Index?
	if (index >= materials.size() || index < 0)
		return nullptr;

	return materials[index].get();
}


const Material* PrimitiveComponent::GetMaterial(uint32_t index) const
{
	return const_cast<PrimitiveComponent*>(this)->GetMaterial(index);
}


void PrimitiveComponent::CollectRenderPrimitives(RenderPrimitiveCollector& rcollector)
{

}




} // End of namespace Raven 
