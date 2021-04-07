#include "RenderRscShader.h"
#include "Utilities/Core.h"

#include "Render/OpenGL/GLShader.h"




#define IMPORT_START_SORT_TAG 1000 
#define IMPORT_COMMON_TAG 0
#define IMPORT_TRANSFORM_VERTEX_TAG 1 
#define IMPORT_MATERIAL_FUNCTION_BASE_TAG 2 
#define IMPORT_LIGHTING_TAG 3 








namespace Raven {




RenderRscShader::RenderRscShader()
{

}


RenderRscShader::~RenderRscShader()
{

}


RenderRscShader* RenderRscShader::Create(ERenderShaderDomain domain, const RenderRscShaderCreateData& data)
{
	RenderRscShader* rsc = new RenderRscShader();
	rsc->domain = domain;
	rsc->type = data.type;
	rsc->shader = std::shared_ptr<GLShader>( GLShader::Create(data.name) );


	// Default Imports...
	rsc->shader->AddExSourceFile(IMPORT_COMMON_TAG, EGLShaderStageBit::All, "shaders/Common.glsl");

	// Setup the shaders for their domain and type
	rsc->SetupShaderForDomain();
	rsc->SetupShaderForType();

	// Materail Function Override.
	EGLShaderStageBit funcStages = data.materialFunction.first;

	if (funcStages != EGLShaderStageBit::None)
	{
		// Override Vertex Function?
		if ((funcStages & EGLShaderStageBit::VertexBit) == EGLShaderStageBit::VertexBit)
			rsc->shader->AddPreprocessor("#define MATERIAL_VERTEX_OVERRIDE 1");

		// Override Fragment Function?
		if ((funcStages & EGLShaderStageBit::FragmentBit) == EGLShaderStageBit::FragmentBit)
			rsc->shader->AddPreprocessor("#define MATERIAL_FUNCTION_OVERRIDE 1");

		// Add The Function
		rsc->shader->AddExSourceFile(IMPORT_START_SORT_TAG + 1,
			data.materialFunction.first, data.materialFunction.second);
	}

	// Build OpenGL Shader/Program.
	rsc->shader->Build();

	return rsc;
}


RenderRscShader* RenderRscShader::CreateCustom(const RenderRscShaderDomainCreateData& domain, const RenderRscShaderCreateData& data)
{
	RenderRscShader* rsc = new RenderRscShader();
	rsc->domain = ERenderShaderDomain::Custom;
	rsc->type = data.type;
	rsc->shader = std::shared_ptr<GLShader>(GLShader::Create(data.name));

	// Default Imports...
	rsc->shader->AddExSourceFile(0, EGLShaderStageBit::All, "shaders/Common.glsl");

	// Add all improts as external source...
	for (uint32_t i = 0; i < domain.imports.size(); ++i)
	{
		rsc->shader->AddExSourceFile(IMPORT_START_SORT_TAG + i,
			domain.imports[i].first, domain.imports[i].second);
	}

	// Add all Preprocessors
	for (const auto& pre : domain.preprocessors)
	{
		rsc->shader->AddPreprocessor(pre);
	}

	// Custom Domain Main Source
	for (const auto& src : domain.sources)
	{
		rsc->shader->SetSourceFile(src.first, src.second);
	}

	// Build OpenGL Shader/Program.
	rsc->shader->Build();
	rsc->shader->Use();

	return rsc;
}


void RenderRscShader::SetupShaderForDomain()
{
	switch (domain)
	{
	case ERenderShaderDomain::Mesh:
	{
		shader->AddExSourceFile(IMPORT_TRANSFORM_VERTEX_TAG,
			EGLShaderStageBit::VertexBit,
			"shaders/VertexTransform.glsl");

		shader->AddExSourceFile(IMPORT_LIGHTING_TAG,
			EGLShaderStageBit::FragmentBit,
			"shaders/Lighting.glsl");

		shader->AddExSourceFile(IMPORT_MATERIAL_FUNCTION_BASE_TAG,
			EGLShaderStageBit::VertexBit | EGLShaderStageBit::FragmentBit,
			"shaders/Materials/MaterialFunctions.glsl");
		
		// Main Source...
		shader->SetSourceFile(EGLShaderStage::Vertex, "shaders/MeshVert.glsl");
		shader->SetSourceFile(EGLShaderStage::Fragment, "shaders/MeshFrag.glsl");

		// Add Input...
		input.AddBlockInput(RenderShaderInput::CommonBlock);
		input.AddBlockInput(RenderShaderInput::TransfromBlock);
		input.AddBlockInput(RenderShaderInput::LightingBlock);
	}
		break;


	case ERenderShaderDomain::Terrain:
	{
		shader->AddExSourceFile(IMPORT_TRANSFORM_VERTEX_TAG,
			EGLShaderStageBit::VertexBit,
			"shaders/VertexTransform.glsl");

		shader->AddExSourceFile(IMPORT_MATERIAL_FUNCTION_BASE_TAG,
			EGLShaderStageBit::VertexBit | EGLShaderStageBit::FragmentBit,
			"shaders/Materials/MaterialFunctions.glsl");

		shader->SetSourceFile(EGLShaderStage::Vertex, "shaders/TerrainVert.glsl");
		shader->SetSourceFile(EGLShaderStage::Fragment, "shaders/TerrainFrag.glsl");

		// Add Input...
		input.AddBlockInput(RenderShaderInput::CommonBlock);
		input.AddBlockInput(RenderShaderInput::TransfromBlock);
	}
		break;


	case ERenderShaderDomain::Skeletal:
		RAVEN_ASSERT(0, "Shader Skeletal Domain not supported yet.");
		break;
	}

}


void RenderRscShader::SetupShaderForType()
{

}


void RenderRscShader::BindBlockInputs()
{
	for (uint32_t i = 0; i < input.GetNumBlocks(); ++i)
	{
		const auto& block = input.GetBlockInput(i);
		shader->BindUniformBlock(block.name, block.binding);
	}
}


void RenderRscShader::BindSamplers()
{
	shader->Use();

	for (uint32_t i = 0; i < input.GetNumSamplers(); ++i)
	{
		const auto& samplers = input.GetSamplerInput(i);
		shader->SetUniform(samplers.name, static_cast<int32_t>(i));
	}
}



} // End of namespace Raven.
