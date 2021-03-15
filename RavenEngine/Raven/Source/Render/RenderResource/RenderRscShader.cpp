#include "RenderRscShader.h"
#include "Utilities/Core.h"

#include "Render/OpenGL/GLShader.h"



using namespace Raven;




RenderRscShader::RenderRscShader()
{

}


RenderRscShader::~RenderRscShader()
{

}


void RenderRscShader::Load(ERenderShaderType type, std::string name)
{
	//
	shader = std::shared_ptr<GLShader>( GLShader::Create(name) );

	//
	switch (type)
	{
	case Raven::ERenderShaderType::Terrain:
	{
		shader->SetSourceFile(EGLShaderStage::Vertex, "shaders/TerrainVert.glsl");
		shader->SetSourceFile(EGLShaderStage::Fragment, "shaders/TerrainFrag.glsl");
		shader->Build();
	}
		break;


	case Raven::ERenderShaderType::Debug:
	{
		shader->SetSourceFile(EGLShaderStage::Vertex, "shaders/DrawDebugVert.glsl");
		shader->SetSourceFile(EGLShaderStage::Fragment, "shaders/DrawDebugFrag.glsl");
		shader->Build();
	}
		break;

	case Raven::ERenderShaderType::MaterialOpaque:
	{

	}
		break;

	case Raven::ERenderShaderType::MaterialMasked:
	case Raven::ERenderShaderType::MaterialTranslucent:
	case Raven::ERenderShaderType::PostProcessing:
		RAVEN_ASSERT(0, "Material type not supported yet.");
		break;
	}
}
