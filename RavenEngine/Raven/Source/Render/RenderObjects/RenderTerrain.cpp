#include "RenderTerrain.h"
#include "Render/RenderResource/RenderRscTerrain.h"

#include "Render/OpenGL/GLShader.h"
#include "Render/OpenGL/GLVertexArray.h"
#include "Render/OpenGL/GLTexture.h"
#include "GL/glew.h"


using namespace Raven;



RenderTerrain::RenderTerrain()
	: height(0.0f)
	, terrainRsc(nullptr)
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


void RenderTerrain::SetHeight(float value)
{
	height = value;
}


void RenderTerrain::Draw(GLShader* shader) const
{
	shader->SetUniform("color", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	shader->SetUniform("scale", terrainRsc->GetScale());
	shader->SetUniform("heightFactor", 50.0f);
	shader->SetUniform("heightMap", 0);

	terrainRsc->GetHeightMap()->Active(0);

	terrainRsc->GetArray()->Bind();
	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)terrainRsc->GetNumVerts());
	terrainRsc->GetArray()->Bind();
}
