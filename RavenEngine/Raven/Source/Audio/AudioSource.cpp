
#include "AudioSource.h"
#include "OpenAL/ALAudioSource.h"
#include "Logger/Console.h"

namespace Raven
{
	AudioSource* AudioSource::Create()
	{
		return new ALAudioSource();
	}
	AudioSource::AudioSource()
	{
		Reset();
	}

	void AudioSource::Reset()
	{
		pitch = 1.0f;
		volume = 1.0f;
		isLooping = true;
		clip = nullptr;
		paused = false;
	}

	AudioSource::~AudioSource()
	{
	}

	void AudioSource::SetAudioClip(AudioClip *s)
	{
		clip = s;
	}
}
