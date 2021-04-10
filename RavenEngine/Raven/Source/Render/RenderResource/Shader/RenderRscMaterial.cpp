#include "RenderRscMaterial.h"
#include "RenderRscShader.h"
#include "UniformBuffer.h"

#include "Render/OpenGL/GLTexture.h"

#include "ResourceManager/Resources/Material.h"
#include "ResourceManager/Resources/Texture2D.h"



#include "glm/gtc/type_ptr.hpp"




namespace Raven {




RenderRscMaterial::RenderRscMaterial(RenderRscShader* shader)
	: shader(shader)
	, materialBuffer(nullptr)
	, blockIndex(-1)
	, ubo(nullptr)
{

}


RenderRscMaterial::~RenderRscMaterial()
{
	delete materialBuffer;
}


void RenderRscMaterial::LoadInputBlock(const std::string& inBlockName)
{
	RAVEN_ASSERT(shader != nullptr, "Updating Render Material with invalid shader.");
		
	// Find Block
	blockIndex = shader->GetInput().GetBlockInputIndex(inBlockName);

	// Delete materail buffer that match the old block.
	if (materialBuffer)
	{
		delete materialBuffer;
		materialBuffer = nullptr;
	}
}


void RenderRscMaterial::ClearMapping()
{
	matInputMap.clear();
	matInputTexturesMap.clear();
}


void RenderRscMaterial::MapParamter(const std::string& name, Texture2D** texture)
{
	int32_t idx = shader->GetInput().GetSamplerInputIndex(name);

	// No input with that name?
	if (idx == -1)
		return;

	std::pair<int32_t, Texture2D**> inputTex;
	inputTex.first = idx;
	inputTex.second = texture;
	matInputTexturesMap.push_back(inputTex);
}


void RenderRscMaterial::MapParamter(const std::string& name, const float* scalar)
{
	// No Input Block?
	if (blockIndex == -1)
		return;

	int32_t idx = shader->GetInput().GetBlockInput(blockIndex).GetInputIndex(name);

	// No input with that name?
	if (idx == -1)
		return;

	const auto& blockInput = shader->GetInput().GetBlockInput(blockIndex).inputs[idx];

	// Not Scalar Type?
	if (blockInput.first.inputType != EShaderInputType::Float)
		return;

	MaterialInputMap inputScalar{};
	inputScalar.index = idx;
	inputScalar.offset = blockInput.second; // The Offset in the uniform block.
	inputScalar.size = sizeof(float);
	inputScalar.data = scalar;
	matInputMap.push_back(inputScalar);
}


void RenderRscMaterial::MapParamter(const std::string& name, const glm::vec4* color)
{
	// No Input Block?
	if (blockIndex == -1)
		return;

	int32_t idx = shader->GetInput().GetBlockInput(blockIndex).GetInputIndex(name);

	// No input with that name?
	if (idx == -1)
		return;

	const auto& blockInput = shader->GetInput().GetBlockInput(blockIndex).inputs[idx];

	// Not Scalar Type?
	if (blockInput.first.inputType != EShaderInputType::Vec4)
		return;

	MaterialInputMap inputScalar{};
	inputScalar.index = idx;
	inputScalar.offset = blockInput.second; // The Offset in the uniform block.
	inputScalar.size = sizeof(glm::vec4);
	inputScalar.data = glm::value_ptr(*color);
	matInputMap.push_back(inputScalar);
}


void RenderRscMaterial::FillBuffer()
{
	// No Input Block?
	if (blockIndex == -1)
		return;

	const auto& blockInput = shader->GetInput().GetBlockInput(blockIndex);

	// Invalid?
	if (!materialBuffer)
	{
		// Allocate...
		materialBuffer = new uint8_t[ blockInput.size ];
	}

	// Fill the buffer with materail data.
	for (const auto& inputParam : matInputMap)
	{
		memcpy(materialBuffer + inputParam.offset, inputParam.data, inputParam.size);
	}


}


void RenderRscMaterial::UpdateUniformBuffer()
{
	// No Input Block? No Unifrom Buffer?
	if (blockIndex == -1 && ubo == nullptr)
		return;

	const auto& blockInput = shader->GetInput().GetBlockInput(blockIndex);

	// Update the unifrom buffer with the material data.
	ubo->UpdateData(blockInput.size, 0, materialBuffer);
}


void RenderRscMaterial::MakeTexturesActive()
{
	for (const auto& inputTex : matInputTexturesMap)
	{
		if (!inputTex.second)
			continue;

		RenderRscTexture* rtex = (*inputTex.second)->renderRscTexture;
		rtex->GetTexture()->Active(inputTex.first);
	}
}




} // End of namespace Raven.
