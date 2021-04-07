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
		static UniformBuffer* Create(const RSInputBlockDescription& block);

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


	private:
		// OpenGL Uniform Buffer.
		GLBuffer* buffer;

		// The Input Block Description used to create this unifrom match.
		RSInputBlockDescription blockDescription;
	};

}
