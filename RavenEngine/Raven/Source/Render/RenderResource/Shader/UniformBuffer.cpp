#include "UniformBuffer.h"
#include "Render/OpenGL/GLBuffer.h"





namespace Raven {


UniformBuffer::UniformBuffer()
	: buffer(nullptr)
{

}


UniformBuffer::~UniformBuffer()
{
	delete buffer;
}


UniformBuffer* UniformBuffer::Create(const RSInputBlockDescription& block)
{
	RAVEN_ASSERT(block.size != -1, "UniformBuffer - Invalid Block Description.");

	UniformBuffer* newUniformBuffer = new UniformBuffer();
	newUniformBuffer->blockDescription = block;
	newUniformBuffer->buffer = GLBuffer::Create(EGLBufferType::Uniform, block.size, EGLBufferUsage::DynamicDraw);

	return newUniformBuffer;
}


void UniformBuffer::UpdateData(int32_t size, int32_t offset, const void* data)
{
	buffer->Bind();
	buffer->UpdateSubData(size, offset, data);
	buffer->Unbind();
}


void UniformBuffer::BindBase()
{
	RAVEN_ASSERT(blockDescription.binding != -1, "UniformBuffer - Invalid binding.");
	buffer->BindBase(blockDescription.binding);
}


int32_t UniformBuffer::GetInputIndex(const std::string name) const
{
	for (int32_t i = 0; i < blockDescription.inputs.size(); ++i)
	{
		if (blockDescription.inputs[i].first.name == name)
			return i;
	}

	return -1;
}


} // End of namespace Raven.


