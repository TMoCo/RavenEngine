#include "RenderPrimitiveCollector.h"

#include "RenderScene.h"
#include "Primitives/RenderMesh.h"
#include "Primitives/RenderSkinnedMesh.h"





namespace Raven {


RenderPrimitiveCollector::RenderPrimitiveCollector(RenderScene* sceneOwner)
	: owner(sceneOwner)
{

}


void RenderPrimitiveCollector::Reset()
{
	primitive.clear();
}


RenderMesh* RenderPrimitiveCollector::NewMesh()
{
	RenderMesh* rmesh = owner->NewPrimitive<RenderMesh>();
	rmesh->SetWorldMatrix(worldMatrix);
	rmesh->SetNormalMatrix(normalMatrix);

	return rmesh;
}


RenderSkinnedMesh* RenderPrimitiveCollector::NewSkinnedMesh()
{
	RenderSkinnedMesh* rskinned = owner->NewPrimitive<RenderSkinnedMesh>();
	rskinned->SetWorldMatrix(worldMatrix);
	rskinned->SetNormalMatrix(normalMatrix);

	return rskinned;
}


} // End of namespace Raven.


