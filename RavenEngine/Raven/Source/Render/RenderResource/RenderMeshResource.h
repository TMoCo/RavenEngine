#pragma once





#include "IRenderResource.h"

#include "glm/vec3.hpp"
#include <vector>



namespace Raven
{
	class GLVertexArray;
	class GLBuffer;



	//
	//
	class RenderMeshResource : public IRenderResource
	{
	public:
		// Construct.
		RenderMeshResource();

		// Destruct.
		~RenderMeshResource();

		//
		void Load(const std::vector<glm::vec3>& verts, const std::vector<unsigned int>& indices);

		//
		inline GLVertexArray* GetArray() { return vertexArray; }

	private:
		//
		GLVertexArray* vertexArray;

		//
		GLBuffer* vertexBuffer;

		//
		GLBuffer* indexBuffer;
	};

}


