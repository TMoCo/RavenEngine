#pragma once



#include "RenderPrimitive.h"




namespace Raven
{
	class RenderRscSkinnedMesh;
	

	//	RenderMesh:
	//			- Represent a Mesh while rendering.
	class RenderSkinnedMesh : public RenderPrimitive
	{	
	public:
		// Construct.
		RenderSkinnedMesh();

		// Destruct.
		~RenderSkinnedMesh();

		// Return the primitive resource.
		virtual RenderRscPrimitive* GetRsc() override;

		// Set the mesh to be drawn.
		void SetMesh(RenderRscSkinnedMesh* inMesh);

		// Set Bones Transform.
		inline void SetBones(const std::vector<glm::mat4>* inBones) { bones = inBones; }

		// Get Bones Transform.
		inline const std::vector<glm::mat4>* GetBones() const { return bones; }

		// Draw Mesh.
		void Draw(GLShader* shader, bool isShadow) const override;

		// Return Expected Domain of this primitive.
		inline virtual ERenderShaderDomain GetDomain() { return ERenderShaderDomain::Skinned; }

	private:
		// The Mesh Render Resource.
		RenderRscSkinnedMesh* mesh;

		// Bones Transform.
		const std::vector<glm::mat4>* bones;
	};

}
