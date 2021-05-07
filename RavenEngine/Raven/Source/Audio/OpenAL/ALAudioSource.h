#pragma once

#include "Audio/AudioSource.h"

#include <AL/al.h>

#define NUM_STREAM_BUFFERS 3

namespace Raven
{
	class ALAudioSource : public AudioSource
	{
	public:
		ALAudioSource();
		virtual ~ALAudioSource();

		void OnUpdate(float msec) override;
		void Pause() override;
		void Resume() override;
		void Stop() override;
		void SetAudioClip(AudioClip *s) override;

	private:
		ALuint source;
		ALuint streamBuffers[NUM_STREAM_BUFFERS];
	};
}