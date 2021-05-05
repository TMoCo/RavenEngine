#pragma once


#include <vector>
#include <unordered_map>
#include "ofbx.h"
#include "Scene/Entity/Entity.h"

namespace Raven
{
	class Model_deprecated;
	class SkinnedMesh;
	class SkinnedMeshSection;
	class Skeleton;
	class Animation;
	class AnimationClip;

	// deprecated Moved to Fbx Importer.
	class FbxLoader_deprecated
	{
	public:
		~FbxLoader_deprecated();
		void Load(const std::string& file, Model_deprecated* model);
		void LoadHierarchy(const std::string& file, Model_deprecated* model);
		void LoadAnimation(const std::string& file, Model_deprecated* model);
		void LoadBones(const std::string& file);

		inline auto& GetAnimation() { return animation; }
		inline auto& GetSkeleton() { return skeleton; }


	private:
		std::shared_ptr<AnimationClip> ImportAnimation(Model_deprecated* model, int32_t index, float frameRate);
		void LoadBones(const ofbx::Mesh* fbxMesh, Model_deprecated* model);
		void LoadMeshes(Model_deprecated* model);
		void LoadWeight(const ofbx::Skin* skin, int32_t firstVertexOffset, SkinnedMeshSection* mesh);
		void ImportMesh(Model_deprecated* model, const ofbx::Mesh* aMesh, int32_t triangleStart, int32_t triangleEnd);
		void LoadAnimation(Model_deprecated* model);
		void LoadBones();

		ofbx::IScene* scene = nullptr;
		Skeleton * skeleton = nullptr;
		std::shared_ptr<Animation> animation;
		std::vector<const ofbx::Object*> bones;
		std::string file;
	};


}; 