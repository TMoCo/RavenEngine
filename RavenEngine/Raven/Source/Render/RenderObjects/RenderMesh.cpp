#include "RenderMesh.h"
#include "Render/RenderResource/RenderRscMesh.h"


#include "Render/OpenGL/GLVertexArray.h"
#include "GL/glew.h"



namespace Raven {



RenderMesh::RenderMesh()
	: mesh(nullptr)
{

}


RenderMesh::~RenderMesh()
{

}


void RenderMesh::SetMesh(RenderRscMesh* inMesh)
{
	mesh = inMesh;
}


void RenderMesh::Draw(GLShader* shader) const
{
	mesh->GetArray()->Bind();
	glDrawElements(GL_TRIANGLES, mesh->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
}


} // End of namespace Raven.
