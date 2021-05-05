#include "SoundComponent.h"
#include "Audio/AudioSource.h"
#include <glm/gtc/type_ptr.hpp>
#include "ImGui/ImGuiHelpers.h"

namespace Raven 
{
	SoundComponent::SoundComponent()
	{
		soundNode.reset(AudioSource::Create());
	}

	SoundComponent::~SoundComponent()
	{
	
	}

	void SoundComponent::OnImGui()
	{
		auto loop = soundNode->GetLooping();
		auto paused = soundNode->GetPaused();
		auto pitch =  soundNode->GetPitch();

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
		ImGui::Columns(2);
		ImGui::Separator();



//un-implemented, because of implementation the space audio is difficult.
//no time to do that.

		if (ImGuiHelper::Property("Loop", loop))
		{
			soundNode->SetLooping(loop);
		}

		if (ImGuiHelper::Property("Paused", paused))
		{
			soundNode->SetPaused(paused);
		}

		std::string path = "";
		

		ImGui::Columns(1);
		ImGui::Separator();
		ImGui::PopStyleVar();


	}
};


