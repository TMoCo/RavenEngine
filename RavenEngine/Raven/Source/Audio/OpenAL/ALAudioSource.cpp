#include "ALAudioSource.h"
#include "ALAudioClip.h"
#include "ALSystemImpl.h"
#include "Core/Camera.h"

#include <glm/gtc/type_ptr.hpp>

namespace Raven
{
	ALAudioSource::ALAudioSource()
	{
		alGenSources(1, &source);
	}

	ALAudioSource::~ALAudioSource()
	{
		Stop();
		alDeleteSources(1, &source);
	}

	void ALAudioSource::OnUpdate(float msec)
	{
		alSourcef(source, AL_GAIN, volume);
		alSourcef(source, AL_PITCH, pitch);
	}

	void ALAudioSource::Pause()
	{
		alSourcePause(source);
		paused = true;
	}

	void ALAudioSource::Resume()
	{
		alSourcePlay(source);
		paused = false;
	}

	void ALAudioSource::Stop()
	{
		alSourceStop(source);
	}

	void ALAudioSource::SetAudioClip(AudioClip* s)
	{
		clip = s;
		if(clip)
		{
			alSourcei(source, AL_BUFFER, static_cast<ALAudioClip*>(clip)->GetBuffer());
			alSourcei(source, AL_LOOPING, isLooping ? 1 : 0);
			alSourcef(source, AL_GAIN, volume);
			alSourcef(source, AL_PITCH, pitch);
		}
	}
}
