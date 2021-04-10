#include "RenderRscMesh.h"



#include "Render/OpenGL/GLVertexArray.h"
#include "Render/OpenGL/GLBuffer.h"



namespace Raven {




RenderRscMesh::RenderRscMesh()
	: vxarray(nullptr)
	, positionBuffer(nullptr)
	, normalBuffer(nullptr)
	, texCoordBuffer(nullptr)
	, indexBuffer(nullptr)
{

}


RenderRscMesh::~RenderRscMesh()
{
	delete vxarray;
	delete positionBuffer;
	delete normalBuffer;
	delete texCoordBuffer;
	delete indexBuffer;
}


void RenderRscMesh::Load(const std::vector<glm::vec3>& positions, const std::vector<glm::vec3>& normals,
	const std::vector<glm::vec2>& texCoord, const std::vector<unsigned int>& indices)
{
	// Create/Update Position Buffer.
	positionBuffer = GLBuffer::Create(
		EGLBufferType::Array, 
		(int)(positions.size() * sizeof(glm::vec3)),
		positions.data(),
		EGLBufferUsage::StaticDraw
	);

	// Create/Update Normal Buffer.
	normalBuffer = GLBuffer::Create(
		EGLBufferType::Array,
		(int)(normals.size() * sizeof(glm::vec3)),
		normals.data(),
		EGLBufferUsage::StaticDraw
	);

	// Create/Update TexCoordinate Buffer.
	texCoordBuffer = GLBuffer::Create(
		EGLBufferType::Array,
		(int)(texCoord.size() * sizeof(glm::vec2)),
		texCoord.data(),
		EGLBufferUsage::StaticDraw
	);

	// Create/Update Index Buffer.
	numIndices = indices.size();
	indexBuffer = GLBuffer::Create(
		EGLBufferType::Element,
		(int)(indices.size() * sizeof(unsigned int)),
		indices.data(),
		EGLBufferUsage::StaticDraw
	);



	// Build Vertex Input Description...
	std::vector<GLVABuildAttribData> attributes{
			// Attribute 0 - Positions
			{
				positionBuffer,    // Buffer
				0,                 // Index
				3,                 // Type-Size
				EGLTypes::Float,   // Type
				sizeof(glm::vec3), // Stride
				0                  // offset
			},

			// Attribute 1 - Normals
			{
				normalBuffer,      // Buffer
				1,                 // Index
				3,                 // Type-Size
				EGLTypes::Float,   // Type
				sizeof(glm::vec3), // Stride
				0                  // offset
			},

			// Attribute 2 - TexCoords
			{
				texCoordBuffer,		 // Buffer
				2,                 // Index
				2,                 // Type-Size
				EGLTypes::Float,   // Type
				sizeof(glm::vec2), // Stride
				0                  // offset
			}
	};


	vxarray = GLVertexArray::Create();
	vxarray->Build(attributes, indexBuffer);
}




} // End of namespace Raven.
