#include "RenderRscShader.h"
#include "Utilities/Core.h"

#include "Render/OpenGL/GLShader.h"



namespace Raven {




uint32_t ShaderInput::TRANSFORM_BINDING = 0;
uint32_t ShaderInput::LGHTING_BINDING = 1;





// -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - 
// -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - 



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

		shader->Use();
		AddInput(ShaderInput::Sampler, "inHeightMap", 0);
		AddInput(ShaderInput::Uniform, "inHeightFactor");
		AddInput(ShaderInput::Uniform, "inHeightMap");
	}
		break;


	case Raven::ERenderShaderType::Debug:
	{
		shader->SetSourceFile(EGLShaderStage::Vertex, "shaders/DrawDebugVert.glsl");
		shader->SetSourceFile(EGLShaderStage::Fragment, "shaders/DrawDebugFrag.glsl");
		shader->Build();

		shader->Use();
		AddInput(ShaderInput::UniformBlock, "TransformBlock", ShaderInput::TRANSFORM_BINDING);
	}
		break;

	case Raven::ERenderShaderType::MaterialOpaque:
	{
		shader->SetSourceFile(EGLShaderStage::Vertex, "shaders/GeneralVert.glsl");
		shader->SetSourceFile(EGLShaderStage::Fragment, "shaders/PhongFrag.glsl");
		shader->Build();

		// Setup Input...
		shader->Use();

		// Global...
		AddInput(ShaderInput::UniformBlock, "TransformBlock", ShaderInput::TRANSFORM_BINDING);
		AddInput(ShaderInput::UniformBlock, "LightingBlock", ShaderInput::LGHTING_BINDING);

		// 
		AddInput(ShaderInput::UniformBlock, "MaterailBlock", 2);
		AddInput(ShaderInput::Sampler, "inDiffuseTexture", 0);
		AddInput(ShaderInput::Sampler, "inSpecularTexture", 1);

	}
		break;

	case Raven::ERenderShaderType::MaterialMasked:
	case Raven::ERenderShaderType::MaterialTranslucent:
	case Raven::ERenderShaderType::PostProcessing:
		RAVEN_ASSERT(0, "Material type not supported yet.");
		break;
	}
}


void RenderRscShader::AddInput(ShaderInput::EInputType type, const std::string& name, uint32_t binding)
{
	if (type == ShaderInput::UniformBlock)
	{
		shader->BindUniformBlock(name, binding);
	}
	else if (type == ShaderInput::Sampler)
	{
		shader->SetUniform(name, (int32_t)binding);
	}


	//
	ShaderInput indesc{};
	indesc.type = type;
	indesc.name = name;
	indesc.binding = binding;
	inputs.push_back(indesc);
}




} // End of namespace Raven.
