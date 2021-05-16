#pragma once




#include "glm/vec3.hpp"
#include "glm/vec2.hpp"
#include "glm/matrix.hpp"

#include <vector>



namespace Raven
{
	class GLVertexArray;
	class GLBuffer;
	class GLShader;


	// RenderScreen:
	//    - Render a sphere covering the entire screen.
	//
	class RenderSphere
	{
		// Cosntruct.
		RenderSphere();

	public:
		// Destruct.
		~RenderSphere();

		// Create a rende screen.
		static RenderSphere* Create();

		// Set View to use for rendering the sphere.
		void SetView(const glm::mat4& mtx);

		// Set Proj to use for rendering the sphere.
		void SetProj(const glm::mat4& mtx);

		// Draw.
		void Draw(GLShader* shader);

		// Draw.
		void DrawSky(GLShader* shader);


	private:
		// Create sphere geometry.
		static void MakeSphere(std::vector<glm::vec3>& outVerts, std::vector<uint32_t>& outIndices);

	private:
		// The OpenGL Vertex Array of the mesh, defines mesh vertex input.
		GLVertexArray* vxArray;

		// OpenGL Buffer for Sphere Positions.
		GLBuffer* vxBuffer;

		// OpenGL Index/Element Buffer.
		GLBuffer* idxBuffer;

		// The number of indices in the index buffer.
		int32_t numIndices;

		// The view matrix of the sphere.
		glm::mat4 view;

		// The projection matrix of the sphere.
		glm::mat4 proj;

	};

}


