
#include "GLVertexArray.h"

#include "GL/glew.h"



using namespace Raven;





GLVertexArray::GLVertexArray()
	: id(0)
{

}


GLVertexArray::~GLVertexArray()
{
	if (id != 0)
	{
		glDeleteVertexArrays(1, &id);
	}
}


GLVertexArray* GLVertexArray::Create()
{
	GLVertexArray* vxarray = new GLVertexArray();

	glGenVertexArrays(1, &vxarray->id);

	return vxarray;
}


void GLVertexArray::Build(const std::vector<GLVABuildAttribData>& data, const GLBuffer* elementBuffer)
{
	glBindVertexArray(id);


	// Attributes Bindings...
	for (const auto& vd : data)
	{
		glBindBuffer(GL_ARRAY_BUFFER, vd.buffer->GetID());

		glVertexAttribPointer(
			vd.index,
			vd.size,
			(GLENUM)vd.type,
			GL_FALSE,
			vd.stride,
			(void*)(std::ptrdiff_t)vd.offset);

		glEnableVertexAttribArray(vd.index);
	}


	// Element Buffer?
	if (elementBuffer != nullptr)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer->GetID());
	}

	// Unbind...
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}


void GLVertexArray::Bind()
{
	glBindVertexArray(id);
}


void GLVertexArray::Unbind()
{
	glBindVertexArray(0);
}
