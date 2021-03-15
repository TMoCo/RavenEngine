#include "RenderRscTerrain.h"
#include "Render/OpenGL/GLTexture.h"
#include "Render/OpenGL/GLVertexArray.h"
#include "Render/OpenGL/GLBuffer.h"

#include "glm/gtx/common.hpp"
#include "glm/vec3.hpp"



using namespace Raven;




RenderRscTerrain::RenderRscTerrain()
	: heightMap(nullptr)
	, vertexArray(nullptr)
	, vertexBuffer(nullptr)
	, numVerts(0)
{

}


RenderRscTerrain::~RenderRscTerrain()
{
	if (vertexArray != nullptr)
		delete vertexArray;

	if (vertexBuffer != nullptr)
		delete vertexBuffer;

}


void RenderRscTerrain::LoadHeightMap(int width, int height, const void* data)
{
	heightMap = GLTexture::Create(EGLTexture::Texture2D, EGLFormat::R);
	heightMap->SetFilter(EGLFilter::Nearest);
	heightMap->SetWrap(EGLWrap::Repeat);

	heightMap->Bind();
	heightMap->UpdateTexData(0, width, height, data);
	heightMap->UpdateTexParams();
	heightMap->Unbind();
}


void RenderRscTerrain::GenerateTerrain(int resolution, const glm::vec2& scale)
{
	terrainScale = scale;
	std::vector<glm::vec3> terrainVerts;

	int vcount = glm::max(resolution, 10);
	terrainVerts.reserve(vcount * vcount * 6);
	float vof = (1.0f / (float)(vcount - 1));

	for (int y = 0; y < vcount; ++y)
	{
		float fy = (float)y / (float)(vcount - 1);

		for (int x = 0; x < vcount; ++x)
		{
			float fx = (float)x / (float)(vcount - 1);

			float v0 = scale.x * (fx);
			float v1 = scale.x * (fx + vof);
			float y0 = scale.y * (fy);
			float y1 = scale.y * (fy + vof);

			terrainVerts.push_back(glm::vec3(v0, 0.0f, y0));
			terrainVerts.push_back(glm::vec3(v1, 0.0f, y1));
			terrainVerts.push_back(glm::vec3(v1, 0.0f, y0));

			terrainVerts.push_back(glm::vec3(v0, 0.0f, y0));
			terrainVerts.push_back(glm::vec3(v0, 0.0f, y1));
			terrainVerts.push_back(glm::vec3(v1, 0.0f, y1));
		}
	}


	numVerts = terrainVerts.size();
	vertexBuffer = GLBuffer::Create(
		EGLBufferType::Array,
		(int)(terrainVerts.size() * sizeof(glm::vec3)),
		terrainVerts.data(),
		EGLBufferUsage::StaticDraw
	);



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


	vertexArray = GLVertexArray::Create();
	vertexArray->Build(attributes, nullptr);
}
