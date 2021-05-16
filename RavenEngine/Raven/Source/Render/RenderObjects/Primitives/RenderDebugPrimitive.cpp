#include "RenderDebugPrimitive.h"
#include "Render/RenderResource/Primitives/RenderRscDebugMesh.h"

#include "Render/OpenGL/GLVertexArray.h"
#include "Render/OpenGL/GLShader.h"
#include "GL/glew.h"



namespace Raven {





RenderDebugPrimitive::RenderDebugPrimitive()
	: mesh(nullptr)
{

}


RenderDebugPrimitive::~RenderDebugPrimitive()
{

}


RenderRscPrimitive* RenderDebugPrimitive::GetRsc()
{
	return mesh;
}


void RenderDebugPrimitive::SetDebugMesh(RenderRscDebugMesh* prim)
{
	mesh = prim;
}



void RenderDebugPrimitive::Draw(GLShader* shader) const
{
	shader->SetUniform("color", color);

	mesh->GetArray()->Bind();
	glDrawElements(GL_TRIANGLES, (GLsizei)mesh->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
}




} // End of namespace Raven.
