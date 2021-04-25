

#include "RenderPrimitive.h"





namespace Raven {





RenderPrimitive::RenderPrimitive()
	: material(nullptr)
	, indexInScene(-1)
	, isSkinned(false)
{

}


RenderPrimitive::~RenderPrimitive()
{

}


void RenderPrimitive::SetWorldMatrix(const glm::mat4& mtx)
{
	worldMatrix = mtx;
}


void RenderPrimitive::SetNormalMatrix(const glm::mat4& mtx)
{
	normalMatrix = mtx;
}


void RenderPrimitive::SetMaterial(RenderRscMaterial* mat)
{
	material = mat;
}





} // End of namespace Raven.

