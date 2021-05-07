#pragma once
#include "Scene/System/ISystem.h"
#include <vector>
#include <memory>

namespace Raven
{
	class Camera;
	class AudioSource;

	class AudioSystem : public ISystem
	{
	public:
		static AudioSystem* Create();

		virtual ~AudioSystem() = default;
		virtual void OnInit() override = 0;
		virtual void OnUpdate(float dt, Scene* scene) override = 0;

		inline auto GetListener() const	{	return listener;	}

		inline auto AddSoundNode(const std::shared_ptr<AudioSource> & node)
		{
			soundNodes.emplace_back(node);
		}

		inline auto Clear()
		{
			soundNodes.clear();
		}
        
        bool isPaused() const { return paused; }
        void SetPaused(bool paused);

	protected:
		Camera* listener;
		std::vector<std::shared_ptr<AudioSource>> soundNodes;
        bool paused;
	};
}
