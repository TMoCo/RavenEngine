
#include "MeshRenderer.h"
#include "Engine.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Scene/Entity/EntityManager.h"
#include "Model_deprecated.h"
#include "ImGui/ImGuiHelpers.h"
#include "Scene/Component/Transform.h"
#include <imgui.h>


namespace Raven
{

	void MeshRenderer::GetMeshFromModel()
	{
		auto currentScene = Engine::Get().GetModule<SceneManager>()->GetCurrentScene();
		auto& enManager = currentScene->GetEntityManager();
		if (auto model = GetEntity().TryGetComponentFromParent<Model_deprecated>()) 
		{
			mesh = model->GetMeshSection(meshIndex);
		}
	}

	void SkinnedMeshRenderer::OnImGui()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
		ImGui::Columns(2);
		ImGui::Separator();


		std::string name = "No root bone";
		
		ImGuiHelper::Property("Root Bone", 
			skeleton.GetRoot() ? skeleton.GetRoot()->name :
			name
		);

		std::string index = std::to_string(meshIndex);
		ImGuiHelper::Property("Mesh Index", index,true);


		ImGui::Columns(1);
		ImGui::Separator();
		ImGui::PopStyleVar();

	}

	void SkinnedMeshRenderer::GetMeshFromModel()
	{
		auto currentScene = Engine::Get().GetModule<SceneManager>()->GetCurrentScene();
		auto& enManager = currentScene->GetEntityManager();
	
		auto model = GetEntity().TryGetComponentFromParent<Model_deprecated>();
		if (model) {
			mesh = model->GetMeshSection(meshIndex);
			if (skeleton.GetRoot()->localTransform == nullptr)
			{
				skeleton.ResetTransfromTarget(model->GetEntity());
			}
		}
	}


	void SkinnedMeshRenderer::UpdateBones()
	{
		if (bones.size() != skeleton.GetBoneSize())
			bones.resize( skeleton.GetBoneSize() );

		for (auto i = 0; i < skeleton.GetBoneSize(); i++)
		{
			auto& bone = skeleton.GetBone(i);
			bones[i] = bone.localTransform->GetWorldMatrix() * bone.offsetMatrix;
		}
	}


};