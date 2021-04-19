#include "RenderRscShader.h"
#include "Utilities/Core.h"

#include "Render/OpenGL/GLShader.h"




#define IMPORT_START_SORT_TAG 1000 
#define IMPORT_COMMON_TAG 0
#define IMPORT_TRANSFORM_VERTEX_TAG 1 
#define IMPORT_MATERIAL_FUNCTION_BASE_TAG 2 
#define IMPORT_COMMON_LIGHTING_TAG 4 
#define IMPORT_LIGHTING_TAG 5 










namespace Raven {




RenderRscShader::RenderRscShader()
	: domain(ERenderShaderDomain::Custom)
	, type(ERenderShaderType::Opaque)
	, renderBatchIndex((uint32_t)-1)
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


	// ...
	rsc->shader->AddPreprocessor("#define RENDER_SHADER_CUSTOM 1");


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

		shader->AddExSourceFile(IMPORT_MATERIAL_FUNCTION_BASE_TAG,
			EGLShaderStageBit::VertexBit | EGLShaderStageBit::FragmentBit,
			"shaders/Materials/MaterialFunctions.glsl");
		
		// Main Source...
		shader->SetSourceFile(EGLShaderStage::Vertex, "shaders/MeshVert.glsl");
		shader->SetSourceFile(EGLShaderStage::Fragment, "shaders/MeshFrag.glsl");

		// Add Input...
		input.AddBlockInput(RenderShaderInput::CommonBlock);
		input.AddBlockInput(RenderShaderInput::TransformBlock);
	}
		break;


	case ERenderShaderDomain::Terrain:
	{
		RAVEN_ASSERT(type == ERenderShaderType::Opaque, "Terrain can only be Opaque.");

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
		input.AddBlockInput(RenderShaderInput::TransformBlock);
	}
		break;


	case ERenderShaderDomain::Skinned:
	{
		shader->AddExSourceFile(IMPORT_TRANSFORM_VERTEX_TAG,
			EGLShaderStageBit::VertexBit,
			"shaders/VertexTransform.glsl");

		shader->AddExSourceFile(IMPORT_MATERIAL_FUNCTION_BASE_TAG,
			EGLShaderStageBit::VertexBit | EGLShaderStageBit::FragmentBit,
			"shaders/Materials/MaterialFunctions.glsl");

		// Max Bones..
		shader->AddPreprocessor("#define RENDER_BONE_TRANSFORM ");
		shader->AddPreprocessor("#define RENDER_SKINNED_MAX_BONES " + std::to_string(RENDER_SKINNED_MAX_BONES));

		// Main Source...
		shader->SetSourceFile(EGLShaderStage::Vertex, "shaders/SkeletonVert.glsl");
		shader->SetSourceFile(EGLShaderStage::Fragment, "shaders/MeshFrag.glsl");

		// Add Input...
		input.AddBlockInput(RenderShaderInput::CommonBlock);
		input.AddBlockInput(RenderShaderInput::TransformBoneBlock);
	}
		break;
	}

}


void RenderRscShader::SetupShaderForType()
{
	switch (type)
	{
	case ERenderShaderType::Opaque:
	{
		shader->AddPreprocessor("#define RENDER_PASS_DEFERRED 1");
		shader->AddPreprocessor("#define RENDER_SHADER_TYPE_OPAQUE 1");
	}
		break;

	case ERenderShaderType::Masked:
	{
		shader->AddPreprocessor("#define RENDER_PASS_DEFERRED 1");
		shader->AddPreprocessor("#define RENDER_SHADER_TYPE_MASKED 1");
	}
		break;

	case ERenderShaderType::Translucent:
	{
		shader->AddPreprocessor("#define RENDER_PASS_FORWARD 1");
		shader->AddPreprocessor("#define RENDER_SHADER_TYPE_TRANSLUCENT 1");

		// Lighting Shader.
		shader->AddExSourceFile(IMPORT_COMMON_LIGHTING_TAG, EGLShaderStageBit::FragmentBit,
			"shaders/CommonLight.glsl");

		shader->AddExSourceFile(IMPORT_LIGHTING_TAG, EGLShaderStageBit::FragmentBit,
			"shaders/Lighting.glsl");

		shader->AddPreprocessor("#define MAX_LIGHTS " + std::to_string(RENDER_PASS_FORWARD_MAX_LIGHTS));
		input.AddBlockInput(RenderShaderInput::LightingBlock_FORWARD);
		input.AddSamplerInput("inSkyEnvironment");
		input.AddSamplerInput("inEnvBRDF");
	}
		break;

	case ERenderShaderType::PostProcessing:
	{
		RAVEN_ASSERT(0, "Shader PostProcessing type not supported yet.");
		shader->AddPreprocessor("#define RENDER_SHADER_TYPE_POSTPROCESSING 1");
	}
		break;
	}
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
