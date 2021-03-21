#pragma once





#include "RenderRscPrimitive.h"

#include "glm/vec3.hpp"
#include <vector>





namespace Raven
{
	class GLVertexArray;
	class GLBuffer;



	//
	//
	class RenderRscMesh : public RenderRscPrimitive
	{
	public:
		// Construct.
		RenderRscMesh();

		// Destruct.
		~RenderRscMesh();

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


