#include "RenderShaderInput.h"


#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/matrix.hpp"





#define COMMON_BLOCK_BINDING 1
#define TRANSFROM_BLOCK_BINDING 2
#define LIGHTING_BLOCK_BINDING 3
#define MATERIAL_BLOCK_BINDING 4








namespace Raven {




RSInputBlockDescription RenderShaderInput::TransfromBlock = RSInputBlockDescription::MakeTransfromBlock();
RSInputBlockDescription RenderShaderInput::LightingBlock = RSInputBlockDescription::MakeLightingBlock();
RSInputBlockDescription RenderShaderInput::CommonBlock = RSInputBlockDescription::MakeCommonBlock();

int32_t RenderShaderInput::MaterialBlockBinding = MATERIAL_BLOCK_BINDING;



// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- 
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- 



void RSInputBlockDescription::BeginUniformBlock(const std::string& name)
{
	RAVEN_ASSERT(size == -1, "Uniform block already built.");
	this->name = name;
}


void RSInputBlockDescription::EndUniformBlock()
{
	RAVEN_ASSERT(inputs.size() != 0, "Empty blocks not allowed.");

	// Compute Block Size
	size = inputs.back().second;
	size += RenderShaderInput::GetSize(inputs.back().first.inputType);
	size = RenderShaderInput::AlignOffset(size, 16); // always align the final size with 16.
}


void RSInputBlockDescription::AddInput(EShaderInputType inputType, const std::string& inputName, int32_t offset)
{
	RAVEN_ASSERT(!name.empty(), "You must begin block to be able to add input.");

	// Offset not specified?
	if (offset == -1)
	{
		if (inputs.empty())
		{
			offset = 0;
		}
		else
		{
			// Compute Current Offset...
			int32_t size = inputs.back().second;
			size += RenderShaderInput::GetSize(inputs.back().first.inputType);
			offset = RenderShaderInput::AlignOffset(size, RenderShaderInput::GetAlignment(inputType));
		}
	}

	// New Input to the block
	std::pair<RSInputDescription, int32_t> blockInput;
	blockInput.first.uniformType = EShaderUniformType::UniformBlock;
	blockInput.first.inputType = inputType;
	blockInput.first.name = inputName;
	blockInput.second = offset;
	inputs.push_back(blockInput);
}


int32_t RSInputBlockDescription::GetInputIndex(const std::string& name)
{
	for (int32_t i = 0; i < (int32_t)inputs.size(); ++i)
	{
		if (inputs[i].first.name == name)
			return i;
	}

	return -1;
}


RSInputBlockDescription RSInputBlockDescription::MakeCommonBlock()
{
	RSInputBlockDescription inputblock;
	inputblock.binding = COMMON_BLOCK_BINDING;
	inputblock.BeginUniformBlock("CommonBlock");
	inputblock.AddInput(EShaderInputType::Vec3, "viewDir");
	inputblock.AddInput(EShaderInputType::Vec3, "viewPos");
	inputblock.AddInput(EShaderInputType::Float, "time");
	inputblock.EndUniformBlock();

	return inputblock;
}


RSInputBlockDescription RSInputBlockDescription::MakeTransfromBlock()
{
	RSInputBlockDescription inputblock;
	inputblock.binding = TRANSFROM_BLOCK_BINDING;
	inputblock.BeginUniformBlock("TransformBlock");
	inputblock.AddInput(EShaderInputType::Mat4, "inModelMatrix");
	inputblock.AddInput(EShaderInputType::Mat4, "inNormalMatrix");
	inputblock.AddInput(EShaderInputType::Mat4, "inViewProjMatrix");
	inputblock.EndUniformBlock();

	return inputblock;
}


RSInputBlockDescription RSInputBlockDescription::MakeLightingBlock()
{
	RSInputBlockDescription inputblock;
	inputblock.binding = LIGHTING_BLOCK_BINDING;
	inputblock.BeginUniformBlock("LightingBlock");
	inputblock.AddInput(EShaderInputType::Vec4, "lightDir");
	inputblock.AddInput(EShaderInputType::Vec4, "lightColor");
	inputblock.AddInput(EShaderInputType::Float, "lightPower");
	inputblock.EndUniformBlock();

	return inputblock;
}





// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- 
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- 





RenderShaderInput::RenderShaderInput()
{

}


RenderShaderInput::~RenderShaderInput()
{

}


void RenderShaderInput::AddInput(EShaderInputType inputType, const std::string& name)
{
	// Find index of an input with the same name.
	int32_t idx = GetInputIndex(name);

	// Exist?
	if (idx != -1)
	{
		// Override.
		inputs[idx].uniformType = EShaderUniformType::Uniform;
		inputs[idx].inputType = inputType;
	}
	else
	{
		// Add New
		RSInputDescription newInput;
		newInput.uniformType = EShaderUniformType::Uniform;
		newInput.inputType = inputType;
		newInput.name = name;
		inputs.push_back(newInput);
	}
}


void RenderShaderInput::AddSamplerInput(const std::string& name)
{
	// Find index of an input with the same name.
	int32_t idx = GetSamplerInputIndex(name);

	// Exist?
	if (idx != -1)
	{
		// Override.
		samplers[idx].uniformType = EShaderUniformType::Sampler;
		samplers[idx].inputType = EShaderInputType::Int;
	}
	else
	{
		// Add New
		RSInputDescription newInput;
		newInput.uniformType = EShaderUniformType::Sampler;
		newInput.inputType = EShaderInputType::Int;
		newInput.name = name;
		samplers.push_back(newInput);
	}
}


void RenderShaderInput::AddSamplerInputs(const std::vector<RSInputDescription>& inSamplers)
{
	for (const auto& s : inSamplers)
	{
		AddSamplerInput(s.name);
	}
}


void RenderShaderInput::AddBlockInput(const RSInputBlockDescription& block)
{
	RAVEN_ASSERT(block.size != -1, "Invalid Block.");

	// Find index of an input block with the same name.
	int32_t idx = GetBlockInputIndex(block.name);

	// Exist?
	if (idx != -1)
	{
		blockInputs[idx] = block; // Override.
	}
	else
	{
		// Add New
		blockInputs.push_back(block);
	}
}


int32_t RenderShaderInput::GetInputIndex(const std::string& name)
{
	for (int32_t i = 0; i < (int32_t)inputs.size(); ++i)
	{
		if (inputs[i].name == name)
			return i;
	}

	return -1;
}


int32_t RenderShaderInput::GetSamplerInputIndex(const std::string& name)
{
	for (int32_t i = 0; i < (int32_t)samplers.size(); ++i)
	{
		if (samplers[i].name == name)
			return i;
	}

	return -1;
}


int32_t RenderShaderInput::GetBlockInputIndex(const std::string& name)
{
	for (int32_t i = 0; i < (int32_t)blockInputs.size(); ++i)
	{
		if (blockInputs[i].name == name)
			return i;
	}

	return -1;
}


int32_t RenderShaderInput::GetSize(EShaderInputType type)
{
	switch (type)
	{
	case Raven::EShaderInputType::Float: return sizeof(float);
	case Raven::EShaderInputType::Int:   return sizeof(int32_t);
	case Raven::EShaderInputType::Vec2:  return sizeof(glm::vec2);
	case Raven::EShaderInputType::Vec3:  return sizeof(glm::vec3);
	case Raven::EShaderInputType::Vec4:  return sizeof(glm::vec4);
	case Raven::EShaderInputType::Mat3:  return sizeof(glm::mat3);
	case Raven::EShaderInputType::Mat4:  return sizeof(glm::mat4);
	}

	return -1;
}


int32_t RenderShaderInput::GetAlignment(EShaderInputType type)
{
	switch (type)
	{
	case Raven::EShaderInputType::Float: return 4;
	case Raven::EShaderInputType::Int:   return 4;
	case Raven::EShaderInputType::Vec2:  return 8;

	case Raven::EShaderInputType::Vec3:
	case Raven::EShaderInputType::Vec4:
		return 16;

	// Each column align with 16.
	case Raven::EShaderInputType::Mat3: 
	case Raven::EShaderInputType::Mat4:
		return 16;
	}

	RAVEN_ASSERT(0, "GetAlignment() - Shader Input Type Not Supported.");
	return -1;
}


int32_t RenderShaderInput::AlignOffset(int32_t offset, int32_t align)
{
	int32_t m = (offset % align);

	// Aligned?
	if (m == 0)
		return offset;

	return offset + (align - m);
}



} // End of namespace Raven.


