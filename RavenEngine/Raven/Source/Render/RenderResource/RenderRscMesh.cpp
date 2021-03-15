#include "RenderRscMesh.h"



#include "Render/OpenGL/GLVertexArray.h"
#include "Render/OpenGL/GLBuffer.h"



using namespace Raven;




RenderRscMesh::RenderRscMesh()
	: vertexArray(nullptr)
	, vertexBuffer(nullptr)
	, indexBuffer(nullptr)
{

}


RenderRscMesh::~RenderRscMesh()
{
	if (vertexArray != nullptr)
		delete vertexArray;

	if (vertexBuffer != nullptr)
		delete vertexBuffer;

	if (indexBuffer != nullptr)
		delete indexBuffer;
}


void RenderRscMesh::Load(const std::vector<glm::vec3>& verts, const std::vector<unsigned int>& indices)
{
	vertexBuffer = GLBuffer::Create(
		EGLBufferType::Array, 
		(int)(verts.size() * sizeof(glm::vec3)),
		verts.data(),
		EGLBufferUsage::StaticDraw
	);


	indexBuffer = GLBuffer::Create(
		EGLBufferType::Element,
		(int)(indices.size() * sizeof(unsigned int)),
		indices.data(),
		EGLBufferUsage::StaticDraw
	);


	vertexArray = GLVertexArray::Create();

	std::vector<GLVABuildAttribData> attributes{
			// Attribute 0
			{
				vertexBuffer,      // Buffers
				0,                 // Index
				3,                 // Type-Size
				EGLTypes::Float,   // Type
				sizeof(glm::vec3), // Stride
				0                  // offset
			}
	};

	vertexArray->Build(attributes, indexBuffer);
}


