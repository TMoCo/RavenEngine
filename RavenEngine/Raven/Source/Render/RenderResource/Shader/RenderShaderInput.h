#pragma once





#include "Utilities/Core.h"
#include "Render/OpenGL/GLTypes.h"


#include <vector>






namespace Raven
{
	// Different unifrom types defined in shader.
	enum class EShaderUniformType
	{
		// Simple uniforms that is set directly using GLShader::SetUnfirom.
		Uniform,

		// Uniform block that is bound to uniform buffer.
		UniformBlock,

		// Texture Samplers.
		Sampler
	};



	// Different data types of shader input
	enum class EShaderInputType
	{
		Unknown,
		Float,
		Int,
		Vec2,
		Vec3,
		Vec4,
		Mat3,
		Mat4
	};


	// Render Shader Input.
	struct RSInputDescription
	{
		// Uniform type.
		EShaderUniformType uniformType;
		
		// Input Type.
		EShaderInputType inputType;

		// The input name.
		std::string name;
	};


	// Render Shader Unifrom Block Input Description
	class RSInputBlockDescription
	{
	public:
		// The Block Name
		std::string name;

		// The overall size of the block.
		int32_t size;

		// The inputs and their offset inside the block
		std::vector< std::pair<RSInputDescription, int32_t> > inputs;

		// The binding used by this block.
		int32_t binding;

	public:
		// Construct.
		RSInputBlockDescription()
			: size(-1)
			, binding(-1)
		{

		}

		// Begin Defining a new input of a shader.
		void BeginUniformBlock(const std::string& name);

		// End Defining a input of a shader.
		void EndUniformBlock();

		// Specify the input in a unifrom block, if offset is -1 then it will compute the offset based on the previous input.
		// This is called between Begin/End uniform block.
		void AddInput(EShaderInputType inputType, const std::string& inputName, int32_t offset = -1);

		// Return the index of an input with that name.
		int32_t GetInputIndex(const std::string& name);

	public:
		// Utility functions for making specific/global uniform blocks to a shader input.
		static RSInputBlockDescription MakeCommonBlock();
		static RSInputBlockDescription MakeTransfromBlock();
		static RSInputBlockDescription MakeLightingBlock();
	};



	// RenderShaderInput:
	//		- Provide an input description for a shader, used by systems that need to know
	//			what the shader expect.
	class RenderShaderInput
	{
	public:
		// Construct.
		RenderShaderInput();

		// Destruct.
		~RenderShaderInput();

		// Specify the input of a normal unifrom.
		void AddInput(EShaderInputType inputType, const std::string& name);

		// Specify the input of a sampler.
		void AddSamplerInput(const std::string& name);
		void AddSamplerInputs(const std::vector<RSInputDescription>& inSamplers);

		// Add unifrom block as input.
		void AddBlockInput(const RSInputBlockDescription& block);

		// Return the index of an input with that name.
		int32_t GetInputIndex(const std::string& name);

		// Return the index of an input sampler with that name.
		int32_t GetSamplerInputIndex(const std::string& name);

		// Return the index of an input uniform block with that name.
		int32_t GetBlockInputIndex(const std::string& name);

		// Return the input associated of that name.
		inline RSInputDescription& GetInput(int32_t idx) { return inputs[idx]; }
		inline const RSInputDescription& GetInput(int32_t idx) const { return inputs[idx]; }

		// Return the block input associated of that name.
		inline RSInputBlockDescription& GetBlockInput(int32_t idx) { return blockInputs[idx]; }
		inline const RSInputBlockDescription& GetBlockInput(int32_t idx) const { return blockInputs[idx]; }

		// Return the sampler input associated of that name.
		inline RSInputDescription& GetSamplerInput(int32_t idx) { return samplers[idx]; }
		inline const RSInputDescription& GetSamplerInput(int32_t idx) const { return samplers[idx]; }

		// Return the number of block inputs.
		inline uint32_t GetNumBlocks() { return static_cast<uint32_t>(blockInputs.size()); }

		// Return the number of samplers
		inline uint32_t GetNumSamplers() { return static_cast<uint32_t>(samplers.size()); }

	public:
		// Return the size of an input type.
		static int32_t GetSize(EShaderInputType type);

		// Return the memory alignment of an input type.
		static int32_t GetAlignment(EShaderInputType type);

		// Return the offset after alignment.
		static int32_t AlignOffset(int32_t offset, int32_t align);

	public:
		// Global Uniform Buffer Description...
		static RSInputBlockDescription CommonBlock;
		static RSInputBlockDescription TransfromBlock;
		static RSInputBlockDescription LightingBlock;

		// The binding for the material shader unifrom block input.
		static int32_t MaterialBlockBinding;

	private:
		// Uniform block inputs.
		std::vector<RSInputBlockDescription> blockInputs;

		// Uniform input.
		std::vector<RSInputDescription> inputs;

		// Sampler input.
		std::vector<RSInputDescription> samplers;
	};

}
