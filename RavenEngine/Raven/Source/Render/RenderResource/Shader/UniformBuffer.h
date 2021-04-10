#pragma once





#include "Utilities/Core.h"
#include "Render/OpenGL/GLTypes.h"
#include "RenderShaderInput.h"








namespace Raven
{
	class GLBuffer;


	// UniformBuffer:
	//		- Manage a unifrom buffer object.
	class UniformBuffer
	{
		// Construct.
		UniformBuffer();

	public:
		// Destruct.
		~UniformBuffer();

		// Create a unifrom buffer from a block description.
		// @param isCPU: if true the unfirom has a cpu buffer for holding the data before updating if needed.
		static UniformBuffer* Create(const RSInputBlockDescription& block, bool isCreateCPU);

		// Return true if this unifrom block description valid.
		inline bool HasValidBlock() const { return blockDescription.size != -1; }

		// Update the unifrom buffer with the new data. 
		void UpdateData(int32_t size, int32_t offset, const void* data);

		// Bind the unifrom buffer to the binding index in the description.
		void BindBase();

		// Return the input description of the uniform block.
		inline const RSInputBlockDescription& GetDescription() const { return blockDescription; }

		// Get the index of an input inside the block.
		int32_t GetInputIndex(const std::string name) const;

		// Get an input inside the block.
		inline const RSInputDescription& GetInput(int32_t idx) const { return blockDescription.inputs[idx].first; }

		// Get an input offset from the start of the block.
		inline int32_t GetInputOffset(int32_t idx) const { return blockDescription.inputs[idx].second; }

		// Return true if the this uniform has buffer on cpu.
		inline bool HasCPUBuffer() { return cpuData != nullptr; }

		// Set data on cpu buffer.
		void SetData(int32_t offset, int32_t size, const void* data);
		
		//  Set a value for the cpu buffer to be used while updating the OpenGL Unfirom Buffer.
		template<class T>
		void SetDataValue(int32_t index, const T& value)
		{
			const auto& input = GetInput(index);
			int32_t offset = GetInputOffset(index);
			int32_t size = RenderShaderInput::GetSize(input.inputType);
			RAVEN_ASSERT(input.count == 1, "Invalid Operation - trying to set a value as an array.");
			RAVEN_ASSERT(sizeof(T) == size, "Invalid DataType - Size does not match.");

			SetData(offset, size, &value);
		}

		// Update the the unfriom buffer from the cpu buffer.
		void Update();

	private:
		// OpenGL Uniform Buffer.
		GLBuffer* buffer;

		// The Input Block Description used to create this unifrom match.
		RSInputBlockDescription blockDescription;

		// Holding the data before updating if needed.
		uint8_t* cpuData;
	};

}
