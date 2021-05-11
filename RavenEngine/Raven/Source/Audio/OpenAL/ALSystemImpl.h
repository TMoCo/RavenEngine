
#include "Audio/AudioSystem.h"

#include <AL/al.h>
#include <AL/alc.h>

namespace Raven
{
	class AudioSystemImpl : public AudioSystem
	{
	public:
		AudioSystemImpl(int32_t numChannels = 8);
		~AudioSystemImpl();

		void OnInit() override;
		void OnUpdate(float dt, Scene* scene) override;
		void UpdateListener();
		void OnImGui() override;

	private:
		ALCcontext* context = nullptr;
		ALCdevice*  device = nullptr;
		int32_t numChannels = 0;
	};
}
