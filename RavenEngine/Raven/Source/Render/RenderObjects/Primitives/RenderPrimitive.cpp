#include "RenderPrimitive.h"
#include "Render/RenderResource/Shader/RenderRscMaterial.h"





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


ERenderShaderDomain RenderPrimitive::GetMaterialDomain() const
{
	return material->GetShaderRsc()->GetDomain();
}


ERenderShaderType RenderPrimitive::GetShaderType() const
{
	return material->GetShaderRsc()->GetType();
}


} // End of namespace Raven.

