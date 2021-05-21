#include "RenderSkinnedMesh.h"
#include "Render/RenderResource/Primitives/RenderRscMesh.h"


#include "Render/OpenGL/GLVertexArray.h"
#include "GL/glew.h"



namespace Raven {



RenderSkinnedMesh::RenderSkinnedMesh()
	: mesh(nullptr)
{
	isSkinned = true;
}


RenderSkinnedMesh::~RenderSkinnedMesh()
{

}


void RenderSkinnedMesh::SetMesh(RenderRscSkinnedMesh* inMesh)
{
	mesh = inMesh;
}


RenderRscPrimitive* RenderSkinnedMesh::GetRsc()
{
	return mesh;
}


void RenderSkinnedMesh::Draw(GLShader* shader, bool isShadow) const
{
	mesh->GetArray()->Bind();
	glDrawElements(GL_TRIANGLES, mesh->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
}


} // End of namespace Raven.
