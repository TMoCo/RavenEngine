#include "RenderPrimitiveCollector.h"

#include "RenderScene.h"
#include "Primitives/RenderMesh.h"
#include "Primitives/RenderSkinnedMesh.h"





namespace Raven {


RenderPrimitiveCollector::RenderPrimitiveCollector(RenderScene* sceneOwner)
	: owner(sceneOwner)
	, worldMatrix(nullptr)
	, normalMatrix(nullptr)
{

}


void RenderPrimitiveCollector::Reset()
{
	primitive.clear();
}


RenderMesh* RenderPrimitiveCollector::NewMesh()
{
	RenderMesh* rmesh = owner->NewPrimitive<RenderMesh>();
	rmesh->SetWorldMatrix(*worldMatrix);
	rmesh->SetNormalMatrix(*normalMatrix);
	primitive.push_back(rmesh);

	return rmesh;
}


RenderSkinnedMesh* RenderPrimitiveCollector::NewSkinnedMesh()
{
	RenderSkinnedMesh* rskinned = owner->NewPrimitive<RenderSkinnedMesh>();
	rskinned->SetWorldMatrix(*worldMatrix);
	rskinned->SetNormalMatrix(*normalMatrix);
	primitive.push_back(rskinned);

	return rskinned;
}


} // End of namespace Raven.


