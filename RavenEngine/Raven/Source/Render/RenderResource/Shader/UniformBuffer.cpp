#include "UniformBuffer.h"
#include "Render/OpenGL/GLBuffer.h"





namespace Raven {


UniformBuffer::UniformBuffer()
	: buffer(nullptr)
	, cpuData(nullptr)
{

}


UniformBuffer::~UniformBuffer()
{
	delete buffer;
	delete cpuData;
}


UniformBuffer* UniformBuffer::Create(const RSInputBlockDescription& block, bool isCreateCPU)
{
	RAVEN_ASSERT(block.size != -1, "UniformBuffer - Invalid Block Description.");

	UniformBuffer* newUniformBuffer = new UniformBuffer();
	newUniformBuffer->blockDescription = block;
	newUniformBuffer->buffer = GLBuffer::Create(EGLBufferType::Uniform, block.size, EGLBufferUsage::DynamicDraw);

	if (isCreateCPU)
	{
		newUniformBuffer->cpuData = new uint8_t[block.size];
	}

	return newUniformBuffer;
}


void UniformBuffer::UpdateData(int32_t size, int32_t offset, const void* data)
{
	buffer->Bind();
	buffer->UpdateSubData(size, offset, data);
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


void UniformBuffer::SetData(int32_t offset, int32_t size, const void* data)
{
	RAVEN_ASSERT(HasCPUBuffer(), "This Buffer has no data on cpu.");
	memcpy(cpuData + offset, data, (size_t)size);
}


void UniformBuffer::Update()
{
	RAVEN_ASSERT(HasCPUBuffer(), "This Buffer has no data on cpu.");
	buffer->Bind();
	buffer->UpdateSubData(blockDescription.size, 0, cpuData);
}

} // End of namespace Raven.


