#include "RenderTerrain.h"
#include "Render/RenderResource/RenderRscTerrain.h"

#include "Render/OpenGL/GLShader.h"
#include "Render/OpenGL/GLVertexArray.h"
#include "Render/OpenGL/GLTexture.h"
#include "GL/glew.h"




namespace Raven {




RenderTerrain::RenderTerrain()
	: terrainRsc(nullptr)
{

}


RenderTerrain::~RenderTerrain()
{

}


RenderRscPrimitive* RenderTerrain::GetRsc()
{
	return terrainRsc;
}


void RenderTerrain::SetTerrainRsc(RenderRscTerrain* terrain)
{
	terrainRsc = terrain;
}



void RenderTerrain::Draw(GLShader* shader) const
{
	//
	shader->SetUniform("inScale", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	shader->SetUniform("inHeightFactor", terrainRsc->GetScale());
	shader->SetUniform("inHeightMap", terrainRsc->GetHeight());

	//
	terrainRsc->GetHeightMap()->Active(0);

	terrainRsc->GetArray()->Bind();
	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)terrainRsc->GetNumVerts());
	terrainRsc->GetArray()->Bind();
}



} // End of namespace Raven.
