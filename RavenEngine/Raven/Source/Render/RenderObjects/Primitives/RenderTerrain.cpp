#include "RenderTerrain.h"
#include "Render/RenderResource/Primitives/RenderRscTerrain.h"

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
	shader->SetUniform("inScale", terrainRsc->GetScale());
	shader->SetUniform("inHeight", glm::vec2(terrainRsc->GetMinHeight(), terrainRsc->GetMaxHeight()));
	shader->SetUniform("inHeightMap", 0);

	//
	terrainRsc->GetHeightMap()->Active(0);

	terrainRsc->GetArray()->Bind();
	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)terrainRsc->GetNumVerts());
	terrainRsc->GetArray()->Bind();
}



} // End of namespace Raven.
