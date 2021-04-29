//////////////////////////////////////////////////////////////////////////////
// This file is part of the Raven Game Engine			                    //
//////////////////////////////////////////////////////////////////////////////



#include "MaterialShader.h"
#include "Render/RenderResource/Shader/UniformBuffer.h"



namespace Raven {




MaterialShader::MaterialShader()
	: IResource()
	, renderRsc(nullptr)
	, domain(ERenderShaderDomain::Mesh)
	, type(ERenderShaderType::Opaque)
	, isComputeMaterialVertex(false)
{
	type = MaterialShader::GetType();
	hasRenderResources = true;
}


MaterialShader::~MaterialShader()
{
	delete renderRsc;
}



void MaterialShader::LoadRenderResource()
{
	RAVEN_ASSERT(!isOnGPU, "Shader already loaded.");
	isOnGPU = true;


	// What Stages this materail function support.
	EGLShaderStageBit stages = EGLShaderStageBit::FragmentBit;

	// Has comute vertex material function?
	if (isComputeMaterialVertex)
	{
		stages = stages | EGLShaderStageBit::VertexBit;
	}

	RenderRscShaderCreateData rscData;
	rscData.name = name;
	rscData.type = type;
	rscData.AddFunction(stages, materialFunction);
	renderRsc = RenderRscShader::Create(domain, rscData); // Build Shader

	// Shader Input...
	if (blockInput.size != -1)
	{
		blockInput.binding = RenderShaderInput::MaterialBlockBinding;
		renderRsc->GetInput().AddBlockInput(blockInput);

		// Create a unifrom buffer for the block input.
		materialUBO = Ptr<UniformBuffer>(UniformBuffer::Create(blockInput, false));
	}

	renderRsc->GetInput().AddSamplerInputs(samplers);
	renderRsc->BindBlockInputs();
	renderRsc->BindSamplers();

}


void MaterialShader::SetName(const std::string& name)
{
	this->name = name;
}


void MaterialShader::SetMaterialFunction(const std::string& func, bool hasComputeVertex)
{
	materialFunction = func;
	isComputeMaterialVertex = hasComputeVertex;
}


void MaterialShader::SetDomain(ERenderShaderDomain val)
{
	RAVEN_ASSERT(val != ERenderShaderDomain::Custom, "Custom Domain not supported by shader resource.");
	domain = val;
}


void MaterialShader::SetType(ERenderShaderType val)
{
	type = val;
}


void MaterialShader::AddSampler(const std::string& name)
{
	RSInputDescription inputSampler;
	inputSampler.uniformType = EShaderUniformType::Sampler;
	inputSampler.inputType = EShaderInputType::Int;
	inputSampler.name = name;
	samplers.push_back(inputSampler);
}


} // End of namespace Raven.
