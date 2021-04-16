#pragma once


#include <vector>
#include <unordered_map>
#include "ofbx.h"
#include "Scene/Entity/Entity.h"

namespace Raven
{
	class Model;
	class Mesh;
	class Skeleton;
	class Animation;
	struct AnimationClip;


	class FbxLoader 
	{
	public:
		~FbxLoader();
		void Load(const std::string& file, Model* model);
		void LoadHierarchy(const std::string& file, Model* model);
		void LoadAnimation(const std::string& file, Model* model);
		void LoadBones(const std::string& file);

		inline auto& GetAnimation() { return animation; }
		inline auto& GetSkeleton() { return skeleton; }


	private:
		std::shared_ptr<AnimationClip> ImportAnimation(Model* model, int32_t index, float frameRate);
		void LoadBones(const ofbx::Mesh* fbxMesh, Model* model);
		void LoadMeshes(Model* model);
		void LoadWeight(const ofbx::Skin* skin, int32_t firstVertexOffset, Mesh* mesh);
		Mesh* ImportMesh(Model* model, const ofbx::Mesh* aMesh, int32_t triangleStart, int32_t triangleEnd);
		void LoadAnimation(Model* model);
		void LoadBones();

		ofbx::IScene* scene = nullptr;
		std::shared_ptr<Skeleton> skeleton;
		std::shared_ptr<Animation> animation;
		std::vector<const ofbx::Object*> bones;
		std::string file;
	};


}; 