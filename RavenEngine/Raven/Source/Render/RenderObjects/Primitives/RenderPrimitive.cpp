

#include "RenderPrimitive.h"





using namespace Raven;





RenderPrimitive::RenderPrimitive()
	: material(nullptr)
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
