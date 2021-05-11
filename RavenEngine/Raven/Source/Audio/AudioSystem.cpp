#include "AudioSystem.h"
#include "Engine.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Scene/Entity/Entity.h"

#include "Scene/Component/SoundComponent.h"
#include "AudioSource.h"
#include "OpenAL/ALSystemImpl.h"

namespace Raven
{
    AudioSystem* AudioSystem::Create()
    {
        return new AudioSystemImpl();
    }
	
	void AudioSystem::SetPaused(bool paused)
	{
		this->paused = paused;
		auto scene = Engine::Get().GetModule<SceneManager>()->GetCurrentScene();
        if(!scene)
            return;

		auto soundsView = scene->GetRegistry().view<SoundComponent>();
		
		if(paused)
		{
			for (auto entity : soundsView)
				soundsView.get<SoundComponent>(entity).GetSoundNode()->Stop();
		}
		else
		{
			for(auto entity : soundsView)
			{
				auto soundNode = soundsView.get<SoundComponent>(entity).GetSoundNode();
                if(!soundNode->GetPaused())
					soundNode->Resume();
			}
		}
	}
}
