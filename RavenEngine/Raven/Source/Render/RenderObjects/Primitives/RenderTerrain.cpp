#include "RenderTerrain.h"
#include "Render/RenderResource/Primitives/RenderRscTerrain.h"
#include "Render/RenderResource/Primitives/RenderRscMesh.h"
#include "Render/RenderResource/Shader/RenderRscMaterial.h"
#include "Render/RenderResource/Shader/UniformBuffer.h"

#include "ResourceManager/Resources/DynamicTexture.h"

#include "Render/OpenGL/GLShader.h"
#include "Render/OpenGL/GLVertexArray.h"
#include "Render/OpenGL/GLTexture.h"
#include "GL/glew.h"




namespace Raven {

// --- --- --- --- ---- ---- ---- 

struct TerrainBinData
{
	glm::vec2 scale;
	glm::vec2 height;
	glm::vec2 offset;
	glm::vec2 uvScale;
}binData;



// --- --- --- --- ---- ---- ---- 



RenderTerrain::RenderTerrain()
	: terrainRsc(nullptr)
	, binIndex(0)
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


void RenderTerrain::Draw(GLShader* shader, bool isShadow) const
{
	// Quads Tesselation Patches
	glPatchParameteri(GL_PATCH_VERTICES, 4);


	if (isShadow)
	{
		glEnable(GL_CULL_FACE);
	}

	// Binding...
	DynamicTexture* heightMap = terrainRsc->GetHeightMap();
	heightMap->GetRenderRsc()->GetTexture()->Active(0);

	// Draw Terrain Bins...
	const auto& bin = (*terrainRsc->GetBins())[binIndex];

	binData.height = terrainRsc->GetHeight();
	binData.scale = terrainRsc->GetScale();
	binData.offset = bin.offset;
	binData.uvScale = bin.uvScale;

	terrainRsc->GetBinUB()->BindBase();
	terrainRsc->GetBinUB()->UpdateData(sizeof(binData), 0, &binData);

	// Draw Tesselation Patches...
	terrainRsc->GetArray()->Bind();
	glDrawElements(GL_PATCHES, (GLsizei)terrainRsc->GetNumIndices(), GL_UNSIGNED_INT, nullptr);

	//glEnable(GL_POLYGON_MODE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glDrawElements(GL_PATCHES, (GLsizei)terrainRsc->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//glEnable(GL_POLYGON_MODE);


	if (isShadow)
	{
		glDisable(GL_CULL_FACE);
	}
}



// ------------------------------------------------------------------------------



RenderTerrainFoliage::RenderTerrainFoliage()
	: instanceRsc(nullptr)
{

}



RenderTerrainFoliage::~RenderTerrainFoliage()
{

}


void RenderTerrainFoliage::SetMeshRsc(RenderRscMeshInstance* rsc)
{
	instanceRsc = rsc;
}


RenderRscPrimitive* RenderTerrainFoliage::GetRsc()
{
	return instanceRsc;
}


void RenderTerrainFoliage::SetInstanceCount(int32_t inCount)
{
	count = inCount;
}


void RenderTerrainFoliage::Draw(GLShader* shader, bool isShadow) const
{
	instanceRsc->GetArray()->Bind();
	glDrawElementsInstanced(GL_TRIANGLES, instanceRsc->GetNumIndices(), GL_UNSIGNED_INT, nullptr, count);
}


} // End of namespace Raven.
